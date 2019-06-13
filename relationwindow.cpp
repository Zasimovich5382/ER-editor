#include "relationwindow.h"
#include "ui_relationwindow.h"

relationWindow::relationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::relationWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Обязательная");
    ui->comboBox->addItem("Необязательная");
    ui->comboBox_2->addItem("Обязательная");
    ui->comboBox_2->addItem("Необязательная");
    ui->comboBox_3->addItem("1 : 1");
    ui->comboBox_3->addItem("1 : n");
    ui->comboBox_3->addItem("n : 1");
    ui->comboBox_3->addItem("m : n");
}

void relationWindow::addOne(QString name){
    ui->entityOne->addItem(name);
}

void relationWindow::addTwo(QString name){
    ui->entityTwo->addItem(name);
}

void relationWindow::remove(){
    ui->entityOne->clear();
    ui->entityTwo->clear();
};

QString relationWindow::getOne(){
    return ui->entityOne->currentText();
}
QString relationWindow::getTwo(){
    return ui->entityTwo->currentText();
}
relationWindow::~relationWindow()
{
    delete ui;
}

void relationWindow::on_addRelationBinary_clicked()
{
    this->close();
    emit twowindow();
}
QVector<QString> relationWindow::getMandatory(){
    QVector<QString> vector = {ui->comboBox->currentText(),ui->comboBox_2->currentText()};
    return vector;
}
QString relationWindow::getDegree(){
    return ui->comboBox_3->currentText();
}
