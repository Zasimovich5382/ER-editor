#ifndef QDYNAMICLAYOUT_H
#define QDYNAMICLAYOUT_H

#include <QPushButton>
#include <QHBoxLayout>

class QDynamicLayout : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit QDynamicLayout(QWidget *parent = 0);
    ~QDynamicLayout();
    static int ResID;
    int getID();


public slots:

private:
    int layoutID = 0;
};
#endif // QDYNAMICLAYOUT_H
