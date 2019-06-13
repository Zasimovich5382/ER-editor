#include "entituwindow.h"
#include "ui_entituwindow.h"
#include "qdynamicbutton.h"
#include "qdynamiclayout.h"

EntituWindow::EntituWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntituWindow)
{
    ui->setupUi(this);
}
EntituWindow::EntituWindow(QVector<KeeType> kee, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntituWindow)
{
    ui->setupUi(this);

    if(kee.length()!=0){
        for(int i = 0; i < kee.length(); i++){
            KeeType keeType = kee.at(i);

            QDynamicLayout * gorizontLayout = new QDynamicLayout;

            QLineEdit *lineEdit = new QLineEdit;
            lineEdit->setText(keeType.getName());

            QComboBox *comboBox = new QComboBox;
            comboBox->currentText() = keeType.getType();

            QLineEdit *lineEdit2 = new QLineEdit;
            lineEdit2->setText(QString::number(keeType.getSize()));

            QDynamicButton *deleteButton = new QDynamicButton;
            deleteButton->setText("Удалить");
            for(int i=0;i<type.length();i++){
                comboBox->addItem(type.at(i));
            }
            connect(deleteButton, &QPushButton::clicked, this, &EntituWindow::deleteLayout);
            gorizontLayout->addWidget(lineEdit);
            gorizontLayout->addWidget(comboBox);
            gorizontLayout->addWidget(lineEdit2);
            gorizontLayout->addWidget(deleteButton);


            ui->verticalLayout->addLayout(gorizontLayout);

        }
    }
}

EntituWindow::~EntituWindow()
{
    delete ui;
}

void EntituWindow::on_changeEntity_clicked()
{
    this->close();
    emit firstwindow();
}

QString EntituWindow:: nameEntity(){
    QString name = ui->nameEntity->text();
    return name;
}


void EntituWindow::setName(QString name)
{
    ui->nameEntity->setText(name);
}


void EntituWindow::on_addButton_clicked()
{
    QDynamicLayout * gorizontLayout = new QDynamicLayout;
    QLineEdit *lineEdit = new QLineEdit;
    QComboBox *comboBox = new QComboBox;
    QLineEdit *lineEdit2 = new QLineEdit;
    QDynamicButton *deleteButton = new QDynamicButton;
    deleteButton->setText("Удалить");
    for(int i=0;i<type.length();i++){
        comboBox->addItem(type.at(i));
    }
    connect(deleteButton, &QPushButton::clicked, this, &EntituWindow::deleteLayout);
    gorizontLayout->addWidget(lineEdit);
    gorizontLayout->addWidget(comboBox);
    gorizontLayout->addWidget(lineEdit2);
    gorizontLayout->addWidget(deleteButton);


    ui->verticalLayout->addLayout(gorizontLayout);
}

void EntituWindow::deleteLayout(){

    QDynamicButton *deleteButton = qobject_cast<QDynamicButton*>(sender());
    int number = deleteButton->getID();

    for(int i = 0; i < ui->verticalLayout->count(); i++){

        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(ui->verticalLayout->itemAt(i)->layout());

        if(dynamicLayout->getID() == number){

            delete dynamicLayout->itemAt(0)->widget();
            delete dynamicLayout->itemAt(0)->widget();
            delete dynamicLayout->itemAt(0)->widget();
            delete dynamicLayout->itemAt(0)->widget();
        }

    }

}

QVector<KeeType> EntituWindow::getKee(){
    QVector<KeeType> vector;
    for(int i = 0; i < ui->verticalLayout->count(); i++){

        QDynamicLayout *dynamicLayout = qobject_cast<QDynamicLayout*>(ui->verticalLayout->itemAt(i)->layout());
        if(dynamicLayout->count()!=0){
            QString name = qobject_cast<QLineEdit*>(dynamicLayout->itemAt(0)->widget())->text();
            QString type = qobject_cast<QComboBox*>(dynamicLayout->itemAt(1)->widget())->currentText();
            QString size = qobject_cast<QLineEdit*>(dynamicLayout->itemAt(2)->widget())->text();
            vector.append(KeeType(name,type,size.toInt()));
        }


    }
    return vector;
}


