#include "abstractpt.h"
#include "allcommands.h"

namespace Item {

    AbstractPT::AbstractPT(const QPointF &center, const QPointF &d_topcenter, int d_width, const QString &d_txt):
        //alle Teilobjekte initialisieren
        QGraphicsItem(nullptr),
        active(true),
        designator(this, d_topcenter, d_width, d_txt),
        currentTool(Tool::Select)
    {
        setPos(center); //Position setzen

        //der Gegenstand kann nur bewegt werden wenn ausählbar.
        //beim Erstellen und gedrückthalten der Maus soll der Gegenstand der Maus folgen.
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    void AbstractPT::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        //dem Compiler unbenutzte Parameter Mitteilen.
        (void)option; (void)widget;

        for(auto& c : incommingConnectors)
            c->update();
        for(auto& c : outgoingConnectors)
            c->update();

        //fals ausgewählt, mit gestricheler Linie darstellen. Sonst bereits geladenen Stift verwenden.
        if(isSelected())painter->setPen(Qt::PenStyle::DashLine);

        painter->setRenderHint(QPainter::Antialiasing); //vermeidet unschoene Kanten.
        painter->drawPath(shape()); //zeichen.
    }

    QJsonObject AbstractPT::toJson()
    {
        QJsonObject obj;
        QJsonObject pos;
        //Positions-JSON-objekt erstellen
        pos.insert("x", this->pos().x());
        pos.insert("y", this->pos().y());
        //dem Haubtobjekt hizufuegen
        obj.insert("pos", pos);
        //Bezeichnung hinzufuegen
        obj.insert("designator", designator.toPlainText());
        //Fertiges Objekt zurueckgeben.
        return obj;
    }

    void AbstractPT::loadTool(const Tool::PetriTool &t)
    {
        currentTool= t; //lade Werkzeug
        //Beim Auswahlwerkzeug auswaelbar machen.
        if(t == Tool::Select)
            setFlag(QGraphicsItem::ItemIsSelectable, true);
        else
            setFlag(QGraphicsItem::ItemIsSelectable, false);
    }


    QPointF AbstractPT::getConnectionPoint(const QPointF &other)const
    {
        //distanzen in x und y Richtung berechnen.
        qreal dx = other.x()-pos().x();
        qreal dy = other.y()-pos().y();
        QPointF p(pos().x(), pos().y());


        if(abs(dy) > abs(dx)){ //welche distanz ist groesser?
            //Berechnungsmethode: x = f(y)
            qreal pitch = dx/dy; //Steigung zur y-Achse
            //Vorzeichenbedingte for-Schleife von y=0 bis y = Rand der Form
            for(qreal y = 0; (dy>0)?(y < dy):(y>dy);(dy>0)?(y++):(y--)){
                p.setY(pos().y()+y);
                p.setX(pos().x()+pitch*y);
                if(!shape().contains(mapFromParent(p)))//Punkt ist ausserhalb der Form des Gegenstandes
                    return p;
            }
        }else{
            //Berechnungsmethode: y = f(x)
            qreal pitch = dy/dx; //Steigung zur x-Achse
            //Vorzeichenbedingte for-Schleife von x=0 bis x = Rand der Form
            for(qreal x = 0; (dx>0)?(x < dx):(x>dx);(dx>0)?(x++):(x--)){
                p.setX(pos().x()+x);
                p.setY(pos().y()+pitch*x);
                if(!shape().contains(mapFromParent(p)))//Punkt ist ausserhalb der Form des Gegenstandes
                    return p;
            }
        }

        return p;

    }

    QPointF AbstractPT::getCenter() const
    {
        return pos(); //Mitte der Form
    }

    void AbstractPT::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        QGraphicsItem::mouseMoveEvent(event);
        switch (currentTool) {
        case Tool::Select:
        case Tool::Place:
        case Tool::Transition:
            if(isSelected()){
                //Verschieben aller ausgewählten Gegenstände.
                static_cast<Petri*>(scene())->undoStack->push(new MoveCmd(
                    scene()->selectedItems(),
                    event->pos()-event->buttonDownPos(Qt::LeftButton)
                )); //Befehl in den undoStack einfuegen
            }
            break;
        default:
            break;
        }
    }

}
