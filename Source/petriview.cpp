#include "petriview.h"

#include "allcommands.h"

PetriView::PetriView(QWidget* parent) :
    QGraphicsView(parent),
    currentTool(Tool::NO_TOOL)
{
    petri = nullptr;
//    petri = new Petri(frameRect()); //ein blankes Petrinetz initialisieren
//    openPetris.append(petri);
//    setScene(petri);

    //Auswahl mehrerer Objekte mit maus ermoeglichen
    setDragMode(QGraphicsView::RubberBandDrag);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    //zoom initialisieren
    connect(&zoom, &ZoomObject::changed, this, [=](){this->setTransform(zoom, false);});
}

void PetriView::loadTool(const Tool::PetriTool &t)
{
    currentTool = t;
    for(Petri* p : openPetris){
        p->loadTool(t);
    }
    //Mauscursor setzen, je nach Werkzeug
    switch (t) {
    case Tool::Place:
    case Tool::Transition:
    case Tool::Connectror:
        setCursor(QCursor(Qt::CrossCursor));
        break;
    default:
        setCursor(QCursor(Qt::ArrowCursor));
        break;
    }
}



void PetriView::mousePressEvent(QMouseEvent *event)
{
    //click
    if(scene()==nullptr)return; //ignorieren

    switch(event->button()){
    case Qt::LeftButton:
        switch (currentTool) {
        case Tool::Place: //neue Stelle einfuegen
            petri->undoStack->push(new AddPlaceCmd(petri, mapToScene(event->pos())));
            break;
        case Tool::Transition: //neue Transition einfuegen
            petri->undoStack->push(new AddTransitionCmd(petri, mapToScene(event->pos())));
            break;
        case Tool::Connectror: //mit dem zeichnen einer Kante beginnen
            petri->startConnector(mapToScene(event->pos()));
            break;

        default: //nichts tun
            break;
        }
        break;
    case Qt::RightButton: //nichts tun

        break;
    default:break;
    }
    //event an Basisklasse weiterleiten
    QGraphicsView::mousePressEvent( event );
}



void PetriView::mouseReleaseEvent(QMouseEvent *event)
{

    if(scene()==nullptr)return; //ignorieren

    //event an Basisklasse weiterleiten
    QGraphicsView::mouseReleaseEvent( event );

    switch(event->button()){
    case Qt::LeftButton:
        switch (currentTool) {

        case Tool::Delete: //Loeschen von Elementen an aktueller Mausposition
            petri->remove(mapToScene(event->pos()));
            break;
        case Tool::Connectror: //Das zeichnen der Kante beenden
            petri->undoStack->push(new StopConnectorCmd(petri, mapToScene(event->pos())));
            break;
        default: //nichts tun
            break;
        }
        break;
    case Qt::RightButton: //nichts tun
        break;
    default:break;
    }

}

void PetriView::wheelEvent(QWheelEvent *event)
{
    if(scene()==nullptr)return; //ignorieren

    if(keymods&Qt::ControlModifier){ //STRG gedrueckt
        //zoomen
        event->accept();
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        // (101%)^(anz. Mausradclicks (hat vorzeichen))
        // etwa +/- 1% pro 15° rotation
        zoom.relativeZoom(pow(1.01,event->angleDelta().y()/15));
    }else{
        //wenn nicht gezoomt wird soll die Basisklasse das scrolen hanhaben
        QGraphicsView::wheelEvent(event);
    }

}



