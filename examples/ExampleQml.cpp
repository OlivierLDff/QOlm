#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <FooExample.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Foo>("Example", 1, 0, "Foo");
    qmlRegisterType<FooList>("Example", 1, 0, "FooList");
    qmlRegisterType<qolm::QOlm<Foo>>("Example", 1, 0, "FooListTemplate");
    qmlRegisterType<qolm::QOlm<QObject>>("Example", 1, 0, "QtObjectList");

    const QUrl url(QStringLiteral("qrc:/ExampleQml.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
