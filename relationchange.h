#ifndef RELATIONCHANGE_H
#define RELATIONCHANGE_H

#include <QDialog>

namespace Ui {
class relationChange;
}

class relationChange : public QDialog
{
    Q_OBJECT

public:
    explicit relationChange(QWidget *parent = nullptr);
    ~relationChange();
    void setRelation(QString type);
    QString getRelation();
signals:
    void relationwindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::relationChange *ui;
};

#endif // RELATIONCHANGE_H
