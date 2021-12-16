#include "transition.h"
#include "allcommands.h"
namespace Item {
    class Connector;

    int Transition::transCnt = 0;

    const QBrush Transition::activeBrush = QBrush(GREEN_FILL);
    const QBrush Transition::inactiveBrush = QBrush(RED_FILL);
    const QPen Transition::pen = QPen(QBrush(NEUTRAL_FILL) , 3);

    Transition::Transition(QPointF center):
        AbstractPT(
            center,
            QPointF(0, WIDTH/2 - 5), WIDTH/2, "t"+QString::number(++transCnt) //Beschreibungstext initialisieren
        )
    {
        setData(0, 'T'); //Damit Objekt als Transition identifiziert werden kann
    }


    bool Transition::isActivated()
    {
        for(Connector* c : incommingConnectors){ //fuer alle reinfliessenden Kanten
            if(c->active){
                if(!c->placeHasEnoughTokens()) //falls einer der reinfliessenden Stellen nicht genug Marken hat
                    return false;
            }
        }
        return true;
    }

    void Transition::fire()
    {
        if(!isActivated())return; //ignoriere wenn inaktiv.

        //Befehle zusammenfassen damit Sie in einem Schritt Rueckgangig gemacht werden
        static_cast<Petri*>(scene())->undoStack->beginMacro("fire");

        //bei Eingehenden Stellen Marken wegnehmen lassen
        for(Connector* c : incommingConnectors){
            c->removeTokens();
        }

        //bei herausgehenden Stellen Marken hinzufuegen
        for(Connector* c : outgoingConnectors){
            c->deliverTokens();
        }

        //Ende des Zusammenfassens
        static_cast<Petri*>(scene())->undoStack->endMacro();
    }

    void Transition::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        painter->setPen(pen);
        //Fuellfarbe unterschiedlich wenn Aktiv oder nicht
        painter->setBrush(isActivated() ? activeBrush : inactiveBrush);
        AbstractPT::paint(painter, option, widget);
    }

    QPainterPath Transition::shape() const
    {
        QPainterPath path;
        //Rechteck
        path.addRect(-WIDTH/2, -WIDTH/2, WIDTH, WIDTH);
        return path;
    }

    void Transition::loadTool(const Tool::PetriTool &t)
    {
        AbstractPT::loadTool(t);
        if(t == Tool::Fire)
            //Zeigefinger Maus-Cursor
            setCursor(QCursor(Qt::PointingHandCursor));
        else
            unsetCursor();
    }

    void Transition::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        //Auslösen dieser Transition
        if(currentTool == Tool::Fire)fire();
        AbstractPT::mousePressEvent(event);
    }

}
