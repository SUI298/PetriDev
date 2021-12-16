#include "annotation.h"
#include <QDebug>
#include "allcommands.h"

//Makro zum formatieren fon text mittels HTML
//Text zentriert, Groesse 13pt, Farbe Elfenbein
#define HTML_ALIGN_CENTER(txt) (                                                \
                    "<!DOCTYPE html>"                                           \
                    "<html>"                                                    \
                    "   <head></head>"                                          \
                    "   <body style=\"  font-size:13pt; \"> \n"                 \
                    "       <p style=\" text-align:center; color: #ECDBBA \">"  \
                    +           txt                                         +   \
                    "       </p>"                                               \
                    "   </body>"                                                \
                    "</html>")


Annotation::Annotation(QGraphicsItem *parent, const QPointF &topcenter, int width, const QString &txt, bool numbersOnly):
   QGraphicsTextItem(parent),
   numbersOnly(numbersOnly)
{
    setTextWidth(width); //breite
    setText(txt);        //Text
    setPos(topcenter.x()- width/2, topcenter.y()); //Position
    //Textcursor funktionalität aktivieren
    setTextInteractionFlags(	Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard );
}


void Annotation::setText(const QString &txt)
{
    setHtml( HTML_ALIGN_CENTER( txt ) ); //formatieren und setzen.
}

void Annotation::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
    bool isNum;
    event->text().toInt(&isNum); //speichern ob geschriebener Text eine Zahl ist

    if(event->key()>=Qt::Key_Space && event->key()<=Qt::Key_ydiaeresis &&    //valides zeichen, nicht ein befehl
          (!numbersOnly||isNum) //Zeichen is nummer ODER alle zeichen werden akzeptiert
       ){
        //Zeichen einfuegen
        static_cast<Petri*>(scene())->undoStack->push(new AppendTextCmd(textCursor(), event->text()));
    }else if(event->key()== Qt::Key_Delete){        //Entfernen Taste
        static_cast<Petri*>(scene())->undoStack->push(new DeleteTextCmd(textCursor(), false));
    }else if(event->key()== Qt::Key_Backspace){     //Loeschen Taste
        static_cast<Petri*>(scene())->undoStack->push(new DeleteTextCmd(textCursor(), true));
    }
}



