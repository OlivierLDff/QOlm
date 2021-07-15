#include <QOlm/QOlm.hpp>
#include <QtCore/QDebug>

using ObjectList = qolm::QOlm<QObject>;

int main(int argc, char *argv[])
{
    ObjectList list;
    list.append(new QObject);
    qDebug() << "list count : " << list.count();
    return 0;
}
