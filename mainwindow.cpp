#include "relation.h"
#include "entity.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include <QTextEdit>
#include <QtWidgets>


//! [0]
MainWindow::MainWindow()
{
    createActions();
    createToolBox();
    createMenus();

    scene = new SceneER(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));

    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    windowEntity = new EntituWindow(this);
    connect(windowEntity, &EntituWindow::firstwindow, this, &MainWindow::changeEntity);

    windowRelation = new relationWindow(this);
    connect(windowRelation, &relationWindow::twowindow, this, &MainWindow::addBinaryRelation);

    manyRelationWindow = new ManyRelationWindow();
    connect(manyRelationWindow, &ManyRelationWindow::threewindow, this, &MainWindow::addManyRelation);

    windowRelationChange = new relationChange();
    connect(windowRelationChange, &relationChange::relationwindow, this, &MainWindow::changeRelation);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    this->setStyleSheet("MainWindow{ background-color: blue; border-radius:5%; }");
    setWindowTitle(tr("ER-editor"));
    setUnifiedTitleAndToolBarOnMac(true);
    scene->setBackgroundBrush(QPixmap(":/images/background3.png"));







}

void MainWindow::save(){
    QSqlQuery query, query2;
    query.exec("DELETE FROM Entity");
    query.exec("DELETE FROM attributes");

    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == Entity::Type){
            QString name = qgraphicsitem_cast<Entity *>(item)->nameEntity;

            QVector<KeeType> kees =qgraphicsitem_cast<Entity *>(item)->keeEntity;

            QString id =QString::number(qgraphicsitem_cast<Entity *>(item)->idDB);

            for(int j = 0; j<kees.length()+1; j++){
                KeeType kee = kees.at(j);
                query.exec("INSERT INTO attributes (entity_number,nameAttr,datatype, size)" "VALUES ("+id+",'"+kee.getName()+"','"+kee.getType()+"', "+QString::number(kee.getSize())+")");
            }

            query2.exec("INSERT INTO Entity (entity_number,name,x,y)" "VALUES ("+id+",'"+name+"',"+QString::number(qgraphicsitem_cast<Entity *>(item)->pos().rx())+","+QString::number(qgraphicsitem_cast<Entity *>(item)->pos().ry())+")");


        }
    }


    query.exec("DELETE FROM ER");
    int i = 1;
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == Relation::Type){
            Relation *arrow = qgraphicsitem_cast<Relation *>(item);
            QString id = QString::number(i);
            for(int j = 0; j<arrow->ItemsAll().length();j++){

                query.exec("INSERT INTO ER (entity_number,№relationship,mandatory,degree)" "VALUES ("+QString::number(arrow->ItemsAll().at(j)->idDB)+","+id+",'"+arrow->getMandatory().at(j)+"','"+arrow->getRelations().at(j)+"')");
            }
            i++;
        }
    }

}



void MainWindow::saveFile(){

    }



void MainWindow::open(){

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "/home/jana", tr("Image Files (*.db )"));
    db.setDatabaseName(fileName);
    db.open();
    QSqlQuery query;
    query.exec("SELECT Entity.entity_number, name, x, y, nameAttr, datatype, size FROM Entity, attributes WHERE Entity.entity_number = attributes.entity_number");
    //Выводим значения из запроса
    while (query.next())
    {

        QString name = query.value(1).toString();
        int x = query.value(2).toInt();
        int y = query.value(3).toInt();
        QString nameAttr = query.value(4).toString();
        QString datatype = query.value(5).toString();
        int size = query.value(6).toInt();
        Entity* ent = scene->addEntity(name,QPointF(50,50));
        ent->setPos(x,y);
        ent->addNewKee(KeeType(nameAttr,datatype, size));
        ent->idDB = query.value(0).toInt();
    }

    int numberRelation = -1;
    QVector <Entity*> entity;

    QVector<QString> mandatory;

    QVector<QString> degree;

    QSqlQuery query2;
    query2.exec("SELECT ER.№relationship, ER.mandatory, ER.degree, name FROM ER, Entity WHERE Entity.entity_number = ER.entity_number");
    if(query2.next()){
    while (query2.next())
    {

        if(numberRelation != query2.value(0).toInt()){
            if(numberRelation != -1){

                scene->addRelationMany(entity, degree, mandatory);
                entity.clear();
                mandatory.clear();
                degree.clear();
                numberRelation = query2.value(0).toInt();
            }else{
                numberRelation = query2.value(0).toInt();
            }
        }
        numberRelation = query2.value(0).toInt();

        QString name = query2.value(3).toString();
        foreach(QGraphicsItem *item, scene->items()){
            if (item->type() == Entity::Type){
                Entity *ent = qgraphicsitem_cast<Entity *>(item);
                if(ent->nameEntity == name) entity.append(ent);
            }
        }

        mandatory.append(query2.value(1).toString());
        degree.append(query2.value(2).toString());

    }
    scene->addRelationMany(entity, degree, mandatory);
    }
}
void MainWindow::changeEntity()
{
    QString name = windowEntity->nameEntity();

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Entity::Type){
            qgraphicsitem_cast<Entity *>(item)->setName(name);
            qgraphicsitem_cast<Entity *>(item)->changeKee(windowEntity->getKee());
        }


    scene->update();

     }
}

void MainWindow::changeRelation(){
    QString type = windowRelationChange->getRelation();

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Relation::Type){
            qgraphicsitem_cast<Relation *>(item)->relationType = type;

        }


        scene->update();

    }
}

void MainWindow::background(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("1"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("2"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("3"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void MainWindow::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
        scene->setItemType(Entity::EntityType(id));
        scene->setObj(SceneER::InsertEntity);

}
//! [2]

//! [3]
void MainWindow::deleteEl()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Relation::Type) {
            scene->removeItem(item);
            Relation *arrow = qgraphicsitem_cast<Relation *>(item);
            QVector <Entity*> vector = arrow->ItemsAll();
            for(int i = 0; i<vector.length();i++){
                vector.at(i)->removeRelation(arrow);
            }
            delete item;
        }
    }
    scene->update();

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Entity::Type){
            qgraphicsitem_cast<Entity *>(item)->removeRelations();
            QList<Entity*> vector = qgraphicsitem_cast<Entity *>(item)->dependents;
            for(int i =0;i<vector.length();i++){
                vector.at(i)->removeRelations();
                delete vector.at(i);
            }
            QList<Entity*> rvector = qgraphicsitem_cast<Entity *>(item)->roles;
            for(int i =0;i<rvector.length();i++){
                rvector.at(i)->addNewKee(qgraphicsitem_cast<Entity *>(item)->keeEntity);

            }
        }

        scene->removeItem(item);
        delete item;
    }
    scene->update();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::changeItem()
{

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Relation::Type){
            windowRelationChange = new relationChange();
            connect(windowRelationChange, &relationChange::relationwindow, this, &MainWindow::changeRelation);
            windowRelationChange->setRelation(qgraphicsitem_cast<Relation *>(item)->relationType);
            windowRelationChange->show();
        }

    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Entity::Type){
            windowEntity = new EntituWindow(qgraphicsitem_cast<Entity *>(item)->keeEntity, this);
            connect(windowEntity, &EntituWindow::firstwindow, this, &MainWindow::changeEntity);
            windowEntity->setName(qgraphicsitem_cast<Entity *>(item)->nameEntity);
            windowEntity->show();
        }

    }



}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::addDependentEntity()
{


    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Entity::Type){

            scene->addDependent(qgraphicsitem_cast<Entity *>(item), item->scenePos());

        }

    }

}

void MainWindow::addRoleEntity(){
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Entity::Type){

            scene->addRole(qgraphicsitem_cast<Entity *>(item), item->scenePos());

        }

    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::addBinaryRelationWindow()
{
    windowRelation->remove();

    foreach(QGraphicsItem *item, scene->items()){
        if (item->type() == Entity::Type){
            QString name = qgraphicsitem_cast<Entity *>(item)->nameEntity;
            windowRelation->addOne(name);
            windowRelation->addTwo(name);
        }
    }

    windowRelation->show();

}

void MainWindow::addBinaryRelation(){
    QString one = windowRelation->getOne();
    QString two = windowRelation->getTwo();
    Entity *itemOne;
    Entity *itemTwo;
    foreach(QGraphicsItem *item, scene->items()){
        if (item->type() == Entity::Type){
            Entity *ent = qgraphicsitem_cast<Entity *>(item);
            if(ent->nameEntity == one) itemOne = ent;
            if(ent->nameEntity == two) itemTwo = ent;
        }
    }
    scene->addRelationBynary(itemOne, itemTwo, windowRelation->getMandatory(),windowRelation->getDegree());
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::addManyRelationWindow(){
    manyRelationWindow = new ManyRelationWindow();
    connect(manyRelationWindow, &ManyRelationWindow::threewindow, this, &MainWindow::addManyRelation);
    manyRelationWindow->setGeometry(100, 100, 500, 500);
    QVector<QString> *vector = new QVector<QString>;

    foreach(QGraphicsItem *item, scene->items()){
        if (item->type() == Entity::Type){
            QString name = qgraphicsitem_cast<Entity *>(item)->nameEntity;
            vector->append(name);
        }
    }
    manyRelationWindow->addEntity(vector);
    manyRelationWindow->show();
}

void MainWindow::addManyRelation(){
    QVector<QString> vector = manyRelationWindow->getName();
    QVector<Entity*> entity;


        for(int i=0;i<vector.length();i++){
            foreach(QGraphicsItem *item, scene->items()){
                if (item->type() == Entity::Type){
                  Entity *ent = qgraphicsitem_cast<Entity *>(item);
                  if(ent->nameEntity == vector.at(i)) entity.append(ent);
                }
        }

    }
    scene->addRelationMany(entity, manyRelationWindow->getType(), manyRelationWindow->getMandatory());

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::pointerGroupClicked(int)
{
    scene->setObj(SceneER::Obj(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]

//! [6]

//! [7]
void MainWindow::itemInserted(Entity *item)
{
    pointerTypeGroup->button(int(SceneER::MoveItem))->setChecked(true);
    scene->setObj(SceneER::Obj(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->entityType()))->setChecked(false);
}
//! [7]

//! [8]

//! [8]

//! [9]

//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About ER-editor"),
                       tr("The <b>ER-editor</b>" ));
}
//! [20]

//! [21]
void MainWindow::createToolBox()
{

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));


}
//! [22]

//! [23]
void MainWindow::createActions()
{


    saveDB = new QAction(QIcon(":/images/save.png"),tr("Save"), this);
    connect(saveDB, SIGNAL(triggered()), this, SLOT(save()));

    saveDBfile = new QAction(QIcon(":/images/save2.png"),tr("Save"), this);
    connect(saveDBfile, SIGNAL(triggered()), this, SLOT(save()));

    openDB = new QAction(QIcon(":/images/open.png"),tr("Open"), this);
    connect(openDB, SIGNAL(triggered()), this, SLOT(open()));



    windowRelationBynary = new QAction(QIcon(":/images/sendtoback.png"), tr("Add binary relation"), this);
    windowRelationBynary->setStatusTip(tr("Add binary relation"));
    connect(windowRelationBynary, SIGNAL(triggered()), this, SLOT(addBinaryRelationWindow()));

    manyWindowRelationBynary = new QAction(QIcon(":/images/sendtoback.png"), tr("Add many relation"), this);
    manyWindowRelationBynary->setStatusTip(tr("Add many relation"));
    connect(manyWindowRelationBynary, SIGNAL(triggered()), this, SLOT(addManyRelationWindow()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteEl()));

    changeAction = new QAction(QIcon(":/images/change.png"), tr("&Change"), this);
    changeAction->setShortcut(tr("Change"));
    changeAction->setStatusTip(tr("Change item from diagram"));
    connect(changeAction, SIGNAL(triggered()), this, SLOT(changeItem()));


    dependentEntity = new QAction(QIcon(":/images/plus.png"), tr("&Add dependet Entity"), this);
    dependentEntity->setShortcut(tr("Add dependet Entity"));
    dependentEntity->setStatusTip(tr("Add dependet Entity"));
    connect(dependentEntity, SIGNAL(triggered()), this, SLOT(addDependentEntity()));

    roleEntity = new QAction(QIcon(":/images/plus.png"), tr("&Add Role"), this);
    roleEntity->setShortcut(tr("Add role"));
    roleEntity->setStatusTip(tr("Add Role Entity"));
    connect(roleEntity, SIGNAL(triggered()), this, SLOT(addRoleEntity()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

//! [24]
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();

    itemMenu->addAction(changeAction);
    itemMenu->addAction(dependentEntity);
    itemMenu->addAction(roleEntity);

    addMenu = menuBar()->addMenu(tr("&Add"));
    addMenu->addAction(windowRelationBynary);
    addMenu->addAction(manyWindowRelationBynary);


    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}
//! [24]

//! [25]
void MainWindow::createToolbars()
{
    //! [25]
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(saveDBfile);
     editToolBar->addAction(saveDB);
    editToolBar->addAction(openDB);
    editToolBar->addAction(saveDB);




    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    QToolButton *linePointerButton2 = new QToolButton;
    linePointerButton2->setCheckable(true);
    linePointerButton2->setIcon(QIcon(":/images/1m.png"));

    QToolButton *linePointerButton3 = new QToolButton;
    linePointerButton3->setCheckable(true);
    linePointerButton3->setIcon(QIcon(":/images/m1.png"));

    QToolButton *linePointerButton4 = new QToolButton;
    linePointerButton4->setCheckable(true);
    linePointerButton4->setIcon(QIcon(":/images/mm.png"));

    QToolButton *entityButton = new QToolButton;
    entityButton->setCheckable(true);
    entityButton->setIcon(QIcon(":/images/ent.png"));

    QToolButton *scrinButton = new QToolButton;
    scrinButton->setCheckable(true);
    scrinButton->setIcon(QIcon(":/images/scrin.png"));

    QToolButton *manyButton = new QToolButton;
    manyButton->setCheckable(true);
    manyButton->setIcon(QIcon(":/images/many.png"));



    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(SceneER::MoveItem));
    pointerTypeGroup->addButton(linePointerButton,
                                int(SceneER::OneToOne));
    pointerTypeGroup->addButton(linePointerButton2,
                                int(SceneER::OneToMany));
    pointerTypeGroup->addButton(linePointerButton3,
                                int(SceneER::ManyToOne));
    pointerTypeGroup->addButton(linePointerButton4,
                                int(SceneER::ManyToMany));
    pointerTypeGroup->addButton(entityButton,
                                int(SceneER::InsertEntity));




    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    backgroundButtonGroup = new QButtonGroup(this);


    QToolButton *button1 = new QToolButton;
    button1->setText("1");
    button1->setIcon(QIcon(":/images/background1.png"));
    button1->setCheckable(true);
    backgroundButtonGroup->addButton(button1);



    QToolButton *button2 = new QToolButton;
    button2->setIcon(QIcon(":/images/background4.png"));
    button1->setText("4");
    button2->setCheckable(true);
    backgroundButtonGroup->addButton(button2);

    QToolButton *button3 = new QToolButton;
    button3->setIcon(QIcon(":/images/background3.png"));
    button1->setText("3");
    button3->setCheckable(true);
    backgroundButtonGroup->addButton(button3);



    QToolButton *button4 = new QToolButton;
    button4->setIcon(QIcon(":/images/background2.png"));
    button1->setText("2");
    button4->setCheckable(true);
    backgroundButtonGroup->addButton(button4);



    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(background(QAbstractButton*)));








    toolbar1 = addToolBar(tr("Pointer type"));
    toolbar1->addWidget(pointerButton);
    toolbar1->addWidget(linePointerButton);
    toolbar1->addWidget(linePointerButton2);
    toolbar1->addWidget(linePointerButton3);
    toolbar1->addWidget(linePointerButton4);
    toolbar1 -> addWidget(manyButton);
    toolbar1->addWidget(entityButton);
    toolbar1->addAction(deleteAction);
    toolbar1->addAction(changeAction);
    toolbar2 = addToolBar(tr(" "));
    toolbar2->addWidget(sceneScaleCombo);
    toolbar2 -> addWidget(button1);
    toolbar2 -> addWidget(button2);
    toolbar2 -> addWidget(button3);
    toolbar2 -> addWidget(button4);
    toolbar2 -> addWidget(scrinButton);

}




QWidget *MainWindow::createCellWidget(const QString &text, Entity::EntityType type)
{




    QToolButton *button = new QToolButton;

    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]



