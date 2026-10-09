#include "proxmark.h"
#include "pmproxy.h"

#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QScopedPointer>
#include <QThread>

#ifndef APP_TARGET
#  define APP_TARGET "harbour-pm5"
#endif

int main(int argc, char* argv[])
{
    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
    app->setApplicationName(QStringLiteral(APP_TARGET));
    app->setOrganizationName(QStringLiteral(APP_TARGET));

    // The blocking serial backend runs in its own thread; the GUI talks to it
    // only through PmProxy (queued calls out, value snapshots back).
    Proxmark* worker = new Proxmark();
    QThread workerThread;
    worker->moveToThread(&workerThread);
    QObject::connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    workerThread.start();

    PmProxy proxy(worker);

    QScopedPointer<QQuickView> view(SailfishApp::createView());
    QQmlContext* ctx = view->rootContext();
    ctx->setContextProperty(QStringLiteral("Proxmark"), &proxy);

    view->setSource(SailfishApp::pathTo(QStringLiteral("qml/main.qml")));
    view->showFullScreen();

    const int rc = app->exec();

    workerThread.quit();
    workerThread.wait(3000);
    return rc;
}
