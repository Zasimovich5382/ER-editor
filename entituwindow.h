#ifndef ENTITUWINDOW_H
#define ENTITUWINDOW_H

#include <QDialog>
#include <QComboBox>
#include "keetype.h"
namespace Ui {
class EntituWindow;
}

class EntituWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EntituWindow(QWidget *parent = nullptr);
    ~EntituWindow();
    QString nameEntity();
    void setName(QString name);
    void setKee(QVector<KeeType> kee);
    QVector<KeeType> getKee();

    EntituWindow(QVector<KeeType> kee, QWidget *parent = nullptr);

signals:
    void firstwindow();

private slots:
    void on_changeEntity_clicked();

    void on_addButton_clicked();

    void deleteLayout();

private:
    Ui::EntituWindow *ui;
    QVector<KeeType> keeEntity;
    QVector<QString> type = {"INT", "CHAR"};
};

#endif // ENTITUWINDOW_H
