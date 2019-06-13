#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>

#include "entity.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE


class Relation : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };


    Relation(Entity *van, Entity *twoo, QGraphicsItem *parent = 0);

    Relation(Entity *rec, QGraphicsItem *parent = 0);

    Relation(QVector <Entity*> rec, QGraphicsItem *parent = 0);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    Entity *first() const { return myFirst; }
    Entity *last() const { return myLast; }
    QVector <Entity*>  ItemsAll() const{return list;}

    void updatePosition();
    void setType(QString type){relationType = type;}
    void setMandatory(QVector<QString> type){mandatory = type;}
    void setRelations(QVector<QString> type){relations = type;}
    QVector<QString>  getMandatory(){return mandatory;}
    QVector<QString>  getRelations(){return relations;}
    QString relationType;
protected:

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    QVector <Entity*> list;
    Entity *myFirst;
    Entity *myLast;
    QPolygonF arrowHead;

    QVector<QString> mandatory;
    QVector<QString> relations;
};


#endif
