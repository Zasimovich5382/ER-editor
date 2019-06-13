#include "keetype.h"

KeeType::KeeType()
{

}
KeeType::KeeType(QString myName, QString myType, int mySize)
{
    name = myName;
    type = myType;
    size = mySize;
}
QString KeeType::getName(){
    return name;
};
QString KeeType::getType(){
    return type;
};
int KeeType::getSize(){
    return size;
};

void KeeType::setName(QString myName){
    name = myName;
};
void KeeType::setType(QString myType){
    type = myType;
};
void KeeType::setSize(int mySize){
    size = mySize;
};
