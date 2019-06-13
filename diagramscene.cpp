#include "diagramscene.h"
#include "relation.h"
#include "entity.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

//! [0]
SceneER::SceneER(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myObj = MoveItem;
    myItemType = Entity::Ent;
    line = 0;
    nameEntity = "Untitled";
}




void SceneER::setObj(Obj obj)
{
    myObj = obj;
}

void SceneER::setNameEntity(QString name)
{
    nameEntity = name;
}
void SceneER::setItemType(Entity::EntityType type)
{
    myItemType = type;
}

void SceneER::setKeeEntity(QVector<KeeType> kee)
{
    keeEntity = kee;
}



void SceneER::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    Entity *item;
    switch (myObj) {
    case InsertEntity:
        item = new Entity(nameEntity, keeEntity, myItemMenu);
        addItem(item);
        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit itemInserted(item);
        myObj = MoveItem;
        break;
    case ManyToMany:
        man = {"Обязательная","Обязательная"};
        deg = {"Множественная","Множественная"};
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black, 2));
        addItem(line);
        break;
    case OneToMany:
        man = {"Обязательная","Обязательная"};
        deg = {"Единичная","Множественная"};
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black, 2));
        addItem(line);
        break;
    case ManyToOne:
        man = {"Обязательная","Обязательная"};
        deg = {"Множественная","Единичная"};
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black, 2));
        addItem(line);
        break;
    case OneToOne:
        man = {"Обязательная","Обязательная"};
        deg = {"Единичная","Единичная"};
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                            mouseEvent->scenePos()));
        line->setPen(QPen(Qt::black, 2));
        addItem(line);
        break;
    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
    update();
}
//! [9]

//! [10]
void SceneER::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if ((line != 0 && myObj == ManyToMany)||(line != 0 && myObj == OneToMany)||(line != 0 && myObj ==ManyToOne)||(line != 0 && myObj ==OneToOne)) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myObj == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
        update();

}
//! [10]

//! [11]
void SceneER::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if ((line != 0 && myObj == ManyToMany)||(line != 0 && myObj == OneToMany)||(line != 0 && myObj ==ManyToOne)||(line != 0 && myObj ==OneToOne)) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == Entity::Type &&
            endItems.first()->type() == Entity::Type &&
            startItems.first() != endItems.first()) {
            Entity *startItem =
                qgraphicsitem_cast<Entity *>(startItems.first());
            Entity *endItem =
                qgraphicsitem_cast<Entity *>(endItems.first());
            Relation *relation = new Relation(startItem, endItem);



            relation->setRelations(deg);
            relation->setMandatory(man);
            startItem->addRelation(relation);
            endItem->addRelation(relation);
            relation->setZValue(-1000.0);
            addItem(relation);
            relation->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();
}
//! [13]

//! [14]



void SceneER::addRelationBynary(Entity *item1, Entity *item2, QVector<QString> mandatory, QString degree){
    Relation *relation = new Relation(item1, item2);
    relation->setMandatory(mandatory);
    relation->setType(degree);
    item1->addRelation(relation);
    item2->addRelation(relation);
    relation->setZValue(-1000.0);
    this->addItem(relation);
    relation->updatePosition();


};
void SceneER::addRelationMany(QVector <Entity*> vector, QVector<QString>type, QVector<QString>mandatory){
    Relation *relation = new Relation(vector);
    relation->setMandatory(mandatory);
    relation->setRelations(type);
    for(int i = 0; i<vector.length(); i++){
        vector.at(i)->addRelation(relation);
    }
    relation->setZValue(-1000.0);
    this->addItem(relation);
    relation->updatePosition();
    update();

}
void SceneER::addDependent(Entity * item1, QPointF f){

    Entity *item2 = new Entity(nameEntity, keeEntity, myItemMenu);
    addItem(item2);
    item2->setPos(f+QPointF(0,200));

    item1->dependents.append(item2);

    Relation *relation = new Relation(item1, item2);
    relation->setMandatory({"Обязательная","Обязательная"});
    relation->setType("Зависимая сущность");
    item1->addRelation(relation);
    item2->addRelation(relation);
    relation->setZValue(-1000.0);
    this->addItem(relation);
    relation->updatePosition();

};

void SceneER::addRole(Entity * item1, QPointF f){

    Entity *item2 = new Entity(nameEntity, keeEntity, myItemMenu);
    addItem(item2);
    item2->setPos(f+QPointF(0,200));

    item1->roles.append(item2);

    Relation *relation = new Relation(item1, item2);
    relation->setMandatory({"Обязательная","Обязательная"});
    relation->setType("Роль");
    item1->addRelation(relation);
    item2->addRelation(relation);
    relation->setZValue(-1000.0);
    this->addItem(relation);
    relation->updatePosition();

};

Entity * SceneER::addEntity(QString item, QPointF f){

    Entity *item2 = new Entity(item, keeEntity, myItemMenu);

    addItem(item2);
    item2->setPos(f);
    return item2;
}
