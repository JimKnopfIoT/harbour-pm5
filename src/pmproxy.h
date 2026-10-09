/*
 * GUI-thread proxy in front of the Proxmark worker.
 *
 * The Proxmark backend does blocking serial I/O (open/PING, LF watch loops,
 * MIFARE key check across all sectors) that would freeze the UI if run on the
 * GUI thread. So Proxmark lives in a worker QThread; this proxy exposes the
 * same QML API on the GUI thread. Actions are forwarded to the worker via
 * queued calls; state comes back as value-copied snapshots (thread-safe).
 */
#ifndef PMPROXY_H
#define PMPROXY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QVariantMap>
#include <QVariantList>

class Proxmark;

class PmProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool present READ present NOTIFY presentChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString portPath READ portPath CONSTANT)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QString model READ model NOTIFY versionChanged)
    Q_PROPERTY(QString fwVer READ fwVer NOTIFY versionChanged)
    Q_PROPERTY(QString romVer READ romVer NOTIFY versionChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString lastTag READ lastTag NOTIFY lfChanged)
    Q_PROPERTY(QString lastTagType READ lastTagType NOTIFY lfChanged)
    Q_PROPERTY(QString hfUid READ hfUid NOTIFY hfChanged)
    Q_PROPERTY(QString hfType READ hfType NOTIFY hfChanged)
    Q_PROPERTY(QString hfDetails READ hfDetails NOTIFY hfChanged)
    Q_PROPERTY(QVariantList hfFields READ hfFields NOTIFY hfChanged)
    Q_PROPERTY(QString mfResult READ mfResult NOTIFY mfChanged)
    Q_PROPERTY(int mfFound READ mfFound NOTIFY mfChanged)
    Q_PROPERTY(QString mfDump READ mfDump NOTIFY mfDumpChanged)
    Q_PROPERTY(int mfDumpBlocks READ mfDumpBlocks NOTIFY mfDumpChanged)
    Q_PROPERTY(QStringList logLines READ logLines NOTIFY logLinesChanged)

public:
    explicit PmProxy(Proxmark* worker, QObject* parent = Q_NULLPTR);

    // Write the current HF readout (type + fields) and the log to a file in the
    // user's Documents dir. csv=false -> .txt (readable), csv=true -> .csv
    // (section,label,value). Returns the full path, or "" on failure/empty.
    // format: "txt" (readable), "csv" (section,label,value) or "json"
    // (structured, re-usable later). target empty -> Documents/pm5-<ts>.<ext>;
    // otherwise that path (file:// stripped, dirs created, extension added).
    Q_INVOKABLE QString exportLog(const QString& format, const QString& target = QString());

    bool present() const { return m_present; }
    bool connected() const { return m_connected; }
    QString portPath() const;
    QString version() const { return m_version; }
    QString model() const { return m_model; }
    QString fwVer() const { return m_fwVer; }
    QString romVer() const { return m_romVer; }
    QString status() const { return m_status; }
    bool busy() const { return m_busy; }
    QString lastTag() const { return m_lastTag; }
    QString lastTagType() const { return m_lastTagType; }
    QString hfUid() const { return m_hfUid; }
    QString hfType() const { return m_hfType; }
    QString hfDetails() const { return m_hfDetails; }
    QVariantList hfFields() const { return m_hfFields; }
    QString mfResult() const { return m_mfResult; }
    int mfFound() const { return m_mfFound; }
    QString mfDump() const { return m_mfDump; }
    int mfDumpBlocks() const { return m_mfDumpBlocks; }
    QStringList logLines() const { return m_log; }

public slots:
    void connectDevice();
    void disconnect();
    void readVersion();
    void readLf();
    void writeEm4100(const QString& hexId);
    void readHf();
    void checkMifareKeys();
    void dumpMifare();
    void readLogWithPin(const QString& pin);
    void rawApdu(const QString& apduHex, bool hold);
    void rawCmd(const QString& cmdHex, const QString& dataHex, bool ng);
    void simulateUid(const QString& uidHex);
    void stopEmulation();

signals:
    void presentChanged();
    void connectedChanged();
    void versionChanged();
    void statusChanged();
    void busyChanged();
    void lfChanged();
    void hfChanged();
    void mfChanged();
    void mfDumpChanged();
    void logLinesChanged();

private slots:
    void onSnapshot(const QVariantMap& s);
    void pollPresent();

private:
    Proxmark* m_worker;
    bool m_present;
    bool m_connected;
    bool m_busy;
    int m_mfFound;
    int m_mfDumpBlocks;
    QString m_version, m_status, m_lastTag, m_lastTagType;
    QString m_model, m_fwVer, m_romVer;
    bool m_userDisconnect = false;     // user chose Disconnect -> no auto-reconnect
    bool m_autoConnectTried = false;   // auto-connect fired once for this plug-in
    QString m_hfUid, m_hfType, m_hfDetails, m_mfResult, m_mfDump;
    QVariantList m_hfFields;
    QStringList m_log;
    QTimer m_pollTimer;
};

#endif // PMPROXY_H
