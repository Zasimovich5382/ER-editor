#ifndef RELATIONWINDOW_H
#define RELATIONWINDOW_H

#include <QComboBox>
#include <QDialog>

namespace Ui {
class relationWindow;
}

class relationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit relationWindow(QWidget *parent = nullptr);
    ~relationWindow();
    void addOne(QString name);
    void addTwo(QString name);
    void remove();
    QString getOne();
    QString getTwo();
    QVector<QString>getMandatory();
    QString getDegree();
signals:
    void twowindow();



private slots:
    void on_addRelationBinary_clicked();

private:
    QComboBox *comboBox;
    Ui::relationWindow *ui;

};

#endif // RELATIONWINDOW_H
