QT += widgets	

QT += core gui sql

HEADERS	    =   mainwindow.h \
                entity.h \
                diagramscene.h \
                relation.h \
                entituwindow.h \
                keetype.h \
                manyrelationwindow.h \
                qdynamicbutton.h \
                qdynamiclayout.h \
                relationchange.h \
                relationwindow.h
SOURCES	    =   mainwindow.cpp \
                entity.cpp \
                entituwindow.cpp \
                keetype.cpp \
                main.cpp \
                relation.cpp \
                diagramscene.cpp \
                manyrelationwindow.cpp \
                qdynamicbutton.cpp \
                qdynamiclayout.cpp \
                relationchange.cpp \
                relationwindow.cpp
RESOURCES   =	diagramscene.qrc


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/graphicsview/diagramscene
INSTALLS += target

FORMS += \
    entituwindow.ui \
    relationchange.ui \
    relationwindow.ui
