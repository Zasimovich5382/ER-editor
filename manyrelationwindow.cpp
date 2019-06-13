

#include "manyrelationwindow.h"


#include <QGridLayout>

ManyRelationWindow::ManyRelationWindow(QWidget *parent) :
    QWidget(parent),
    degreeButton(new QPushButton("Quantity", this)),
    lineEdit(new QLineEdit(this)),
    gridLayout(new QGridLayout(this))
{
    setLayout(gridLayout);
    gridLayout->addWidget(degreeButton, 0, 0, 1, 1);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 1);
    connect(degreeButton, &QPushButton::clicked, this, &ManyRelationWindow::addComboBox);

}
void ManyRelationWindow::addComboBox(){
    int degree = (lineEdit->text()).toInt();
    this->clear();
    degreeButton = new QPushButton("Quantity", this);
    lineEdit = new QLineEdit(this);
    gridLayout->addWidget(degreeButton, 0, 0, 1, 1);
    gridLayout->addWidget(lineEdit, 0, 1, 1, 1);
    connect(degreeButton, &QPushButton::clicked, this, &ManyRelationWindow::addComboBox);
    int i;
    for(i = 1; i<degree+1; i++){

        comboBox = new QComboBox(this);
        mandatory = new QComboBox(this);
        type = new QComboBox(this);

        gridLayout->addWidget( comboBox, i, 0, 1, 1);
        gridLayout->addWidget( mandatory, i, 1, 1, 1);
        gridLayout->addWidget( type, i, 2, 1, 1);

        for(int j = 0; j<entitys->length();j++){
            comboBox->addItem(entitys->at(j));
        }


        mandatory->addItem("Обязательная");
        mandatory->addItem("Необязательная");
        type->addItem("Множественная");
        type->addItem("Единичная");
    }
    QPushButton *addButton = new QPushButton("Add", this);
    gridLayout->addWidget( addButton, i, 2, 1, 1);
    connect(addButton, &QPushButton::clicked, this, &ManyRelationWindow::addRelation);
}

void ManyRelationWindow::addRelation(){
    emit threewindow();
    this->clear();
    this->close();

}

QVector<QString> ManyRelationWindow::getName(){
    QVector<QString> *vector = new QVector<QString>;
    if (! gridLayout)
        return *vector;

    for(int i = 2; i<gridLayout->count()-1;i=i+3) {
            QComboBox *box = static_cast<QComboBox*>(gridLayout->itemAt(i)->widget());
            vector->append(box->currentText());
    }

    return *vector;

}

QVector<QString> ManyRelationWindow::getMandatory(){
    QVector<QString> *vector = new QVector<QString>;
    if (! gridLayout)
        return *vector;

    for(int i = 3; i<gridLayout->count()-1;i=i+3) {
        QComboBox *box = static_cast<QComboBox*>(gridLayout->itemAt(i)->widget());
        vector->append(box->currentText());
    }

    return *vector;
}
QVector<QString> ManyRelationWindow::getType(){
    QVector<QString> *vector = new QVector<QString>;
    if (! gridLayout)
        return *vector;

    for(int i = 4; i<gridLayout->count()-1;i=i+3) {
        QComboBox *box = static_cast<QComboBox*>(gridLayout->itemAt(i)->widget());
        vector->append(box->currentText());
    }

    return *vector;
}

ManyRelationWindow::~ManyRelationWindow()
{


}

void ManyRelationWindow::clear() {
    if (! gridLayout)
        return;
    while (auto item = gridLayout->takeAt(0)) {
        delete item->widget();
    }
}

void ManyRelationWindow::addEntity(QVector<QString> *vector){
    this->entitys = vector;
}
