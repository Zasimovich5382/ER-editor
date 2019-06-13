#ifndef MANYRELATIONWINDOW_H
#define MANYRELATIONWINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QGridLayout>
class ManyRelationWindow : public QWidget
{
     Q_OBJECT
public:
    explicit ManyRelationWindow(QWidget *parent = 0);
    ~ManyRelationWindow();
    void clear();
    QVector<QString> getName();
    QVector<QString> getType();
    QVector<QString> getMandatory();
    void addEntity(QVector<QString> *vector);
signals:
    void threewindow();

private slots:
    void addComboBox();
    void addRelation();
private:
    QVector<QComboBox *> vector;
    QVector<QString> *entitys;
    QPushButton *degreeButton;
    QLineEdit *lineEdit;
    QGridLayout *gridLayout;
    QComboBox *comboBox;
    QComboBox *mandatory;
    QComboBox *type;

};

#endif // MANYRELATIONWINDOW_H
