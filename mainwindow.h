#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"
#include "entity.h"
#include "entituwindow.h"
#include "manyrelationwindow.h"
#include "relationwindow.h"
#include "keetype.h"
#include "relationchange.h"
#include "diagramscene.h"

#include <QMainWindow>

class SceneER;

QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:

    void background(QAbstractButton *button);
    void buttonGroupClicked(int id);

    void deleteEl();
    void changeItem();
    void addDependentEntity();
    void addRoleEntity();
    void addBinaryRelationWindow();
    void addManyRelationWindow();
    void save();
    void saveFile();
    void open();


    void pointerGroupClicked(int id);

    void itemInserted(Entity *item);


    void sceneScaleChanged(const QString &scale);





    void about();

private:
    void changeEntity();
    void changeRelation();
    void addBinaryRelation();
    void addManyRelation();


    void createToolBox();


    void createActions();
    void createMenus();
    void createToolbars();

    QWidget *createCellWidget(const QString &text,
                              Entity::EntityType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    EntituWindow *windowEntity;
    relationWindow *windowRelation;
    relationChange *windowRelationChange;
    ManyRelationWindow *manyRelationWindow;

    SceneER *scene;
    QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *changeAction;
    QAction *dependentEntity;
    QAction *roleEntity;
    QAction *windowRelationBynary;
    QAction *manyWindowRelationBynary;
    QAction *saveDB;
    QAction *saveDBfile;
    QAction *openDB;



    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;
    QMenu *relationMenu;
    QMenu *addMenu;


    QToolBar *editToolBar;

    QToolBar *toolbar1;
    QToolBar *toolbar2;

    QComboBox *sceneScaleCombo;


    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

};
//! [0]

#endif // MAINWINDOW_H
