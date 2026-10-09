#include "pmproxy.h"
#include "proxmark.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static QString csvCell(const QString& s)
{
    QString v = s;
    v.replace(QLatin1Char('"'), QLatin1String("\"\""));
    return QLatin1Char('"') + v + QLatin1Char('"');
}

QString PmProxy::exportLog(const QString& format, const QString& target)
{
    const QString fmt = format.toLower();
    const QString ext = (fmt == QLatin1String("csv")) ? QStringLiteral(".csv")
                      : (fmt == QLatin1String("json")) ? QStringLiteral(".json")
                      : QStringLiteral(".txt");
    QString path = target;
    path.remove(QStringLiteral("file://"));   // folder pickers hand back a URL
    if (path.isEmpty()) {
        QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        if (dir.isEmpty()) dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        if (dir.isEmpty()) return QString();
        QDir().mkpath(dir);
        const QString ts = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-HHmmss"));
        path = dir + QStringLiteral("/pm5-") + ts + ext;
    } else {
        QFileInfo fi(path);
        if (!fi.dir().exists()) QDir().mkpath(fi.path());
        if (fi.suffix().isEmpty()) path += ext;
    }
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return QString();

    // JSON: structured + re-usable later (card data + fields + full transcript).
    if (fmt == QLatin1String("json")) {
        QJsonObject root;
        root["app"] = QStringLiteral("harbour-pm5");
        root["exported"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        root["version"] = m_version;
        root["type"] = m_hfType;
        QJsonArray fields;
        for (int i = 0; i < m_hfFields.size(); i++) {
            const QVariantMap m = m_hfFields.at(i).toMap();
            QJsonObject o;
            o["label"] = m.value("label").toString();
            o["value"] = m.value("value").toString();
            o["level"] = m.value("level").toInt();
            fields.append(o);
        }
        root["fields"] = fields;
        QJsonArray log;
        for (int i = 0; i < m_log.size(); i++) log.append(m_log.at(i));
        root["log"] = log;
        f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
        f.close();
        return path;
    }

    QTextStream out(&f);
    out.setCodec("UTF-8");

    if (fmt == QLatin1String("csv")) {
        out << "section,label,value\r\n";
        if (!m_hfType.isEmpty())
            out << csvCell(QStringLiteral("type")) << ',' << csvCell(QString()) << ',' << csvCell(m_hfType) << "\r\n";
        for (int i = 0; i < m_hfFields.size(); i++) {
            const QVariantMap m = m_hfFields.at(i).toMap();
            out << csvCell(QStringLiteral("field")) << ',' << csvCell(m.value("label").toString())
                << ',' << csvCell(m.value("value").toString()) << "\r\n";
        }
        for (int i = 0; i < m_log.size(); i++)
            out << csvCell(QStringLiteral("log")) << ',' << csvCell(QString()) << ',' << csvCell(m_log.at(i)) << "\r\n";
    } else {
        out << "harbour-pm5 - HF scan\n"
            << QDateTime::currentDateTime().toString(Qt::ISODate) << "\n\n";
        if (!m_hfType.isEmpty()) out << m_hfType << "\n\n";
        for (int i = 0; i < m_hfFields.size(); i++) {
            const QVariantMap m = m_hfFields.at(i).toMap();
            out << m.value("label").toString() << ": " << m.value("value").toString() << '\n';
        }
        out << "\n--- Log ---\n";
        for (int i = 0; i < m_log.size(); i++) out << m_log.at(i) << '\n';
    }
    f.close();
    return path;
}

PmProxy::PmProxy(Proxmark* worker, QObject* parent) :
    QObject(parent),
    m_worker(worker),
    m_present(false),
    m_connected(false),
    m_busy(false),
    m_mfFound(0),
    m_mfDumpBlocks(0)
{
    connect(m_worker, &Proxmark::snapshot, this, &PmProxy::onSnapshot);

    // Presence is just a device-node check - safe and cheap on the GUI thread,
    // so poll it here (the worker only pushes state while it is doing work).
    connect(&m_pollTimer, &QTimer::timeout, this, &PmProxy::pollPresent);
    m_pollTimer.start(1000);
    pollPresent();
}

QString PmProxy::portPath() const
{
    return QStringLiteral("/dev/ttyACM0");
}

void PmProxy::pollPresent()
{
    const bool p = QFile::exists(portPath());
    if (p != m_present) {
        m_present = p;
        emit presentChanged();
        // Replugging clears a previous manual Disconnect and re-arms auto-connect.
        if (!p) { m_userDisconnect = false; m_autoConnectTried = false; }
    }
    // Auto-connect once when a device is present and we are not connected,
    // unless the user explicitly disconnected.
    if (m_present && !m_connected && !m_userDisconnect && !m_autoConnectTried) {
        m_autoConnectTried = true;
        connectDevice();
    }
}

// --- action forwarders: queued into the worker thread, never block the GUI ---
void PmProxy::connectDevice()   { m_userDisconnect = false; QMetaObject::invokeMethod(m_worker, "connectDevice", Qt::QueuedConnection); }
void PmProxy::disconnect()      { m_userDisconnect = true; m_autoConnectTried = true; QMetaObject::invokeMethod(m_worker, "disconnect", Qt::QueuedConnection); }
void PmProxy::readVersion()     { QMetaObject::invokeMethod(m_worker, "readVersion", Qt::QueuedConnection); }
void PmProxy::readLf()          { QMetaObject::invokeMethod(m_worker, "readLf", Qt::QueuedConnection); }
void PmProxy::readHf()          { QMetaObject::invokeMethod(m_worker, "readHf", Qt::QueuedConnection); }
void PmProxy::checkMifareKeys() { QMetaObject::invokeMethod(m_worker, "checkMifareKeys", Qt::QueuedConnection); }
void PmProxy::dumpMifare()      { QMetaObject::invokeMethod(m_worker, "dumpMifare", Qt::QueuedConnection); }
void PmProxy::readLogWithPin(const QString& pin)
{
    QMetaObject::invokeMethod(m_worker, "readLogWithPin", Qt::QueuedConnection, Q_ARG(QString, pin));
}
void PmProxy::rawApdu(const QString& apduHex, bool hold)
{
    QMetaObject::invokeMethod(m_worker, "rawApdu", Qt::QueuedConnection,
                              Q_ARG(QString, apduHex), Q_ARG(bool, hold));
}
void PmProxy::rawCmd(const QString& cmdHex, const QString& dataHex, bool ng)
{
    QMetaObject::invokeMethod(m_worker, "rawCmd", Qt::QueuedConnection,
                              Q_ARG(QString, cmdHex), Q_ARG(QString, dataHex), Q_ARG(bool, ng));
}
void PmProxy::simulateUid(const QString& uidHex)
{
    QMetaObject::invokeMethod(m_worker, "simulateUid", Qt::QueuedConnection, Q_ARG(QString, uidHex));
}
void PmProxy::stopEmulation()
{
    QMetaObject::invokeMethod(m_worker, "stopEmulation", Qt::QueuedConnection);
}
void PmProxy::writeEm4100(const QString& hexId)
{
    QMetaObject::invokeMethod(m_worker, "writeEm4100", Qt::QueuedConnection, Q_ARG(QString, hexId));
}

void PmProxy::onSnapshot(const QVariantMap& s)
{
    if (s.contains("connected")    && s["connected"].toBool()    != m_connected)    { m_connected = s["connected"].toBool(); emit connectedChanged(); }
    if (s.contains("busy")         && s["busy"].toBool()         != m_busy)         { m_busy = s["busy"].toBool(); emit busyChanged(); }
    if (s.contains("version")      && s["version"].toString()    != m_version)      { m_version = s["version"].toString(); emit versionChanged(); }
    if (s.contains("model")  && s["model"].toString()  != m_model)  { m_model  = s["model"].toString();  emit versionChanged(); }
    if (s.contains("fwVer")  && s["fwVer"].toString()  != m_fwVer)  { m_fwVer  = s["fwVer"].toString();  emit versionChanged(); }
    if (s.contains("romVer") && s["romVer"].toString() != m_romVer) { m_romVer = s["romVer"].toString(); emit versionChanged(); }
    if (s.contains("status")       && s["status"].toString()     != m_status)       { m_status = s["status"].toString(); emit statusChanged(); }

    bool lf = false;
    if (s.contains("lastTag")     && s["lastTag"].toString()     != m_lastTag)     { m_lastTag = s["lastTag"].toString(); lf = true; }
    if (s.contains("lastTagType") && s["lastTagType"].toString() != m_lastTagType) { m_lastTagType = s["lastTagType"].toString(); lf = true; }
    if (lf) emit lfChanged();

    bool hf = false;
    if (s.contains("hfUid")     && s["hfUid"].toString()     != m_hfUid)     { m_hfUid = s["hfUid"].toString(); hf = true; }
    if (s.contains("hfType")    && s["hfType"].toString()    != m_hfType)    { m_hfType = s["hfType"].toString(); hf = true; }
    if (s.contains("hfDetails") && s["hfDetails"].toString() != m_hfDetails) { m_hfDetails = s["hfDetails"].toString(); hf = true; }
    if (s.contains("hfFields")) { m_hfFields = s["hfFields"].toList(); hf = true; }
    if (hf) emit hfChanged();

    bool mf = false;
    if (s.contains("mfResult") && s["mfResult"].toString() != m_mfResult) { m_mfResult = s["mfResult"].toString(); mf = true; }
    if (s.contains("mfFound")  && s["mfFound"].toInt()      != m_mfFound)  { m_mfFound = s["mfFound"].toInt(); mf = true; }
    if (mf) emit mfChanged();

    bool md = false;
    if (s.contains("mfDump")       && s["mfDump"].toString() != m_mfDump)          { m_mfDump = s["mfDump"].toString(); md = true; }
    if (s.contains("mfDumpBlocks") && s["mfDumpBlocks"].toInt() != m_mfDumpBlocks) { m_mfDumpBlocks = s["mfDumpBlocks"].toInt(); md = true; }
    if (md) emit mfDumpChanged();

    if (s.contains("logLines")) { m_log = s["logLines"].toStringList(); emit logLinesChanged(); }
}
