#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

#include <QOlm/QOlm.hpp>

class Setup : public QObject
{
    Q_OBJECT

public:
    Setup() {}

public slots:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
        //engine->rootContext()->setContextProperty("myContextProperty", QVariant(true));
        qDebug() << "Enter in the function" ;
        qmlRegisterType<qolm::QOlm<QObject>>("QOlmTestsQml", 1, 0, "QtObjectList");
    }
};

QUICK_TEST_MAIN_WITH_SETUP(QOlmTestsQml, Setup)

#include "QOlmTestsQml.moc"