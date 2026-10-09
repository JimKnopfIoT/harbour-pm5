#include "proxmark.h"

#include <QDebug>
#include <QDateTime>
#include <QFile>

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#define PORT "/dev/ttyACM0"

#define CMD_MAGIC   0x61334d50u   // "PM3a"
#define RESP_MAGIC  0x62334d50u   // "PM3b"
#define CMD_POST    0x3361        // "a3"
#define RESP_POST   0x3362        // "b3"

Proxmark::Proxmark(QObject* parent) :
    QObject(parent),
    iFd(-1),
    iMfFound(0),
    iMfDumpBlocks(0),
    iBusy(false)
{
    iStatus = tr("Not connected");
}

Proxmark::~Proxmark()
{
    closePort();
}

QString Proxmark::portPath() const
{
    return QStringLiteral(PORT);
}

bool Proxmark::present() const
{
    return QFile::exists(QStringLiteral(PORT));
}

void Proxmark::pushState()
{
    QVariantMap m;
    m["present"]      = present();
    m["connected"]    = connected();
    m["version"]      = iVersion;
    m["model"]        = iModel;
    m["fwVer"]        = iFwVer;
    m["romVer"]       = iRomVer;
    m["status"]       = iStatus;
    m["busy"]         = iBusy;
    m["lastTag"]      = iLastTag;
    m["lastTagType"]  = iLastTagType;
    m["hfUid"]        = iHfUid;
    m["hfType"]       = iHfType;
    m["hfDetails"]    = iHfDetails;
    m["hfFields"]     = iHfFields;
    m["mfResult"]     = iMfResult;
    m["mfFound"]      = iMfFound;
    m["mfDump"]       = iMfDump;
    m["mfDumpBlocks"] = iMfDumpBlocks;
    m["logLines"]     = iLog;
    emit snapshot(m);
}

void Proxmark::setStatus(const QString& s)
{
    iStatus = s;
    // Also to the journal, so the first connection attempt can be diagnosed
    // from the outside - the sailjail-vs-open question is answered here.
    qWarning() << "[proxmark]" << s;
    emit statusChanged();
    appendLog(s);
}

void Proxmark::appendLog(const QString& s)
{
    // Timestamp every line so each operation is unambiguous in the transcript.
    const QString line =
        QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd HH:mm:ss  ")) + s;
    iLog.append(line);   // chronological: oldest first, newest at the bottom
    while (iLog.size() > 200) iLog.removeFirst();
    emit logLinesChanged();
    emit logLine(line);
    pushState();
}

// Start a fresh log (new read) that opens with the (freshly timestamped)
// device header.
void Proxmark::resetLog()
{
    iLog.clear();
    emit logLinesChanged();
    for (int k = 0; k < iDevHeader.size(); k++) appendLog(iDevHeader.at(k));
}

void Proxmark::setBusy(bool b)
{
    if (iBusy == b) return;
    iBusy = b;
    emit busyChanged();
    pushState();
}

// --------------------------------------------------------------------------
// Port
// --------------------------------------------------------------------------

bool Proxmark::openPort()
{
    if (iFd >= 0) return true;

    // O_NOCTTY: this is not our controlling terminal. No O_NONBLOCK - we want
    // a normal blocking fd and drive timeouts with select().
    iFd = ::open(PORT, O_RDWR | O_NOCTTY);
    if (iFd < 0) {
        const int e = errno;
        // errno is the whole story here: EACCES means sailjail or group,
        // ENOENT means unplugged. This is exactly what the first run needs
        // to tell us.
        setStatus(tr("open(%1) failed: %2 (errno %3)")
            .arg(QStringLiteral(PORT))
            .arg(QString::fromLocal8Bit(strerror(e)))
            .arg(e));
        return false;
    }

    struct termios tio;
    if (tcgetattr(iFd, &tio) != 0) {
        setStatus(tr("tcgetattr failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
        closePort();
        return false;
    }
    cfmakeraw(&tio);
    // CDC-ACM ignores the line rate, but set something sane anyway.
    cfsetispeed(&tio, B115200);
    cfsetospeed(&tio, B115200);
    tio.c_cflag |= (CLOCAL | CREAD);
    tio.c_cflag &= ~CRTSCTS;
    tio.c_cc[VMIN] = 0;
    tio.c_cc[VTIME] = 0;
    if (tcsetattr(iFd, TCSANOW, &tio) != 0) {
        setStatus(tr("tcsetattr failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
        closePort();
        return false;
    }

    tcflush(iFd, TCIOFLUSH);
    iRx.clear();
    return true;
}

void Proxmark::closePort()
{
    if (iFd >= 0) {
        ::close(iFd);
        iFd = -1;
    }
}

// --------------------------------------------------------------------------
// Framing
// --------------------------------------------------------------------------

QByteArray Proxmark::frameCommand(quint16 cmd, const QByteArray& data, bool ng)
{
    QByteArray f;
    const quint32 magic = CMD_MAGIC;
    f.append((char)(magic & 0xff));
    f.append((char)((magic >> 8) & 0xff));
    f.append((char)((magic >> 16) & 0xff));
    f.append((char)((magic >> 24) & 0xff));

    // length is 15 bits, ng is bit 15
    quint16 lenng = (quint16)(data.size() & 0x7fff);
    if (ng) lenng |= 0x8000;
    f.append((char)(lenng & 0xff));
    f.append((char)((lenng >> 8) & 0xff));

    f.append((char)(cmd & 0xff));
    f.append((char)((cmd >> 8) & 0xff));

    f.append(data);

    // No CRC over USB: the postamble magic stands in for it.
    f.append((char)(CMD_POST & 0xff));
    f.append((char)((CMD_POST >> 8) & 0xff));
    return f;
}

bool Proxmark::sendCommand(quint16 cmd, const QByteArray& data, bool ng)
{
    if (iFd < 0) return false;
    const QByteArray f = frameCommand(cmd, data, ng);
    int off = 0;
    while (off < f.size()) {
        const ssize_t n = ::write(iFd, f.constData() + off, f.size() - off);
        if (n < 0) {
            if (errno == EINTR) continue;
            setStatus(tr("write failed: %1").arg(QString::fromLocal8Bit(strerror(errno))));
            return false;
        }
        off += (int)n;
    }
    return true;
}

bool Proxmark::readResponse(quint16* cmd, qint8* status, QByteArray* data,
    int timeoutMs)
{
    // Preamble is 10 bytes: magic(4) len/ng(2) status(1) reason(1) cmd(2).
    const int PRE = 10;

    struct timeval start;
    gettimeofday(&start, Q_NULLPTR);

    for (;;) {
        // Do we already have a full frame buffered?
        int magicAt = -1;
        for (int i = 0; i + 4 <= iRx.size(); i++) {
            const quint32 m = (quint8)iRx.at(i)
                | ((quint8)iRx.at(i + 1) << 8)
                | ((quint8)iRx.at(i + 2) << 16)
                | ((quint32)(quint8)iRx.at(i + 3) << 24);
            if (m == RESP_MAGIC) { magicAt = i; break; }
        }
        if (magicAt >= 0 && iRx.size() >= magicAt + PRE) {
            const int p = magicAt;
            const quint16 lenng = (quint8)iRx.at(p + 4) | ((quint8)iRx.at(p + 5) << 8);
            const int len = lenng & 0x7fff;
            const qint8 st = (qint8)iRx.at(p + 6);
            const quint16 rcmd = (quint8)iRx.at(p + 8) | ((quint8)iRx.at(p + 9) << 8);
            const int total = PRE + len + 2;   // + postamble/crc
            if (iRx.size() >= magicAt + total) {
                if (cmd) *cmd = rcmd;
                if (status) *status = st;
                if (data) *data = iRx.mid(p + PRE, len);
                iRx.remove(0, magicAt + total);
                return true;
            }
        }

        // Timeout check
        struct timeval now;
        gettimeofday(&now, Q_NULLPTR);
        const long elapsed = (now.tv_sec - start.tv_sec) * 1000
            + (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= timeoutMs) return false;

        // Wait for more bytes
        fd_set rf;
        FD_ZERO(&rf);
        FD_SET(iFd, &rf);
        struct timeval tv;
        const long remain = timeoutMs - elapsed;
        tv.tv_sec = remain / 1000;
        tv.tv_usec = (remain % 1000) * 1000;
        const int s = ::select(iFd + 1, &rf, Q_NULLPTR, Q_NULLPTR, &tv);
        if (s < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        if (s == 0) return false;   // timeout

        char buf[512];
        const ssize_t n = ::read(iFd, buf, sizeof(buf));
        if (n > 0) {
            iRx.append(buf, (int)n);
        } else if (n < 0 && errno != EINTR && errno != EAGAIN) {
            return false;
        }
    }
}

// --------------------------------------------------------------------------
// Public operations
// --------------------------------------------------------------------------

void Proxmark::connectDevice()
{
    if (connected()) return;
    setBusy(true);

    if (!present()) {
        setStatus(tr("No %1 - is the Proxmark plugged in?").arg(QStringLiteral(PORT)));
        setBusy(false);
        return;
    }
    if (!openPort()) {
        setBusy(false);
        return;
    }

    emit connectedChanged();
    setStatus(tr("Port open, probing..."));

    if (!ping()) {
        setStatus(tr("Port opened but the Proxmark did not answer a PING. "
                     "It may run older firmware that only speaks the legacy frame format."));
        // Keep the port open; the version attempt or a retry may still help.
    } else {
        setStatus(tr("Proxmark answered. Reading version..."));
        readVersion();
    }

    setBusy(false);
}

void Proxmark::disconnect()
{
    closePort();
    iVersion.clear();
    emit versionChanged();
    emit connectedChanged();
    setStatus(tr("Disconnected"));
}

bool Proxmark::ping()
{
    if (iFd < 0) return false;

    const QByteArray payload = QByteArrayLiteral("iNFCping");
    if (!sendCommand(CMD_PING, payload)) return false;

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    // The device may emit an unsolicited debug line first; loop a few times.
    for (int i = 0; i < 4; i++) {
        if (!readResponse(&rcmd, &st, &data, 1500)) return false;
        if (rcmd == CMD_PING) {
            return data.startsWith(payload);
        }
        if (rcmd == CMD_DEBUG_PRINT) {
            appendLog(tr("PM5: %1").arg(QString::fromLatin1(data)));
        }
    }
    return false;
}

// Strip the "OS........." / "Bootrom...." label, keep "Iceman/.../v4...."
static QString versionTail(const QString& line)
{
    const int i = line.indexOf(QStringLiteral("Iceman/"));
    return i >= 0 ? line.mid(i) : line;
}

// Just the version token, e.g. "v4.23346" from "...master/v4.23346-suspect ...".
static QString verShort(const QString& line)
{
    const int i = line.indexOf(QStringLiteral("/v"));
    if (i < 0) return QString();
    const QString s = line.mid(i + 1);
    int cut = s.size();
    for (int j = 0; j < s.size(); j++)
        if (s.at(j) == QLatin1Char('-') || s.at(j) == QLatin1Char(' ')) { cut = j; break; }
    return s.left(cut);
}

void Proxmark::readVersion()
{
    if (iFd < 0) return;
    if (!sendCommand(CMD_VERSION, QByteArray())) return;

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 2000)) break;
        if (rcmd == CMD_VERSION || rcmd == CMD_ACK) {
            // Dump the raw payload once so the exact struct layout can be
            // parsed properly instead of guessed.
            qWarning() << "[proxmark] version payload" << data.size()
                       << "bytes:" << data.toHex();
            // The payload is a struct followed by a multi-line ASCII banner
            // with ANSI colour escapes. Strip the escapes, then pick the
            // "Iceman/..." build line - that is the one worth showing.
            QString text;
            for (int j = 0; j < data.size(); j++) {
                const quint8 c = (quint8)data.at(j);
                if (c == 0x1b) {                 // ESC - skip until the 'm'
                    while (j < data.size() && data.at(j) != 'm') j++;
                    continue;
                }
                text.append((c == '\n' || (c >= 0x20 && c < 0x7f))
                    ? QChar::fromLatin1(c) : QChar::fromLatin1(' '));
            }

            // The banner carries two "Iceman/...v4...." lines - a "Bootrom"
            // one and an "OS" one. Prefer the OS line: it is the running
            // firmware and the only version fwUsesNg14a() may gate on (the
            // bootrom stays old even after a fullimage update).
            iVersion.clear();
            QString firstVer, osLn, romLn;
            const QStringList lines = text.split(QLatin1Char('\n'));
            for (int j = 0; j < lines.size(); j++) {
                const QString ln = lines.at(j).simplified();
                if (!ln.contains(QStringLiteral("Iceman/")) &&
                    !ln.contains(QStringLiteral("/v"), Qt::CaseInsensitive))
                    continue;
                if (firstVer.isEmpty()) firstVer = ln;
                if (ln.contains(QStringLiteral("Iceman/")) &&
                    ln.contains(QStringLiteral("OS"), Qt::CaseInsensitive) && osLn.isEmpty())
                    osLn = ln;
                if (ln.contains(QStringLiteral("Iceman/")) &&
                    ln.contains(QStringLiteral("Bootrom"), Qt::CaseInsensitive) && romLn.isEmpty())
                    romLn = ln;
            }
            iVersion = !osLn.isEmpty() ? osLn : firstVer;
            if (iVersion.isEmpty()) iVersion = text.simplified().left(120);
            iModel  = QStringLiteral("Proxmark 5");
            iFwVer  = verShort(osLn.isEmpty() ? iVersion : osLn);
            iRomVer = verShort(romLn);
            emit versionChanged();
            setStatus(tr("Connected"));

            // Device header: the first lines of every read's log (chronological
            // order, so top to bottom). Cached and re-seeded by resetLog().
            iDevHeader.clear();
            iDevHeader << tr("Device · Proxmark 5 @ %1").arg(QStringLiteral(PORT));
            iDevHeader << tr("Firmware · %1").arg(versionTail(osLn.isEmpty() ? iVersion : osLn));
            if (!romLn.isEmpty()) iDevHeader << tr("ROM · %1").arg(versionTail(romLn));
            for (int k = 0; k < iDevHeader.size(); k++) appendLog(iDevHeader.at(k));
            return;
        }
        if (rcmd == CMD_DEBUG_PRINT) {
            appendLog(tr("PM5: %1").arg(QString::fromLatin1(data)));
        }
    }
    setStatus(tr("Connected (no version reply)"));
}

bool Proxmark::watchProbe(quint16 cmd, const QString& marker,
    bool hexAfterTagId, QString* outId, int budgetMs)
{
    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(cmd, QByteArray())) return false;

    struct timeval start;
    gettimeofday(&start, Q_NULLPTR);
    bool found = false;

    for (;;) {
        struct timeval now;
        gettimeofday(&now, Q_NULLPTR);
        const long elapsed = (now.tv_sec - start.tv_sec) * 1000
            + (now.tv_usec - start.tv_usec) / 1000;
        if (elapsed >= budgetMs) break;

        quint16 rcmd = 0;
        qint8 st = 0;
        QByteArray data;
        if (!readResponse(&rcmd, &st, &data, budgetMs - elapsed)) break;
        if (rcmd != CMD_DEBUG_PRINT) continue;

        // Strip ANSI escapes.
        QString s;
        for (int i = 0; i < data.size(); i++) {
            const quint8 c = (quint8)data.at(i);
            if (c == 0x1b) { while (i < data.size() && data.at(i) != 'm') i++; continue; }
            if (c >= 0x20 && c < 0x7f) s.append(QChar::fromLatin1(c));
        }
        appendLog(QStringLiteral("PM5: ") + s);

        if (!s.contains(marker)) continue;

        if (hexAfterTagId) {
            // EM4100 / HID: the id is the first hex run after "TAG ID:".
            const int p = s.indexOf(QStringLiteral("TAG ID:"));
            if (p < 0) continue;
            QString hex;
            for (int i = p + 7; i < s.size(); i++) {
                const QChar ch = s.at(i);
                if (ch.isSpace()) { if (!hex.isEmpty()) break; else continue; }
                if (isxdigit(ch.toLatin1())) hex.append(ch);
                else if (!hex.isEmpty()) break;
            }
            if (!hex.isEmpty()) { *outId = hex.toUpper(); found = true; break; }
        } else {
            // IO Prox: the descriptive tail after the marker is the id.
            const int p = s.indexOf(marker);
            *outId = s.mid(p + marker.length()).simplified();
            if (!outId->isEmpty()) { found = true; break; }
        }
    }

    // Stop the watch loop and drain its final reply.
    sendCommand(CMD_BREAK_LOOP, QByteArray());
    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 3; i++) {
        if (!readResponse(&rcmd, &st, &data, 700)) break;
        if (rcmd == cmd) break;
    }
    return found;
}

static QString hexColon(const QByteArray& b)
{
    QString s;
    for (int i = 0; i < b.size(); i++) {
        if (i) s.append(QLatin1Char(':'));
        s.append(QString("%1").arg((quint8)b.at(i), 2, 16, QChar('0')).toUpper());
    }
    return s;
}

// SAK (SEL_RES) -> human readable type. This is the same table the Reference
// glossary uses; here it decides whether we are looking at MIFARE Classic.
static QString sakType(quint8 sak, bool* isClassic)
{
    if (isClassic) *isClassic = false;
    switch (sak) {
    case 0x00: return QObject::tr("MIFARE Ultralight / NTAG (Type 2)");
    case 0x08: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic 1K");
    case 0x09: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic Mini");
    case 0x18: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic 4K");
    case 0x88: if (isClassic) *isClassic = true; return QObject::tr("Infineon MIFARE Classic 1K");
    case 0x28: if (isClassic) *isClassic = true; return QObject::tr("MIFARE Classic + ISO-DEP");
    case 0x10: return QObject::tr("MIFARE Plus 2K (SL2)");
    case 0x11: return QObject::tr("MIFARE Plus 4K (SL2)");
    case 0x20: return QObject::tr("ISO 14443-4 (DESFire, Plus SL3, JCOP…)");
    default:   return QObject::tr("unknown (SAK %1)")
                   .arg(QString("%1").arg(sak, 2, 16, QChar('0')).toUpper());
    }
}

// The well-known MIFARE Classic default keys. A field card that was never
// re-keyed opens with one of these on at least some sectors.
static const char* MF_DEFAULT_KEYS[] = {
    "FFFFFFFFFFFF", "000000000000", "A0A1A2A3A4A5", "D3F7D3F7D3F7",
    "A0B0C0D0E0F0", "B0B1B2B3B4B5", "AABBCCDDEEFF", "1A2B3C4D5E6F",
    "123456789ABC", "010203040506", "112233445566", "4D3A99C351DD",
    "1A982C7E459A", "714C5C886E97", "587EE5F9350F", "A0478CC39091",
    "533CB6C723F6", "8FD0A4F256E9"
};
static const int MF_KEYCNT = sizeof(MF_DEFAULT_KEYS) / sizeof(MF_DEFAULT_KEYS[0]);

// Try the whole dictionary against one block for one key type (0=A, 1=B).
// Returns the matching 6-byte key, or an empty array.
QByteArray Proxmark::mfTryKeys(quint8 block, quint8 keyType,
    const QByteArray& keyBlock)
{
    QByteArray p;
    p.append((char)keyType);
    p.append((char)block);
    p.append((char)0);                 // clear_trace
    p.append((char)0);                 // key_count high byte
    p.append((char)MF_KEYCNT);         // key_count low byte
    p.append(keyBlock);

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_MIFARE_CHKKEYS, p)) return QByteArray();

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 4000)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_HF_MIFARE_CHKKEYS) {
            // Reply: key[6] + found(1). status 0 = ok.
            if (st == 0 && data.size() >= 7 && (quint8)data.at(6) != 0) {
                return data.left(6);
            }
            return QByteArray();
        }
    }
    return QByteArray();
}

void Proxmark::checkMifareKeys()
{
    if (iFd < 0) return;
    setBusy(true);
    iMfResult.clear();
    iMfFound = 0;
    iMfDump.clear();
    iMfDumpBlocks = 0;
    emit mfChanged();
    emit mfDumpChanged();

    // Concatenate the dictionary into one 6-byte-per-key block.
    QByteArray keyBlock;
    for (int i = 0; i < MF_KEYCNT; i++) {
        keyBlock.append(QByteArray::fromHex(MF_DEFAULT_KEYS[i]));
    }

    // MIFARE Classic 1K: 16 sectors, 4 blocks each, trailer at sector*4+3.
    const int sectors = 16;
    QStringList lines;
    int found = 0;

    for (int s = 0; s < sectors; s++) {
        setStatus(tr("Checking sector %1/%2...").arg(s + 1).arg(sectors));
        const quint8 trailer = (quint8)(s * 4 + 3);
        iMfKeyA[s] = mfTryKeys(trailer, 0, keyBlock);
        iMfKeyB[s] = mfTryKeys(trailer, 1, keyBlock);
        if (!iMfKeyA[s].isEmpty() || !iMfKeyB[s].isEmpty()) found++;
        lines << tr("Sector %1  A:%2  B:%3")
            .arg(s, 2, 10, QChar('0'))
            .arg(iMfKeyA[s].isEmpty() ? QStringLiteral("--")
                 : QString::fromLatin1(iMfKeyA[s].toHex()).toUpper())
            .arg(iMfKeyB[s].isEmpty() ? QStringLiteral("--")
                 : QString::fromLatin1(iMfKeyB[s].toHex()).toUpper());
    }

    iMfResult = lines.join(QStringLiteral("\n"));
    iMfFound = found;
    emit mfChanged();
    setStatus(tr("%1 of %2 sectors open with a default key").arg(found).arg(sectors));
    setBusy(false);
}

QByteArray Proxmark::mfReadBlock(quint8 block, quint8 keyType,
    const QByteArray& key)
{
    if (key.size() != 6) return QByteArray();

    QByteArray p;
    p.append((char)block);
    p.append((char)keyType);
    p.append(key);                     // mf_readblock_t: blockno, keytype, key[6]

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_MIFARE_READBL, p)) return QByteArray();

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 2000)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_HF_MIFARE_READBL) {
            if (st == 0 && data.size() >= 16) return data.left(16);
            return QByteArray();
        }
    }
    return QByteArray();
}

void Proxmark::dumpMifare()
{
    if (iFd < 0) return;
    if (iMfFound == 0) {
        setStatus(tr("Run the key check first"));
        return;
    }
    setBusy(true);
    iMfDump.clear();
    iMfDumpBlocks = 0;
    emit mfDumpChanged();

    const int sectors = 16;
    QStringList lines;
    int ok = 0;

    for (int s = 0; s < sectors; s++) {
        setStatus(tr("Dumping sector %1/%2...").arg(s + 1).arg(sectors));
        // Prefer key A for reading; fall back to key B.
        const bool useA = !iMfKeyA[s].isEmpty();
        const QByteArray key = useA ? iMfKeyA[s] : iMfKeyB[s];
        const quint8 keyType = useA ? 0 : 1;

        for (int b = 0; b < 4; b++) {
            const quint8 block = (quint8)(s * 4 + b);
            QByteArray blk = key.isEmpty() ? QByteArray()
                                           : mfReadBlock(block, keyType, key);
            if (blk.size() == 16) {
                if (b == 3) {
                    // Trailer: the read masks the key bytes. Rebuild from the
                    // keys we already know, keep the access bytes (6..9).
                    if (iMfKeyA[s].size() == 6) blk.replace(0, 6, iMfKeyA[s]);
                    if (iMfKeyB[s].size() == 6) blk.replace(10, 6, iMfKeyB[s]);
                }
                ok++;
            } else {
                blk = QByteArray(16, 0);   // unreadable block -> zeros
            }
            lines << QStringLiteral("%1  %2")
                .arg(block, 3, 10, QChar('0'))
                .arg(QString::fromLatin1(blk.toHex()).toUpper());
        }
    }

    iMfDump = lines.join(QStringLiteral("\n"));
    iMfDumpBlocks = ok;
    emit mfDumpChanged();
    setStatus(tr("Dumped %1 of %2 blocks").arg(ok).arg(sectors * 4));
    setBusy(false);
}

// --------------------------------------------------------------------------
// ISO14443-4 / APDU / EMV
// --------------------------------------------------------------------------

void Proxmark::addField(const QString& label, const QString& value, bool mono,
                        int level, const QString& action)
{
    QVariantMap f;
    f["label"]  = label;
    f["value"]  = value;
    f["mono"]   = mono;
    f["level"]  = level;   // 0 normal · 1 reachable-but-locked (orange) · 2 error (red)
    f["action"] = action;  // non-empty -> tappable in the UI (e.g. "pin")
    iHfFields.append(f);
}

bool Proxmark::fwUsesNg14a() const
{
    // Pick the firmware minor out of an "Iceman/.../v4.NNNNN..." build line.
    const int at = iVersion.indexOf(QStringLiteral("/v4."));
    if (at < 0) return false;                 // unknown -> assume legacy MIX
    int j = at + 4;
    QString num;
    while (j < iVersion.size() && iVersion.at(j).isDigit()) num.append(iVersion.at(j++));
    bool ok = false;
    const int minor = num.toInt(&ok);
    // The NG conversion landed between v4.21611 and v4.23346; gate above it.
    return ok && minor >= 23000;
}

QByteArray Proxmark::apduExchange(const QByteArray& apdu, bool connect)
{
    // Discovery default: field off after each exchange (clean re-select).
    return apduExchangeEx(apdu, connect, /*keepField=*/false);
}

QByteArray Proxmark::apduExchangeEx(const QByteArray& apdu, bool connect, bool keepField)
{
    if (iFd < 0) return QByteArray();
    const bool ng = fwUsesNg14a();
    // Without NO_DISCONNECT the firmware turns the field off after the exchange
    // (iso14443a.c OUT: hf_field_off), so the next CONNECT re-selects from a
    // clean power-up - right for independent discovery SELECTs. keepField adds
    // NO_DISCONNECT to hold one activation across several APDUs (SELECT -> GPO
    // -> READ RECORD), where only the first call carries CONNECT.
    quint64 flags = ISO14A_APDU | ISO14A_APPEND_CRC;
    if (connect) flags |= ISO14A_CONNECT;
    if (keepField) flags |= ISO14A_NO_DISCONNECT;

    QByteArray p;
    if (ng) {
        // iso14a_raw_cmd_t: u32 flags, u32 timeout, u32 wait_us,
        //                   u16 len, u16 lenbits, u8 data[].
        for (int i = 0; i < 4; i++) p.append((char)((flags >> (8 * i)) & 0xff));
        for (int i = 0; i < 8; i++) p.append((char)0);          // timeout, wait_us
        const quint16 alen = (quint16)apdu.size();
        p.append((char)(alen & 0xff));
        p.append((char)((alen >> 8) & 0xff));
        p.append((char)0); p.append((char)0);                   // lenbits = 0
        p.append(apdu);
    } else {
        // MIX command: 3x u64 oldargs (flags, apdu length, 0) then the APDU bytes.
        for (int i = 0; i < 8; i++) p.append((char)((flags >> (8 * i)) & 0xff));
        const quint64 alen = (quint64)apdu.size();
        for (int i = 0; i < 8; i++) p.append((char)((alen >> (8 * i)) & 0xff));
        for (int i = 0; i < 8; i++) p.append((char)0);
        p.append(apdu);
    }

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_ISO14443A_READER, p, ng)) return QByteArray();

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;

    if (ng) {
        // With CONNECT the firmware answers in TWO NG frames: first the
        // card-select (sel = select status), then the APDU response
        // (iso14443a.c ReaderIso14443a). Skip the select frame; if it reports
        // no card (sel == 0) the firmware sends no APDU frame -> give up.
        if (connect) {
            bool gotSel = false;
            for (int i = 0; i < 8; i++) {
                if (!readResponse(&rcmd, &st, &data, 2500)) break;
                if (rcmd == CMD_DEBUG_PRINT) continue;
                if (rcmd == CMD_HF_ISO14443A_READER) { gotSel = true; break; }
            }
            if (!gotSel || data.size() < 4 || (quint8)data.at(2) == 0)
                return QByteArray();
        }
        bool gotApdu = false;
        for (int i = 0; i < 8; i++) {
            if (!readResponse(&rcmd, &st, &data, 2500)) break;
            if (rcmd == CMD_DEBUG_PRINT) continue;
            if (rcmd == CMD_HF_ISO14443A_READER) { gotApdu = true; break; }
        }
        if (!gotApdu) return QByteArray();
        // iso14a_raw_resp_t: u16 len, u8 sel, u8 rfu, u8 data[]; data[] holds
        // the R-APDU including the 2 ISO14443 CRC bytes.
        if (data.size() < 4) return QByteArray();
        const int n = (quint8)data.at(0) | ((quint8)data.at(1) << 8);
        if (n < 2 || data.size() < 4 + n) return QByteArray();
        QByteArray resp = data.mid(4, n);
        resp.chop(2);   // drop the ISO14443 CRC
        return resp;
    }

    // MIX: one combined reply (oldarg0 = response length incl. 2 CRC bytes,
    // payload at [24]).
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &data, 2500)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_ACK) {
            if (data.size() < 8) return QByteArray();
            quint64 n = 0;
            for (int k = 0; k < 8; k++) n |= (quint64)(quint8)data.at(k) << (8 * k);
            if (n < 2 || data.size() < 24 + (int)n) return QByteArray();
            QByteArray resp = data.mid(24, (int)n);
            resp.chop(2);   // drop the ISO14443 CRC
            return resp;
        }
    }
    return QByteArray();
}

static QString sw(const QByteArray& r)
{
    if (r.size() < 2) return QStringLiteral("----");
    return QString("%1%2")
        .arg((quint8)r.at(r.size() - 2), 2, 16, QChar('0'))
        .arg((quint8)r.at(r.size() - 1), 2, 16, QChar('0')).toUpper();
}

// Minimal BER-TLV: collect the values of one tag (recurses into constructed).
static void tlvCollect(const QByteArray& d, quint32 tag, QList<QByteArray>* out)
{
    int i = 0;
    while (i < d.size()) {
        // The constructed bit (0x20) lives in the FIRST tag byte, even for a
        // multi-byte tag like BF0C - don't test it on the combined value.
        const quint8 b0 = (quint8)d.at(i);
        quint32 t = (quint8)d.at(i++);
        if ((t & 0x1f) == 0x1f) { if (i >= d.size()) break; t = (t << 8) | (quint8)d.at(i++); }
        if (i >= d.size()) break;
        int len = (quint8)d.at(i++);
        if (len & 0x80) {
            int nb = len & 0x7f; len = 0;
            while (nb-- && i < d.size()) len = (len << 8) | (quint8)d.at(i++);
        }
        if (len < 0 || i + len > d.size()) break;
        QByteArray val = d.mid(i, len);
        if (t == tag) out->append(val);
        if (b0 & 0x20) tlvCollect(val, tag, out);   // constructed -> recurse
        i += len;
    }
}
static QByteArray tlvFirst(const QByteArray& d, quint32 tag)
{
    QList<QByteArray> v; tlvCollect(d, tag, &v);
    return v.isEmpty() ? QByteArray() : v.first();
}

QString Proxmark::emvDiscover()
{
    // Identity documents first: eID / ePassport by AID.
    QString docType;
    {
        static const struct { const char* aid; const char* name; } docs[] = {
            { "E80704007F00070302", "Personalausweis (eID)" },
            { "A0000002471001",     "ePassport (eMRTD)" }
        };
        for (int i = 0; i < 2; i++) {
            const QByteArray aid = QByteArray::fromHex(docs[i].aid);
            QByteArray s;
            s.append((char)0x00).append((char)0xA4).append((char)0x04).append((char)0x00);
            s.append((char)aid.size()).append(aid).append((char)0x00);
            const QByteArray r = apduExchange(s, true);
            // Neutral probe line (AID + SW), so a non-match doesn't read as if
            // the card WERE an eID/passport. The name is shown only when found.
            appendLog(QStringLiteral("SEL %1 sw=%2")
                .arg(QString::fromLatin1(aid.toHex()).toUpper()).arg(sw(r)));
            if (r.size() < 2) continue;
            const QString w = sw(r);
            // Identify by the FCI DF name if present, else by the AID we sent.
            const QString df = QString::fromLatin1(tlvFirst(r, 0x84).toHex()).toUpper();
            QString name;
            for (int k = 0; k < 2; k++)
                if (df == QLatin1String(docs[k].aid)) name = QString::fromLatin1(docs[k].name);
            // Accept the AID as present only on a positive status word; a
            // payment card answers the eID SELECT with a "not found"/error SW
            // (6A82, 6A86, 0000, ...) and must not be labelled as a document.
            if (name.isEmpty() &&
                (w == QLatin1String("9000") || w == QLatin1String("6982")))
                name = QString::fromLatin1(docs[i].name);
            if (name.isEmpty()) continue;
            const QString stt = w == QLatin1String("9000") ? tr("selectable")
                : w == QLatin1String("6982") ? tr("present · PACE/PIN required")
                : (QStringLiteral("SW ") + w);
            // Reachable but the content stays locked (PACE/TA needed) -> orange.
            addField(tr("Document"), name + QStringLiteral("  ·  ") + stt, false, 1);
            if (docType.isEmpty()) docType = name;
        }
    }

    struct Known { const char* aid; const char* name; };
    static const Known known[] = {
        { "A0000000041010", "Mastercard" },
        { "A0000000043060", "Maestro" },
        { "A0000000031010", "Visa" },
        { "A0000000032010", "V PAY" },
        { "A00000002501",   "American Express" },
        { "A0000003591010028001", "girocard (ZKA)" },
        { "D27600002545500100",   "girocard" },
        { "D276000025455041",     "GeldKarte/Girogo" },
        { "A0000000651010",  "JCB" },
        { "A0000001523010",  "Discover" }
    };
    const int nknown = sizeof(known) / sizeof(known[0]);

    QList<QByteArray> aids;

    // PPSE first: SELECT 2PAY.SYS.DDF01, harvest every AID (tag 4F).
    QByteArray ppseName("2PAY.SYS.DDF01");
    QByteArray sel;
    sel.append((char)0x00).append((char)0xA4).append((char)0x04).append((char)0x00);
    sel.append((char)ppseName.size()).append(ppseName).append((char)0x00);
    QByteArray ppse = apduExchange(sel, true);
    appendLog(QStringLiteral("PPSE sw=%1").arg(sw(ppse)));
    if (ppse.size() >= 2 && (quint8)ppse.at(ppse.size()-2) == 0x90) {
        QList<QByteArray> found; tlvCollect(ppse, 0x4f, &found);
        for (int k = 0; k < found.size(); k++)
            appendLog(QStringLiteral("PPSE AID %1").arg(QString::fromLatin1(found.at(k).toHex()).toUpper()));
        aids.append(found);
    }
    // Then the known-AID list (covers cards without a PPSE, e.g. girocard).
    for (int i = 0; i < nknown; i++) {
        aids.append(QByteArray::fromHex(known[i].aid));
    }

    QString typeLabel;
    QList<QByteArray> payAids;
    QStringList seen;
    for (int i = 0; i < aids.size(); i++) {
        const QByteArray aid = aids.at(i);
        const QString aidHex = QString::fromLatin1(aid.toHex()).toUpper();
        if (aid.isEmpty() || seen.contains(aidHex)) continue;
        seen << aidHex;

        QByteArray s;
        s.append((char)0x00).append((char)0xA4).append((char)0x04).append((char)0x00);
        s.append((char)aid.size()).append(aid).append((char)0x00);
        QByteArray r = apduExchange(s, true);
        appendLog(QStringLiteral("AID %1 sw=%2").arg(aidHex).arg(sw(r)));
        if (r.size() < 2 || (quint8)r.at(r.size()-2) != 0x90) continue;

        // Application label (50) or preferred name (9F12); else the known name.
        QByteArray lbl = tlvFirst(r, 0x50);
        if (lbl.isEmpty()) lbl = tlvFirst(r, 0x9f12);
        QString name = QString::fromLatin1(lbl);
        if (name.trimmed().isEmpty()) {
            name.clear();
            for (int k = 0; k < nknown; k++)
                if (aidHex == QString(known[k].aid)) { name = QString::fromLatin1(known[k].name); break; }
        }
        addField(tr("EMV application"),
            (name.isEmpty() ? aidHex : name + QStringLiteral("  ·  ") + aidHex));
        if (typeLabel.isEmpty()) typeLabel = tr("Payment card (EMV)");
        payAids << aid;
    }

    // Try the on-card transaction log (best effort). International schemes
    // (Visa A0000000031010, Mastercard A0000000041010) usually release it after
    // GPO; girocard (ZKA) gates it. Try those first, stop at the first readable
    // log, and note once if a log exists but stays protected.
    {
        QList<QByteArray> order;
        for (int i = 0; i < payAids.size(); i++) {
            const QString h = QString::fromLatin1(payAids.at(i).toHex()).toUpper();
            if (h == QLatin1String("A0000000041010") || h == QLatin1String("A0000000031010"))
                order.prepend(payAids.at(i));
            else
                order.append(payAids.at(i));
        }
        bool gated = false;
        iLogAid.clear();
        for (int i = 0; i < order.size(); i++) {
            const int n = readEmvLog(order.at(i));
            if (n > 0) { gated = false; break; }
            if (n == 0) { gated = true; if (iLogAid.isEmpty()) iLogAid = order.at(i); }
        }
        if (gated)
            addField(tr("Transaction log"),
                tr("present, protected — tap to try an offline PIN"),
                false, 1, QStringLiteral("pin"));
    }

    return docType.isEmpty() ? typeLabel : docType;
}

// EMV value formatters for transaction-log fields.
static QString emvCurrency(const QByteArray& c)
{
    if (c.size() < 2) return QString();
    const int code = ((quint8)c.at(0) << 8) | (quint8)c.at(1);
    switch (code) {
        case 0x0978: return QStringLiteral("EUR");
        case 0x0840: return QStringLiteral("USD");
        case 0x0826: return QStringLiteral("GBP");
        case 0x0756: return QStringLiteral("CHF");
        default:      return QString::asprintf("%04X", code);
    }
}
static QString emvAmount(const QByteArray& a)
{
    if (a.isEmpty()) return QString();
    bool ok = false;
    const qulonglong v = QString::fromLatin1(a.toHex()).toULongLong(&ok);   // n12 BCD
    return ok ? QString::number(v / 100.0, 'f', 2) : QString();
}
static QString emvDate(const QByteArray& d)
{
    if (d.size() < 3) return QString();
    const QString h = QString::fromLatin1(d.toHex());   // YYMMDD
    return QStringLiteral("20") + h.mid(0, 2) + "-" + h.mid(2, 2) + "-" + h.mid(4, 2);
}

// Plaintext offline PIN block (ISO-9564 format 2): 0x2<len>, PIN digits packed
// BCD, padded with 0xF nibbles to 8 bytes.
static QByteArray emvPinBlock(const QByteArray& pin)
{
    QByteArray b(8, (char)0xFF);
    const int n = qMin(pin.size(), 12);
    b[0] = (char)(0x20 | (n & 0x0f));
    for (int i = 0; i < n; i++) {
        const int digit = (quint8)pin.at(i) - '0';
        if (digit < 0 || digit > 9) continue;
        const int pos = 1 + i / 2;
        if ((i & 1) == 0) b[pos] = (char)((digit << 4) | 0x0f);
        else              b[pos] = (char)(((quint8)b.at(pos) & 0xf0) | digit);
    }
    return b;
}

int Proxmark::readEmvLog(const QByteArray& aid, const QByteArray& pin)
{
    // Held session: one activation across SELECT -> (VERIFY) -> GPO -> READ RECORD.
    QByteArray sel;
    sel.append((char)0x00).append((char)0xA4).append((char)0x04).append((char)0x00);
    sel.append((char)aid.size()).append(aid).append((char)0x00);
    QByteArray fci = apduExchangeEx(sel, /*connect=*/true, /*keepField=*/true);
    if (fci.size() < 2 || (quint8)fci.at(fci.size() - 2) != 0x90) {
        apduExchangeEx(sel, false, false);   // release the field
        return -1;
    }

    // Optional offline PIN. Only run VERIFY when the PIN-try counter is >= 2,
    // so a wrong PIN can never take the card to 0 (blocked) from this app.
    if (!pin.isEmpty()) {
        QByteArray gd; gd.append((char)0x80).append((char)0xCA).append((char)0x9F).append((char)0x17).append((char)0x00);
        const QByteArray ptcResp = apduExchangeEx(gd, false, true);
        const QByteArray ptcTlv = tlvFirst(ptcResp, 0x9f17);
        const int ptc = ptcTlv.isEmpty() ? -1 : (quint8)ptcTlv.at(0);
        appendLog(QStringLiteral("PIN tries left: %1").arg(ptc < 0 ? QStringLiteral("unknown") : QString::number(ptc)));
        // Fail-safe: only send VERIFY when the card tells us there are >= 2
        // tries. Unknown counter (card hides 9F17) -> do NOT spend a try blindly.
        if (ptc < 2) {
            addField(tr("PIN"), ptc < 0
                ? tr("not tried — card hides its PIN-try counter, not risking a try")
                : tr("not tried — only %n try(ies) left, would risk blocking", "", ptc),
                false, 2);
            apduExchangeEx(sel, false, false);
            return 0;
        }
        // VERIFY, plaintext PIN (P2=80). The command carries the PIN -> never log it.
        const QByteArray blk = emvPinBlock(pin);
        QByteArray verify;
        verify.append((char)0x00).append((char)0x20).append((char)0x00).append((char)0x80)
              .append((char)blk.size()).append(blk);
        const QByteArray vr = apduExchangeEx(verify, false, true);
        const QString vsw = sw(vr);
        appendLog(QStringLiteral("VERIFY sw=%1").arg(vsw));
        if (vsw != QLatin1String("9000")) {
            // 63Cx = wrong PIN, x tries left; 6983/6984 = blocked/invalid.
            QString msg;
            int lvl = 2;
            if (vsw.startsWith(QStringLiteral("63C"))) {
                msg = tr("wrong PIN · %1 tries left").arg(vsw.right(1).toInt(Q_NULLPTR, 16));
            } else if (vsw == QLatin1String("6983") || vsw == QLatin1String("6984")) {
                msg = tr("PIN blocked / not usable");
            } else {
                // 6985 (conditions), 6A88, 6D00, 6E00, 6700 ... the card refuses
                // offline PIN here WITHOUT counting it as a wrong attempt.
                msg = tr("card refuses offline PIN here (SW %1) — no try spent").arg(vsw);
                lvl = 1;
            }
            addField(tr("PIN"), msg, false, lvl);
            apduExchangeEx(sel, false, false);
            return 0;
        }
        addField(tr("PIN"), tr("verified ✓"), false, 0);
    }

    // Log Entry (9F4D) = [SFI, record count]. Usually in the FCI, else GET DATA.
    QByteArray le = tlvFirst(fci, 0x9f4d);
    if (le.size() < 2) {
        QByteArray gd; gd.append((char)0x80).append((char)0xCA).append((char)0x9F).append((char)0x4D).append((char)0x00);
        QByteArray r = apduExchangeEx(gd, false, true);
        appendLog(QStringLiteral("GET 9F4D sw=%1").arg(sw(r)));
        le = tlvFirst(r, 0x9f4d);
        if (le.size() < 2 && r.size() >= 4 && (quint8)r.at(r.size() - 2) == 0x90)
            le = r.left(r.size() - 2);
    }
    if (le.size() < 2) {
        appendLog(QStringLiteral("no on-card transaction log (9F4D) on %1")
            .arg(QString::fromLatin1(aid.toHex()).toUpper()));
        apduExchangeEx(sel, false, false);
        return -1;
    }
    const int sfi = (quint8)le.at(0);
    const int count = (quint8)le.at(1);
    if (count <= 0) { apduExchangeEx(sel, false, false); return -1; }
    appendLog(QStringLiteral("LOG aid=%1 sfi=%2 cnt=%3")
        .arg(QString::fromLatin1(aid.toHex()).toUpper()).arg(sfi).arg(count));

    // Log Format (9F4F): a DOL (tag+length pairs) describing each log record.
    QList<quint32> dolTag; QList<int> dolLen;
    {
        QByteArray gd; gd.append((char)0x80).append((char)0xCA).append((char)0x9F).append((char)0x4F).append((char)0x00);
        QByteArray r = apduExchangeEx(gd, false, true);
        QByteArray fmt = tlvFirst(r, 0x9f4f);
        appendLog(QStringLiteral("9F4F sw=%1 %2").arg(sw(r))
            .arg(QString::fromLatin1(fmt.toHex()).toUpper()));
        int i = 0;
        while (i < fmt.size()) {
            quint32 t = (quint8)fmt.at(i++);
            if ((t & 0x1f) == 0x1f) { if (i >= fmt.size()) break; t = (t << 8) | (quint8)fmt.at(i++); }
            if (i >= fmt.size()) break;
            dolTag << t; dolLen << (quint8)fmt.at(i++);
        }
    }

    // READ RECORD on the log file needs a transaction context: without a prior
    // GET PROCESSING OPTIONS the card answers 6982. Fire a GPO with the PDOL
    // (9F38 from the FCI) filled with neutral defaults - enough to unlock the
    // log on cards that gate it, harmless otherwise.
    {
        const QByteArray pdol = tlvFirst(fci, 0x9f38);
        QByteArray pd;
        int i = 0;
        while (i < pdol.size()) {
            quint32 t = (quint8)pdol.at(i++);
            if ((t & 0x1f) == 0x1f) { if (i >= pdol.size()) break; t = (t << 8) | (quint8)pdol.at(i++); }
            if (i >= pdol.size()) break;
            const int l = (quint8)pdol.at(i++);
            QByteArray v(l, (char)0x00);
            if (t == 0x9f37)                { for (int z = 0; z < l; z++) v[z] = (char)(0x11 * (z + 1)); } // UN
            else if (t == 0x5f2a && l == 2) { v[0] = 0x09; v[1] = 0x78; }   // currency EUR
            else if (t == 0x9f1a && l == 2) { v[0] = 0x02; v[1] = 0x76; }   // country DE
            else if (t == 0x9a   && l == 3) { v[0] = 0x26; v[1] = 0x01; v[2] = 0x01; } // date
            else if (t == 0x9f66 && l >= 1) { v[0] = 0x36; }               // Visa TTQ
            else if (t == 0x9f35 && l == 1) { v[0] = 0x22; }               // terminal type
            pd.append(v);
        }
        QByteArray tmpl; tmpl.append((char)0x83).append((char)pd.size()).append(pd);
        QByteArray gpo;
        gpo.append((char)0x80).append((char)0xA8).append((char)0x00).append((char)0x00)
           .append((char)tmpl.size()).append(tmpl).append((char)0x00);
        const QByteArray gr = apduExchangeEx(gpo, false, true);
        appendLog(QStringLiteral("GPO sw=%1").arg(sw(gr)));
    }

    const int cap = count < 15 ? count : 15;
    int shown = 0;
    for (int rec = 1; rec <= cap; rec++) {
        const bool last = (rec == cap);
        QByteArray rr;
        rr.append((char)0x00).append((char)0xB2).append((char)rec)
          .append((char)((sfi << 3) | 0x04)).append((char)0x00);
        QByteArray r = apduExchangeEx(rr, false, /*keepField=*/!last);
        appendLog(QStringLiteral("REC%1 sw=%2 %3").arg(rec).arg(sw(r))
            .arg(r.size() > 2 ? QString::fromLatin1(r.left(r.size() - 2).toHex()).toUpper() : QString()));
        if (r.size() < 2 || (quint8)r.at(r.size() - 2) != 0x90) continue;
        const QByteArray body = r.left(r.size() - 2);   // strip SW

        // The record is the DOL values concatenated, in order, no TLV tags.
        QByteArray amount, date, cur; long atc = -1;
        int off = 0;
        for (int k = 0; k < dolTag.size(); k++) {
            const int l = dolLen.at(k);
            if (off + l > body.size()) break;
            const QByteArray v = body.mid(off, l); off += l;
            switch (dolTag.at(k)) {
                case 0x9f02: amount = v; break;
                case 0x9a:   date = v; break;
                case 0x5f2a: cur = v; break;
                case 0x9f36: { atc = 0; for (int z = 0; z < v.size(); z++) atc = (atc << 8) | (quint8)v.at(z); } break;
                default: break;
            }
        }
        QStringList parts;
        const QString amt = emvAmount(amount);
        if (!amt.isEmpty()) {
            const QString c = emvCurrency(cur);
            parts << (amt + (c.isEmpty() ? QString() : QStringLiteral(" ") + c));
        }
        const QString dt = emvDate(date);
        if (!dt.isEmpty()) parts << dt;
        if (atc >= 0) parts << QStringLiteral("ATC %1").arg(atc);
        if (parts.isEmpty()) continue;
        addField(tr("Transaction %1").arg(++shown), parts.join(QStringLiteral("  ·  ")), true);
    }
    return shown;   // >0 = listed, 0 = log present but records stayed locked
}

void Proxmark::readLogWithPin(const QString& pin)
{
    if (iFd < 0 || iLogAid.isEmpty()) return;
    setBusy(true);
    setStatus(tr("Trying offline PIN..."));
    QByteArray pinb = pin.toLatin1();
    readEmvLog(iLogAid, pinb);
    pinb.fill('\0'); pinb.clear();   // scrub the digits from memory
    emit hfChanged();
    pushState();
    setStatus(tr("PIN attempt done"));
    setBusy(false);
}

// -------------------------------------------------------------------------
// Raw terminal
// -------------------------------------------------------------------------

// Human-readable meaning of a status word (ISO 7816 + DESFire 91xx).
static QString swMeaning(const QString& w)
{
    if (w == QLatin1String("9000") || w == QLatin1String("9100")) return QStringLiteral("OK");
    if (w == QLatin1String("91AF")) return QStringLiteral("mehr Frames — 'weiter (AF)'");
    if (w == QLatin1String("6A82")) return QStringLiteral("nicht gefunden");
    if (w == QLatin1String("6982") || w == QLatin1String("91AE")) return QStringLiteral("Auth nötig / Fehler");
    if (w == QLatin1String("6985")) return QStringLiteral("Bedingungen nicht erfüllt");
    if (w == QLatin1String("911C")) return QStringLiteral("illegal command");
    if (w == QLatin1String("919D")) return QStringLiteral("permission denied");
    if (w == QLatin1String("91BE")) return QStringLiteral("boundary error");
    if (w == QLatin1String("917E")) return QStringLiteral("length error");
    if (w.startsWith(QLatin1String("61"))) return QStringLiteral("Daten verfügbar");
    if (w.startsWith(QLatin1String("63C"))) return QStringLiteral("falsch, Versuche: ") + w.right(1);
    return QString();
}

// Human-readable name of a C-APDU (best effort), for the ">>" log line.
static QString apduName(const QByteArray& a)
{
    if (a.size() < 2) return QString();
    const quint8 cla = (quint8)a.at(0), ins = (quint8)a.at(1);
    if (cla == 0x90) {
        switch (ins) {
            case 0x60: return QStringLiteral("DESFire GetVersion");
            case 0xAF: return QStringLiteral("DESFire weiter (AF)");
            case 0x6A: return QStringLiteral("DESFire GetApplicationIDs");
            case 0x6F: return QStringLiteral("DESFire GetFileIDs");
            case 0xF5: return QStringLiteral("DESFire GetFileSettings");
            case 0x6E: return QStringLiteral("DESFire GetFreeMemory");
            case 0x45: return QStringLiteral("DESFire GetKeySettings");
            case 0x5A: return QStringLiteral("DESFire SelectApplication");
            case 0xBD: return QStringLiteral("DESFire ReadData");
            case 0x6C: return QStringLiteral("DESFire GetValue");
            case 0x64: return QStringLiteral("DESFire GetKeyVersion");
            default: break;
        }
    }
    if (cla == 0x00 && ins == 0xA4) {
        if (a.contains("2PAY.SYS.DDF01")) return QStringLiteral("SELECT PPSE");
        if (a.contains("1PAY.SYS.DDF01")) return QStringLiteral("SELECT PSE");
        return QStringLiteral("ISO SELECT");
    }
    if (cla == 0x80 && ins == 0xCA) return QStringLiteral("GET DATA");
    if (cla == 0x80 && ins == 0xA8) return QStringLiteral("GET PROCESSING OPTIONS");
    if (cla == 0x00 && ins == 0xB2) return QStringLiteral("READ RECORD");
    if (cla == 0x00 && ins == 0x20) return QStringLiteral("VERIFY (PIN)");
    return QString();
}

// Human-readable name of a wire command id.
static QString cmdName(quint16 c)
{
    switch (c) {
        case 0x0107: return QStringLiteral("CMD_VERSION");
        case 0x0109: return QStringLiteral("CMD_PING");
        case 0x0112: return QStringLiteral("CMD_CAPABILITIES");
        case 0x0385: return QStringLiteral("CMD_HF_ISO14443A_READER");
        case 0x0305: return QStringLiteral("CMD_HF_ISO14443B_COMMAND");
        case 0x0620: return QStringLiteral("CMD_HF_MIFARE_READBL");
        case 0x0623: return QStringLiteral("CMD_HF_MIFARE_CHKKEYS");
        default: return QString();
    }
}

void Proxmark::rawApdu(const QString& apduHex, bool hold)
{
    if (iFd < 0) return;
    const QByteArray apdu = QByteArray::fromHex(
        apduHex.simplified().remove(QLatin1Char(' ')).toLatin1());
    if (apdu.isEmpty()) { appendLog(QStringLiteral("! empty / invalid APDU")); return; }
    setBusy(true);
    const QString nm = apduName(apdu);
    appendLog(QStringLiteral(">> %1%2").arg(QString::fromLatin1(apdu.toHex()).toUpper())
        .arg(nm.isEmpty() ? QString() : QStringLiteral("   (") + nm + QStringLiteral(")")));
    // hold: keep one activation across calls (connect only the first time).
    const bool connect = hold ? !iTermHeld : true;
    const QByteArray r = apduExchangeEx(apdu, connect, /*keepField=*/hold);
    iTermHeld = hold;
    if (r.isEmpty()) { appendLog(QStringLiteral("<< (no response)")); setBusy(false); return; }

    const QString w = sw(r);
    const QString m = swMeaning(w);
    appendLog(QString(QStringLiteral("<< %1  sw=%2%3"))
        .arg(QString::fromLatin1(r.toHex()).toUpper()).arg(w)
        .arg(m.isEmpty() ? QString() : QStringLiteral("  (") + m + QStringLiteral(")")));

    // Human-readable decode of the common DESFire structure commands.
    if (apdu.size() >= 2 && (quint8)apdu.at(0) == 0x90) {
        const quint8 ins = (quint8)apdu.at(1);
        QByteArray body = r; if (body.size() >= 2) body.chop(2);   // drop 91xx status
        if (ins == 0x6A) {                      // GetApplicationIDs
            if (body.isEmpty()) appendLog(QStringLiteral("   (keine Applikationen)"));
            for (int i = 0; i + 3 <= body.size(); i += 3)
                appendLog(QStringLiteral("   App-AID %1").arg(QString::fromLatin1(body.mid(i, 3).toHex()).toUpper()));
        } else if (ins == 0x6F) {               // GetFileIDs
            appendLog(body.isEmpty() ? QStringLiteral("   (keine Dateien)")
                : QStringLiteral("   Datei-IDs: %1").arg(QString::fromLatin1(body.toHex()).toUpper()));
        } else if (ins == 0x6E && body.size() >= 3) {   // GetFreeMemory
            const long fr = (quint8)body.at(0) | ((quint8)body.at(1) << 8) | ((quint8)body.at(2) << 16);
            appendLog(QStringLiteral("   frei: %1 Byte").arg(fr));
        } else if (ins == 0x60 && body.size() >= 7) {   // GetVersion (HW frame)
            appendLog(QStringLiteral("   HW vendor=%1 typ=%2 ~%3 Byte Speicher (→ 'weiter (AF)')")
                .arg((quint8)body.at(0)).arg((quint8)body.at(2))
                .arg(1 << ((quint8)body.at(5) >> 1)));
        }
    }
    setBusy(false);
}

void Proxmark::rawCmd(const QString& cmdHex, const QString& dataHex, bool ng)
{
    if (iFd < 0) return;
    bool okc = false;
    const quint16 cmd = cmdHex.simplified().remove(QLatin1Char(' '))
        .remove(QStringLiteral("0x")).toUShort(&okc, 16);
    if (!okc) { appendLog(QStringLiteral("! bad command id")); return; }
    const QByteArray data = QByteArray::fromHex(
        dataHex.simplified().remove(QLatin1Char(' ')).toLatin1());
    setBusy(true);
    QString sendLine = QString(QStringLiteral(">> CMD %1 %2 [%3]"))
        .arg(cmd, 4, 16, QChar('0')).arg(ng ? QStringLiteral("NG") : QStringLiteral("MIX"))
        .arg(QString::fromLatin1(data.toHex()).toUpper()).toUpper();
    const QString cnm = cmdName(cmd);
    if (!cnm.isEmpty()) sendLine += QStringLiteral("   (") + cnm + QStringLiteral(")");
    appendLog(sendLine);
    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(cmd, data, ng)) { appendLog(QStringLiteral("! send failed")); setBusy(false); return; }
    quint16 rcmd = 0; qint8 st = 0; QByteArray r;
    bool got = false;
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &r, 2500)) break;
        if (rcmd == CMD_DEBUG_PRINT) { appendLog(QStringLiteral("PM5: ") + QString::fromLatin1(r)); continue; }
        got = true; break;
    }
    if (!got) appendLog(QStringLiteral("<< (no reply)"));
    else appendLog(QString(QStringLiteral("<< cmd=%1 st=%2 %3"))
        .arg(rcmd, 4, 16, QChar('0')).arg((int)st)
        .arg(QString::fromLatin1(r.toHex()).toUpper()).toUpper());
    setBusy(false);
}

// -------------------------------------------------------------------------
// UID emulation
// -------------------------------------------------------------------------

void Proxmark::simulateUid(const QString& uidHex)
{
    if (iFd < 0) return;
    const QByteArray uid = QByteArray::fromHex(
        uidHex.simplified().remove(QLatin1Char(' ')).remove(QLatin1Char(':')).toLatin1());
    if (uid.size() != 4 && uid.size() != 7 && uid.size() != 10) {
        appendLog(QStringLiteral("! UID must be 4, 7 or 10 bytes"));
        setStatus(tr("UID must be 4, 7 or 10 bytes"));
        return;
    }
    // tagtype: 3 = DESFire (7B), 4 = ISO 14443-4 (10B), 1 = MIFARE Classic (4B).
    const quint8 tagtype = uid.size() == 7 ? 3 : (uid.size() == 10 ? 4 : 1);
    const quint16 flags = uid.size() == 7 ? 0x0020 : (uid.size() == 10 ? 0x0030 : 0x0010);

    // struct p (PACKED, 69 B): tagtype, flags(u16), uid[10], exitAfter,
    // rats[20], ulauth_1a1_len, ulauth_1a2_len, ulauth_1a1[16], ulauth_1a2[16],
    // ulauth_1a2_mirror. Everything but tagtype/flags/uid stays zero.
    QByteArray p(69, (char)0);
    p[0] = (char)tagtype;
    p[1] = (char)(flags & 0xff);
    p[2] = (char)((flags >> 8) & 0xff);
    for (int i = 0; i < uid.size(); i++) p[3 + i] = uid.at(i);

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_ISO14443A_SIMULATE, p, /*ng=*/true)) {
        setStatus(tr("Could not start emulation"));
        return;
    }
    // The device now simulates and only replies once the loop ends (button or
    // CMD_BREAK_LOOP); do not block waiting here.
    appendLog(QStringLiteral("EMU start · UID %1 · tagtype %2")
        .arg(QString::fromLatin1(uid.toHex()).toUpper()).arg(tagtype));
    setStatus(tr("Emulating %1 — hold the PM5 to the reader, then Stop")
        .arg(QString::fromLatin1(uid.toHex()).toUpper()));
}

void Proxmark::stopEmulation()
{
    if (iFd < 0) return;
    sendCommand(CMD_BREAK_LOOP, QByteArray(), /*ng=*/true);
    quint16 rcmd = 0; qint8 st = 0; QByteArray data;
    for (int i = 0; i < 6; i++) { if (!readResponse(&rcmd, &st, &data, 400)) break; }
    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    appendLog(QStringLiteral("EMU stop"));
    setStatus(tr("Emulation stopped"));
}

// --------------------------------------------------------------------------

static QString atqaDecode(quint8 a0, quint8 a1)
{
    const int uidbits = (a0 >> 6) & 0x03;   // 00=single 01=double 10=triple
    const char* sz = uidbits == 0 ? "single UID (4 byte)"
                   : uidbits == 1 ? "double UID (7 byte)"
                   : uidbits == 2 ? "triple UID (10 byte)" : "reserved";
    return QString("bit-frame anticollision %1 · %2")
        .arg((int)(a0 & 0x1f), 0, 2).arg(QString::fromLatin1(sz));
}

static QString atsDecode(const QByteArray& ats)
{
    if (ats.isEmpty()) return QString();
    QStringList out;
    const quint8 t0 = (quint8)ats.at(0);
    const int fsci = t0 & 0x0f;
    static const int fsc[] = {16,24,32,40,48,64,96,128,256};
    out << QString("T0=%1 · FSCI %2 → max frame %3 byte")
        .arg((int)t0, 2, 16, QChar('0')).toUpper()
        .arg(fsci).arg(fsci <= 8 ? fsc[fsci] : 256);
    int idx = 1;
    if (t0 & 0x10) { if (idx < ats.size()) out << QString("TA(1)=%1 (bit rates)").arg((quint8)ats.at(idx),2,16,QChar('0')).toUpper(); idx++; }
    if (t0 & 0x20) { if (idx < ats.size()) { quint8 tb=(quint8)ats.at(idx); out << QString("TB(1)=%1 · FWI %2 · SFGI %3").arg(tb,2,16,QChar('0')).arg(tb>>4).arg(tb&0x0f).toUpper(); } idx++; }
    if (t0 & 0x40) { if (idx < ats.size()) out << QString("TC(1)=%1 (CID/NAD)").arg((quint8)ats.at(idx),2,16,QChar('0')).toUpper(); idx++; }
    if (idx < ats.size()) {
        QByteArray hist = ats.mid(idx);
        out << QString("historical bytes: %1").arg(QString::fromLatin1(hist.toHex()).toUpper());
    }
    return out.join(QStringLiteral("\n"));
}

// ISO 14443-B: Perso/eID, passport, many girocard. Select, report PUPI/ATQB.
bool Proxmark::readHfB()
{
    if (iFd < 0) return false;
    // iso14b_raw_cmd_t { uint16_t flags; uint32_t timeout; uint8_t rawlen; }
    const quint16 flags = ISO14B_CONNECT | ISO14B_SELECT_STD | ISO14B_CLEARTRACE;
    QByteArray p;
    p.append((char)(flags & 0xff));
    p.append((char)((flags >> 8) & 0xff));
    for (int i = 0; i < 4; i++) p.append((char)0);   // timeout
    p.append((char)0);                               // rawlen

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    if (!sendCommand(CMD_HF_ISO14443B_COMMAND, p, /*ng=*/true)) return false;

    quint16 rcmd = 0; qint8 st = 0; QByteArray data;
    bool got = false;
    for (int i = 0; i < 6; i++) {
        if (!readResponse(&rcmd, &st, &data, 2500)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == CMD_HF_ISO14443B_COMMAND) { got = true; break; }
    }
    if (!got || st != 0) return false;   // no Type B card

    // iso14b_card_select_t { uid[10]; uidlen; atqb[7]; chipid; cid; }
    if (data.size() < 20) return false;
    const int uidlen = qBound(0, (int)(quint8)data.at(10), 10);
    QByteArray pupi = data.mid(0, uidlen);
    QByteArray atqb = data.mid(11, 7);

    iHfUid = hexColon(pupi);
    iHfType = tr("ISO 14443-B");

    addField(tr("PUPI / NFCID0"), iHfUid, true);
    addField(tr("ATQB"), QString::fromLatin1(atqb.toHex()).toUpper(), true);
    addField(tr("Application data"), QString::fromLatin1(atqb.left(4).toHex()).toUpper());
    addField(tr("Protocol info"), QString::fromLatin1(atqb.mid(4, 3).toHex()).toUpper());
    addField(tr("Card type"), tr("German eID, passport, many girocard"));

    iHfDetails = tr("Type B · PUPI %1").arg(iHfUid);
    emit hfChanged();
    setStatus(tr("ISO 14443-B card · PUPI %1").arg(iHfUid));
    return true;
}

void Proxmark::readHf()
{
    if (iFd < 0) return;
    setBusy(true);
    resetLog();   // each read starts a fresh, timestamped transcript
    iHfUid.clear(); iHfType.clear(); iHfDetails.clear(); iHfFields.clear(); iLogAid.clear();
    emit hfChanged();

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    setStatus(tr("Reading 13.56 MHz - hold the card on the HF antenna..."));

    const bool ng = fwUsesNg14a();
    const quint64 flags = ISO14A_CONNECT | ISO14A_CLEARTRACE;
    QByteArray p;
    if (ng) {
        // iso14a_raw_cmd_t: u32 flags, u32 timeout, u32 wait_us,
        //                   u16 len, u16 lenbits (all trailing fields 0 here).
        for (int i = 0; i < 4; i++) p.append((char)((flags >> (8 * i)) & 0xff));
        for (int i = 0; i < 12; i++) p.append((char)0);
    } else {
        // MIX: 3x u64 oldargs (flags, 0, 0), no data.
        for (int i = 0; i < 8; i++) p.append((char)((flags >> (8 * i)) & 0xff));
        for (int i = 0; i < 16; i++) p.append((char)0);
    }
    if (!sendCommand(CMD_HF_ISO14443A_READER, p, ng)) {
        setStatus(tr("Could not start HF read"));
        setBusy(false);
        return;
    }

    const quint16 wantCmd = ng ? (quint16)CMD_HF_ISO14443A_READER : (quint16)CMD_ACK;
    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    bool got = false;
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &data, 2500)) break;
        if (rcmd == CMD_DEBUG_PRINT) continue;
        if (rcmd == wantCmd) { got = true; break; }
    }
    // NG replies with iso14a_raw_resp_t (len u16, sel u8, rfu u8, data[]), so
    // the card struct sits at offset 4 and the select status is byte 2. MIX
    // prefixes 3x u64 oldargs, so the struct is at 24 and the status at byte 0.
    const int base = ng ? 4 : 24;
    const int selOff = ng ? 2 : 0;
    if (!got || data.size() < base + 15) {
        setStatus(tr("No Type A card - trying Type B..."));
        if (!readHfB()) setStatus(tr("No card in the HF field"));
        setBusy(false);
        return;
    }
    const quint8 selectStatus = (quint8)data.at(selOff);
    if (selectStatus == 0) {
        setStatus(tr("No Type A card - trying Type B..."));
        if (!readHfB()) setStatus(tr("No 13.56 MHz card found"));
        setBusy(false);
        return;
    }
    const int uidlen = qBound(0, (int)(quint8)data.at(base + 10), 10);
    QByteArray uid = data.mid(base, uidlen);
    const quint8 atqa0 = (quint8)data.at(base + 11);
    const quint8 atqa1 = (quint8)data.at(base + 12);
    const quint8 sak   = (quint8)data.at(base + 13);
    const int atsLen = qBound(0, (int)(quint8)data.at(base + 14), data.size() - (base + 15));
    QByteArray ats = data.mid(base + 15, atsLen);

    bool isClassic = false;
    const QString type = sakType(sak, &isClassic);
    const bool randomUid = (uidlen == 4 && (quint8)uid.at(0) == 0x08);

    iHfUid = hexColon(uid);
    iHfType = type;

    appendLog(QStringLiteral("HF-A UID %1 ATQA %2%3 SAK %4%5")
        .arg(iHfUid)
        .arg(atqa1, 2, 16, QChar('0')).arg(atqa0, 2, 16, QChar('0'))
        .arg(sak, 2, 16, QChar('0'))
        .arg(ats.isEmpty() ? QString() : QStringLiteral(" ATS ") + QString::fromLatin1(ats.toHex()).toUpper())
        .toUpper());

    // Structured, explained field list (the detail page renders these).
    addField(tr("UID / NFCID1"),
        iHfUid + (randomUid ? QStringLiteral("   (random, regenerated each session)") : QString()), true);
    addField(tr("UID length"), tr("%n byte(s)", "", uidlen));
    addField(tr("ATQA"),
        QString("%1%2").arg(atqa1,2,16,QChar('0')).arg(atqa0,2,16,QChar('0')).toUpper()
        + QStringLiteral("   ") + atqaDecode(atqa0, atqa1));
    addField(tr("SAK / SEL_RES"),
        QString("%1").arg(sak,2,16,QChar('0')).toUpper()
        + QStringLiteral("   ") + type + (isClassic ? tr("  · Crypto1") : QString()));
    if (!ats.isEmpty()) {
        addField(tr("ATS"), QString::fromLatin1(ats.toHex()).toUpper(), true);
        addField(tr("ATS decoded"), atsDecode(ats));
    }

    iHfDetails = tr("ATQA %1%2 · SAK %3")
        .arg(atqa1,2,16,QChar('0')).arg(atqa0,2,16,QChar('0'))
        .arg(sak,2,16,QChar('0')).toUpper();

    // ISO 14443-4 -> try to name the smart-card application (EMV).
    if (sak & 0x20) {
        setStatus(tr("ISO 14443-4 - probing applications..."));
        const QString emvType = emvDiscover();
        if (!emvType.isEmpty()) iHfType = emvType;
        else if (randomUid)
            addField(tr("Card class"),
                tr("eID / passport / girocard (content needs PACE/keys)"), false, 1);
    }

    emit hfChanged();
    setStatus(tr("%1 · UID %2").arg(iHfType).arg(iHfUid));
    setBusy(false);
}

void Proxmark::writeEm4100(const QString& hexId)
{
    if (iFd < 0) return;
    setBusy(true);

    bool ok = false;
    const quint64 id = hexId.trimmed().toULongLong(&ok, 16);
    if (!ok) {
        setStatus(tr("Not a valid hex id: %1").arg(hexId));
        setBusy(false);
        return;
    }

    // CMD_LF_EM410X_CLONE payload (pm3_cmd.h, PACKED):
    //   bool Q5; bool EM; bool add_electra; uint8_t clock; u32 high; u32 low
    // Q5=0, EM=0 -> a plain T5577. clock 64 = the EM4100 RF/64 standard.
    QByteArray p;
    p.append((char)0);            // Q5
    p.append((char)0);            // EM
    p.append((char)0);            // add_electra
    p.append((char)64);           // clock
    const quint32 high = (quint32)((id >> 32) & 0xffffffff);
    const quint32 low  = (quint32)(id & 0xffffffff);
    for (int i = 0; i < 4; i++) p.append((char)((high >> (8 * i)) & 0xff));
    for (int i = 0; i < 4; i++) p.append((char)((low  >> (8 * i)) & 0xff));

    tcflush(iFd, TCIFLUSH);
    iRx.clear();
    setStatus(tr("Writing %1 to T5577 - hold the tag on the LF antenna...").arg(hexId.toUpper()));

    if (!sendCommand(CMD_LF_EM410X_CLONE, p)) {
        setStatus(tr("Could not start the write"));
        setBusy(false);
        return;
    }

    quint16 rcmd = 0;
    qint8 st = 0;
    QByteArray data;
    bool done = false, success = false;
    for (int i = 0; i < 8; i++) {
        if (!readResponse(&rcmd, &st, &data, 3000)) break;
        if (rcmd == CMD_DEBUG_PRINT) {
            appendLog(QStringLiteral("PM5: ") + QString::fromLatin1(data));
            continue;
        }
        if (rcmd == CMD_LF_EM410X_CLONE) {
            done = true;
            success = (st == 0);   // PM3_SUCCESS
            break;
        }
    }

    if (success) {
        setStatus(tr("Written: T5577 now answers as EM4100 %1").arg(hexId.toUpper()));
    } else if (done) {
        setStatus(tr("Write rejected by the device (no tag, or not a T5577?)"));
    } else {
        setStatus(tr("No reply to the write - was a tag on the antenna?"));
    }
    setBusy(false);
}

void Proxmark::readLf()
{
    if (iFd < 0) return;
    setBusy(true);
    iLastTag.clear();
    iLastTagType.clear();
    emit lastTagChanged();
    setStatus(tr("Searching 125 kHz - hold the tag on the LF antenna..."));

    struct Probe { quint16 cmd; const char* type; const char* marker; bool hex; };
    static const Probe probes[] = {
        { CMD_LF_EM410X_WATCH, "EM4100",   "EM TAG ID:", true  },
        { CMD_LF_HID_WATCH,    "HID Prox", "TAG ID:",    true  },
        { CMD_LF_IO_WATCH,     "IO Prox",  "IO Prox",    false }
    };

    QString id;
    QString type;
    for (int i = 0; i < 3; i++) {
        setStatus(tr("Trying %1...").arg(QString::fromLatin1(probes[i].type)));
        if (watchProbe(probes[i].cmd, QString::fromLatin1(probes[i].marker),
                probes[i].hex, &id)) {
            type = QString::fromLatin1(probes[i].type);
            break;
        }
    }

    if (!type.isEmpty()) {
        iLastTag = id;
        iLastTagType = type;
        emit lastTagChanged();
        setStatus(tr("%1: %2").arg(type).arg(id));
    } else {
        setStatus(tr("No 125 kHz badge found (blank tag, or an unsupported format)"));
    }
    setBusy(false);
}
