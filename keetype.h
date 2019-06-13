#ifndef KEETYPE_H
#define KEETYPE_H
#include <QList>
#include <QVariant>

class KeeType
{
public:
    KeeType();
    //~KeeType();

    KeeType(QString name, QString type, int size);

    QString getName();
    QString getType();
    int getSize();

    void setName(QString name);
    void setType(QString type);
    void setSize(int size);
private:
    QString type;
    QString name;
    int size;
};

#endif // KEETYPE_H
