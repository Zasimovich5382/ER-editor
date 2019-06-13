#include "entity.h"
#include "relation.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>


Entity::Entity(QString name,
               QVector<KeeType> kee,
               QMenu *menu,
               QGraphicsItem *parent): QGraphicsItem(parent)
{
    myContextMenu = menu;
    nameEntity = name;
    keeEntity = kee;
    entity << QPointF(-30, -10) << QPointF(130, -10) << QPointF(130, 130) << QPointF(-30, 130) << QPointF(-30, -10);

    setEntity(entity);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


void Entity::removeRelation(Relation *arrow)
{
    int index = relations.indexOf(arrow);

    if (index != -1)
        relations.removeAt(index);
}

void Entity::removeRelations()
{
    foreach (Relation *relation, relations) {
        for(int i = 0; i<relation->ItemsAll().length();i++){
            relation->ItemsAll().at(i)->removeRelation(relation);
        }

        scene()->removeItem(relation);
        delete relation;
    }
}



QRectF Entity::boundingRect() const
{

    return QRectF (-30,-10,160,140);
}

QPainterPath Entity::shape() const
{
    QPainterPath path;
    path.addRect(-30,-10,160,140);
    return path;
}

void Entity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::black);
    painter->setBrush(Qt::yellow);
    int j = 30;
    for(int i = 0; i < keeEntity.length();i++){
        j=j+20;
    }
    painter->drawRect(-30,-10,160,j+70);

    painter->setFont(QFont("Arial", 8, QFont::Normal));

    painter->drawText(-20, 30, "Имя: "+nameEntity);
    painter->drawLine(-30, 50, 130, 50);
    j = 70;
    for(int i = 0; i < keeEntity.length();i++){
        KeeType kees =  keeEntity.at(i);
        painter->drawText(-20, j,kees.getName());
        j=j+20;
    }

}

void Entity::addRelation(Relation *relation)
{
    relations.append(relation);
}

void Entity::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
//! [5]

//! [6]
QVariant Entity::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Relation *arrow, relations) {
            arrow->updatePosition();
        }
    }

    return value;
}
//! [6]
