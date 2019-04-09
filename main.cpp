#include <QGuiApplication>
//#include <QQuickStyle>
#include "ardbl.h"



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<arduinoBL>("ModuleArduino", 1, 0, "ArduinoBl");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
       exit(-1);

    return app.exec();
}
