#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include <QFont>

#include "backend.h"

int main(int argc, char *argv[])
{    
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setFont(QFont(QLatin1String("monospace")));

    QCoreApplication::setOrganizationName("QTrackUplink");
    QCoreApplication::setApplicationName("QTrackUplink");

    Backend backend;

    backend.init();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("backend", &backend);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
