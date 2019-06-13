#ifndef ENTITY_H
#define ENTITY_H

#include <QGraphicsPixmapItem>
#include <QList>
#include"keetype.h"


class Relation;

class Entity : public QObject, public QGraphicsItem
{
public:

    enum { Type = UserType + 15 };
    enum EntityType {Ent};

    Entity(QString name, QVector<KeeType> kee, QMenu *menu, QGraphicsItem *parent = 0);

    void removeRelation(Relation *relation);
    void removeRelations();

    EntityType entityType() const {
        return myEntityType;
    }

    QPolygonF getEntity() const {
        return entity;
    }

    void setEntity(QPolygonF myentity){entity = myentity;}

    void addRelation(Relation *relation);
    int type() const { return Type;}

    QString nameEntity;
    int idDB;
    void setName(QString name){nameEntity = name;}

    QVector<KeeType> keeEntity;
    void changeKee(QVector<KeeType> kee){keeEntity = kee;}
    void addNewKee(QVector<KeeType> kee){keeEntity.append(kee);}
    void addNewKee(KeeType kee){keeEntity.append(kee);}

    QList<Entity*> dependents;
    QList<Entity*> roles;
    QPointF posit;



protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QRectF boundingRect() const ;
    QPainterPath shape() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) ;

private:
    EntityType myEntityType;
    QPolygonF entity;
    QMenu *myContextMenu;
    QList<Relation *> relations;




};
//! [0]

#endif // DIAGRAMITEM_H

