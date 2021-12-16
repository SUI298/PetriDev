#include "place.h"

#include "allcommands.h"
namespace Item {

    const QBrush Place::filling = QBrush(NEUTRAL_FILL);
    const QBrush Place::tokenBrush = QBrush(BACKGROUND);
    const QPen Place::pen = QPen(QBrush(NEUTRAL_FILL) , 3);

    int Place::placeCnt = 0;



    Place::Place(const QPointF& center):
        AbstractPT(
            center,
            QPointF(0, WIDTH/2 - 5), WIDTH/2, "p"+QString::number(++placeCnt) //parameter fuer Beschreibungstext
        ),
        tokens(0)
    {
        setData(0, 'P'); //Damit Objekt als Stelle identifiziert werden kann
    }

    void Place::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {

        //Standart Farben setzen
        painter->setPen(pen);
        painter->setBrush(filling);
        //Basisklasse zeichnen lassen
        AbstractPT::paint(painter, option, widget);

        //Marken zeichnen
        if(tokens){
            painter->setBrush(tokenBrush); //Farbe fuer Marken laden
            int todraw = tokens; //anzahl noch zu zeichnender Marken
            //radius in der Marken kleiner, je mehr es hat.
            float r_token = 0.15*std::sqrt(WIDTH * WIDTH /(tokens));
            //Radius auf dem Marken sitzen können
            float r_circle = WIDTH/2 - 2 * r_token - pen.width();
            //wenn es nur eine oder mehr als vier Marken gibt soll eine in der Mitte gezeichnet werden
            if(todraw == 1 || todraw > 4){
                todraw--;
                painter->drawEllipse(QPointF(0,0) , r_token, r_token);
            }
            //die noch verbleibenden Marken regelmässig verteilt auf einem Kreis zeichnen.
            for(int i = 0; i < todraw; i++){
                float angle = 2*3.14159*i/todraw;
                QPointF tokenPos(r_circle*std::sin(angle), -r_circle*std::cos(angle));
                painter->drawEllipse(tokenPos, r_token, r_token);
            }
        }

    }

    QPainterPath Place::shape() const
    {
        QPainterPath path;
        //Kreis
        path.addEllipse(QPointF(0,0), WIDTH/2, WIDTH/2);
        return path;
    }

    QJsonObject Place::toJson()
    {
        //von Basisklasse
        QJsonObject obj(AbstractPT::toJson());
        //Anzahl Markierungen hinzufuegen
        obj.insert("tokens", tokens);
        return obj;
    }

    void Place::loadTool(const Tool::PetriTool &t)
    {
        AbstractPT::loadTool(t);
        if(t == Tool::Token)
            //Falls Markierungs Werkzeug soll der Zeigefinger verwendet werden.
            setCursor(QCursor(Qt::PointingHandCursor));
        else
            unsetCursor(); //kein spezieller Maus-Cursor
    }

    void Place::addTokens(int t)
    {
        if(t<0xFF-(int)tokens && -t<=(int)tokens){ //Bereichkontrolle
            tokens+=t;
        }
    }



    void Place::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        switch(event->button()){
        case Qt::LeftButton:
            if(currentTool == Tool::Token)
                //Markierung hinzufuegen Befehl
                static_cast<Petri*>(scene())->undoStack->push(new AddTokenCmd(this, 1));
            update(); //neu zeichnen anfordern
            break;
        case Qt::RightButton:
            if(currentTool == Tool::Token)
                //Markierung wegnehmen Befehl
                static_cast<Petri*>(scene())->undoStack->push(new AddTokenCmd(this, -1));
            update(); //neu zeichnen anfordern
            break;
        default:break;
        }
        AbstractPT::mousePressEvent(event);
    }

}
