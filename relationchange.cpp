#include "relationchange.h"
#include "ui_relationchange.h"

relationChange::relationChange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::relationChange)
{
    ui->setupUi(this);
    ui->comboBox->addItem("1 : 1");
    ui->comboBox->addItem("1 : n");
    ui->comboBox->addItem("n : 1");
    ui->comboBox->addItem("m : n");
}

relationChange::~relationChange()
{
    delete ui;
}

void relationChange::on_pushButton_clicked()
{
    this->close();
    emit relationwindow();
}

void relationChange::setRelation(QString type){
    ui->comboBox->currentText() = type;
}
QString relationChange::getRelation(){
    return ui->comboBox->currentText();
}
