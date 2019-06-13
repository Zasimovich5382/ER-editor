#include "relation.h"

#include <qmath.h>
#include <QPen>
#include <QPainter>

Relation::Relation(Entity *first, Entity *last, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    myFirst = first;
    myLast = last;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    relationType = "1 : 1";
    mandatory = {"Обязательная","Обязательная"};
    list.push_back(first);
    list.push_back(last);
}

Relation::Relation(Entity *rec, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    list.push_back(rec);
}

Relation::Relation(QVector <Entity*> rec, QGraphicsItem *parent): QGraphicsLineItem(parent){
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    for(int i =0; i<rec.length();i++){
        list.append(rec.at(i));
    }
    myFirst = rec.first();
    myLast = rec.last();
}


QRectF Relation::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
//! [1]

//! [2]
QPainterPath Relation::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}
//! [2]

//! [3]
void Relation::updatePosition()
{
    QLineF line(mapFromItem(myFirst, 0, 0), mapFromItem(myLast, 0, 0));
    setLine(line);
}

void Relation::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    QPen myPen = pen();
    myPen.setColor(Qt::black);
    myPen.setStyle(Qt::SolidLine);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(Qt::black);

    if(list.length() == 7){
        if (myFirst->collidesWithItem(myLast))
        return;
        QLineF centerLine(myFirst->pos(), myLast->pos());
        QPolygonF endPolygon = myLast->getEntity();
        QPointF p1 = endPolygon.first() + myLast->pos();
        QPointF p2;
        QPointF intersectPoint;
        QLineF polyLine;
        for (int i = 1; i < endPolygon.count(); ++i) {
            p2 = endPolygon.at(i) + myLast->pos();
            polyLine = QLineF(p1, p2);
            QLineF::IntersectType intersectType =
                polyLine.intersect(centerLine, &intersectPoint);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p1 = p2;
        }

        if (myLast->collidesWithItem(myFirst))
            return;
        QLineF centerLine2(myLast->pos(), myFirst->pos());
        QPolygonF endPolygon2 = myFirst->getEntity();
        QPointF p3 = endPolygon2.first() + myFirst->pos();
        QPointF p4;
        QPointF intersectPoint2;
        QLineF polyLine2;
        for (int i = 1; i < endPolygon2.count(); ++i) {
            p4 = endPolygon2.at(i) + myFirst->pos();
            polyLine2 = QLineF(p3, p4);
            QLineF::IntersectType intersectType =
                polyLine2.intersect(centerLine2, &intersectPoint2);
            if (intersectType == QLineF::BoundedIntersection)
                break;
            p3 = p4;
        }

        setLine(QLineF(intersectPoint, intersectPoint2));


        double angle = std::atan2(-line().dy(), line().dx());
        /////////////////////////////////////////////////////////////////////////////
        QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                    cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                    cos(angle + M_PI - M_PI / 3) * arrowSize);
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        QPointF arrowP1MNCenter = QPointF((arrowP1.rx()+arrowP2.rx())/2,
                                      (arrowP1.ry()+arrowP2.ry())/2);

        QPointF arrowP1MN = arrowP1MNCenter + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                              cos(angle - M_PI / 3) * arrowSize);
        QPointF arrowP2MN = arrowP1MNCenter + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                              cos(angle - M_PI + M_PI / 3) * arrowSize);

        /////////////////////////////////////////////////////////////////////////////////////////////////////
        QPointF arrowSide2_1 = line().p2() + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                    cos(angle - M_PI / 3) * arrowSize);
        QPointF arrowSide2_2 = line().p2() + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                    cos(angle - M_PI + M_PI / 3) * arrowSize);
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        QPointF arrowSide2Center = QPointF((arrowSide2_1.rx()+arrowSide2_2.rx())/2,
                                          (arrowSide2_1.ry()+arrowSide2_2.ry())/2);

        QPointF arrowP1MN_2 = arrowSide2Center + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                         cos(angle + M_PI / 3) * arrowSize);
        QPointF arrowP2MN_2 = arrowSide2Center + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                         cos(angle + M_PI - M_PI / 3) * arrowSize);
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        arrowHead.clear();
        if(relationType == "n : 1") {
            arrowHead << arrowSide2Center << arrowP1MN_2 << arrowSide2Center << arrowP2MN_2;
            painter->drawPolygon(arrowHead);
        }
        else if(relationType == "1 : n") {
            arrowHead   << arrowP1MNCenter << arrowP2MN << arrowP1MNCenter <<arrowP1MN;
            painter->drawPolygon(arrowHead);
        }
        else if(relationType == "m : n") {
            arrowHead << arrowSide2Center << arrowP1MN_2 << arrowSide2Center << arrowP2MN_2;
            painter->drawPolygon(arrowHead);
            arrowHead.clear();
            arrowHead   << arrowP1MNCenter << arrowP2MN << arrowP1MNCenter <<arrowP1MN;
            painter->drawPolygon(arrowHead);
        }
        else if(relationType == "Зависимая сущность") {

            arrowHead << line().p1() << arrowP1 << arrowP2;
            painter->drawPolygon(arrowHead);
        }
        else if(relationType == "Роль") {
            myPen.setColor(Qt::green);
            painter->setPen(myPen);
            painter->setBrush(QBrush(Qt::green));
            arrowHead << line().p1() << arrowP1 << arrowP2;
            painter->drawPolygon(arrowHead);
        }
        myPen.setColor(Qt::black);
        painter->setPen(myPen);
        painter->setBrush(Qt::black);

        QPointF centerPoint = QPointF((line().p1().rx()+line().p2().rx())/2,
                                      ((line().p1().ry()+line().p2().ry())/2));

        if(relationType == "Роль") {
            myPen.setColor(Qt::green);
            painter->setPen(myPen);
            painter->setBrush(QBrush(Qt::green));
        }
        if(mandatory.first() == "Обязательная") myPen.setStyle(Qt::SolidLine);
        else myPen.setStyle(Qt::DashLine);
        painter->drawLine(QLineF(line().p1(), centerPoint));
        painter->setPen(myPen);

        if(mandatory.last() == "Обязательная") myPen.setStyle(Qt::SolidLine);
        else myPen.setStyle(Qt::DashLine);
        painter->drawLine(QLineF(line().p2(), centerPoint));


        if (isSelected()) {
            painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }
    }else  {

        double summX = 0;
        double summY = 0;
        for(int i=0; i<list.length(); i++){
            summX = summX + list.at(i)->pos().rx();
            summY = summY + list.at(i)->pos().ry();
        }
        QPointF centerPoint = QPointF(summX/list.length(),
                                      summY/list.length());

        for(int i=0; i<list.length(); i++){
            myLast = list.at(i);
            QLineF centerLine(centerPoint, myLast->pos());
            QPolygonF endPolygon = myLast->getEntity();
            QPointF p1 = endPolygon.first() + myLast->pos();
            QPointF p2;
            QPointF intersectPoint;
            QLineF polyLine;
            for (int i = 1; i < endPolygon.count(); ++i) {
                p2 = endPolygon.at(i) + myLast->pos();
                polyLine = QLineF(p1, p2);
                QLineF::IntersectType intersectType =
                    polyLine.intersect(centerLine, &intersectPoint);
                if (intersectType == QLineF::BoundedIntersection)
                    break;
                p1 = p2;
            }

            setLine(QLineF(intersectPoint, centerPoint));
            //setLine(QLineF(list.at(i)->pos(), centerPoint));



            double angle = std::atan2(-line().dy(), line().dx());
            /////////////////////////////////////////////////////////////////////////////
            QPointF arrowP1 = line().p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                    cos(angle + M_PI / 3) * arrowSize);
            QPointF arrowP2 = line().p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                                    cos(angle + M_PI - M_PI / 3) * arrowSize);
            ////////////////////////////////////////////////////////////////////////////////////////////////////
            QPointF arrowP1MNCenter = QPointF((arrowP1.rx()+arrowP2.rx())/2,
                                              (arrowP1.ry()+arrowP2.ry())/2);

            QPointF arrowP1MN = arrowP1MNCenter + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                          cos(angle - M_PI / 3) * arrowSize);
            QPointF arrowP2MN = arrowP1MNCenter + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                          cos(angle - M_PI + M_PI / 3) * arrowSize);



            if(mandatory.at(i) == "Обязательная") myPen.setStyle(Qt::SolidLine);
            else myPen.setStyle(Qt::DashLine);
            painter->setPen(myPen);
            setLine(QLineF( centerPoint, list.at(i)->pos()));
            painter->drawLine(line());

            arrowHead.clear();
            if(relations.at(i) == "Множественная"){
                arrowHead   << arrowP1MNCenter << arrowP2MN << arrowP1MNCenter <<arrowP1MN;
                painter->drawPolygon(arrowHead);

            }
            if (isSelected()) {
                painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
                QLineF myLine = line();
                myLine.translate(0, 4.0);
                painter->drawLine(myLine);
                myLine.translate(0,-8.0);
                painter->drawLine(myLine);
            }
        }




    }
}



