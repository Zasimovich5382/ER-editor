#ifndef SCENEER_H
#define SCENEER_H

#include "entity.h"
#include "keetype.h"
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class SceneER : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Obj { InsertEntity, MoveItem, ManyToMany, OneToMany,ManyToOne,OneToOne };

    explicit SceneER(QMenu *itemMenu, QObject *parent = 0);

    void addRelationBynary(Entity *item1, Entity *item2, QVector<QString>mandatory, QString degree);
    void addRelationMany(QVector <Entity*> vector, QVector<QString>type, QVector<QString>mandatory);
    void addDependent(Entity * item, QPointF f);
    void addRole(Entity * item, QPointF f);
    Entity * addEntity(QString item, QPointF f);

public slots:
    void setObj(Obj obj);
    void setNameEntity(QString name);
    void setKeeEntity(QVector<KeeType> kee);
    void setItemType(Entity::EntityType type);


signals:
    void itemInserted(Entity *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:


    Entity::EntityType myItemType;
    QMenu *myItemMenu;
    Obj myObj;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;

    QString nameEntity;
    QVector<KeeType> keeEntity;
    QVector<QString>man;
    QVector<QString>deg;
};
//! [0]

#endif // SceneER_H
