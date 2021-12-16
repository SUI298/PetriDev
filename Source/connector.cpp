#include "connector.h"

#include "allcommands.h"
#include <cmath>
#include <QDebug>


namespace Item {

    const QPen Connector::pen = QPen(QBrush(NEUTRAL_FILL) , 2);
    const QBrush Connector::arrowBrush = QBrush(NEUTRAL_FILL);
    const qreal Connector::arrowsize = 20;

    Connector::Connector(QPointF start, AbstractPT* tailPT) :
        QGraphicsItem(nullptr),
        active(true),
        autoReactivation(true),
        head(0,0),
        weight(this, start, 30, "1", true),
        headPT(nullptr),
        tailPT(nullptr)
    {
        setPos(start);
        loadTool(Tool::Connectror);
        setTailPT(tailPT);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setData(0, 'C');
    }


    void Connector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        //informiere Compiler ueber nicht verwendete Parameter
        (void)option; (void)widget;

        QPolygonF arrowHead;
        updatePoints(); //Punkte Aktualisieren, falls sich die Stellen/Transitionen bewegt haben.

        painter->setRenderHint(QPainter::Antialiasing); //vermeidet unshoene darstellung
        painter->setPen(pen);
        painter->drawLine(head, QPointF(0,0)); //linie von Ende bis Ursprung

        //Dreieck fuer Pfeilende
        float angle = atan2f(head.y(), -head.x() ); //Winkel der Kante
        arrowHead.clear();
        //Punkte auf beiden seiten der Spitze
        QPointF arrowP1 = head + QPointF(sin(angle + M_PI / 3) * arrowsize,
                                            cos(angle + M_PI / 3) * arrowsize);
        QPointF arrowP2 = head + QPointF(sin(angle + M_PI - M_PI / 3) * arrowsize,
                                            cos(angle + M_PI - M_PI / 3) * arrowsize);
        //Dreieck bilden
        arrowHead<<head<<arrowP1<<arrowP2;
        painter->setBrush(arrowBrush);
        painter->drawPolygon(arrowHead);
    }

    bool Connector::shouldExist()
    {
        //keine nullptr
        if((headPT)&&(tailPT)){
            //falls Urprung und Ende unterschiedliche und aktive Gegenstände darstellen
            if(headPT->getType() != tailPT->getType() && tailPT->active && headPT->active)
                return true;
        }
        return false;
    }

    uint8_t Connector::getWeight()
    {
        //text zu zahl
        int result = weight.toPlainText().toInt();

        //pruefen ob im erlaubten bereich
        if(result > 0xFF){
            weight.setText("255");
            result = 255;
        }else if(result == 0){
            result = 1;
        }

        return result;
    }

    bool  Connector::placeHasEnoughTokens()
    {
        if(tailPT){
            if(tailPT->getType()== 'P'){ //ist eine Stelle?
                return (tailPT->getTokens()>=getWeight()); //genug Marken?
            }
        }
        return false; //keine Stelle am Ursprung
    }

    void  Connector::deliverTokens()
    {
        if(headPT){
            if(headPT->getType()== 'P'){ //ist eine Stelle?
                //Marken hinzufuegen Befehl
                static_cast<Petri*>(scene())->undoStack
                        ->push(new AddTokenCmd(headPT, getWeight()));
            }
        }
    }

    void Connector::removeTokens()
    {
        if(tailPT){
            if(tailPT->getType()== 'P'){ //ist eine Stelle
                //Marken entfernen Befehl
                static_cast<Petri*>(scene())->undoStack
                        ->push(new AddTokenCmd(tailPT, -getWeight()));
            }
        }
    }

    QPainterPath Connector::shape() const
    {
        const int r = 5; //radius um Kante
        QPainterPath path;
        QPointF delta = head; //Kannte als Vektor
        if(delta.x()==0 && delta.y() == 0){
            //falls Nullvektor
            path.addEllipse(head, r, r);
        }else{
            //normalisieren des Vektors und skalieren um r
            delta *= r/sqrtf((float)delta.y()*(float)delta.y()+ (float)delta.x()*(float)delta.x());
            //delta ist nun ein vektor in richtung Kante aber mit Laenge r

            QPointF r_delta(delta.y(), -delta.x()); //eine um PI/2 rotiere version des vektors delta erstellen.
            //erstellen eines Rechtecks in Richtung der Kante, wessen Seiten r von der Kannte enfernt sind
            //etwa so: [ -------> ]
            path.moveTo(    - delta + r_delta);
            path.lineTo(    - delta - r_delta);
            path.lineTo(head+ delta - r_delta);
            path.lineTo(head+ delta + r_delta);
            path.lineTo(    - delta + r_delta);
        }
        return path;
    }

    QJsonObject Connector::toJson()
    {
        QJsonObject obj;
        obj.insert("weight", getWeight()); //gewichtung einfuegen
        if(headPT){
            QJsonObject headPos;
            headPos.insert("x", headPT->pos().x());
            headPos.insert("y", headPT->pos().y());
            obj.insert("headPos", headPos); //Kantenende einfuegen
        }
        if(headPT){
            QJsonObject tailPos;
            tailPos.insert("x", tailPT->pos().x());
            tailPos.insert("y", tailPT->pos().y());
            obj.insert("tailPos", tailPos); //Kantenursprung hinzufuegen
        }
        return obj;
    }

    void Connector::loadTool(const Tool::PetriTool &t)
    {
        currentTool = t;
    }

    void Connector::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {

        if(currentTool == Tool::Connectror){
            //beim zeichnen der Kante folgt das Kantenende der Maus bis zum loslassen
            head = event->pos();
            update();
        }
        QGraphicsItem::mouseMoveEvent(event);
    }


    void Connector::updatePoints()
    {
        if(headPT && tailPT){
            //Verbindungpunkte berechten fuer Ende und Ursprung
            setPos(tailPT->getConnectionPoint(headPT->pos()));
            head = mapFromParent(headPT->getConnectionPoint(tailPT->pos()));
        }else if(tailPT){
            //Nur fuer Ursprung berechnen anhand aktuellem Kantenende.
            QPointF parentHead = mapToParent(head); //in Szenenkoordinatensystem speichern
            setPos(tailPT->getConnectionPoint(parentHead));
            head = mapFromParent(parentHead); //wieder zurueck in Lokalkoordinaten
        }
        weight.setPos(0.5*head - QPointF(0.5*weight.textWidth(), 0)); //position der Gewichtungsangabe anpassen
    }

    void Connector::setTailPT(AbstractPT *value)
    {
        tailPT = value;
        if(tailPT != nullptr)tailPT->addConnectorOut(this); //fuege dem Gegenstand Ziger zu sich selbst hinzu
    }

    void Connector::setHeadPT(AbstractPT *value)
    {
        headPT = value;
        if(headPT != nullptr)headPT->addConnectorIn(this); //fuege dem Gegenstand Ziger zu sich selbst hinzu
    }

}
