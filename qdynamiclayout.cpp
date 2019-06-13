#include "qdynamiclayout.h"

QDynamicLayout::QDynamicLayout(QWidget *parent) :
    QHBoxLayout (parent)
{
    ResID++;
    layoutID = ResID;


}

QDynamicLayout::~QDynamicLayout()
{

}


int QDynamicLayout::getID()
{
    return layoutID;
}


int QDynamicLayout::ResID = 0;
