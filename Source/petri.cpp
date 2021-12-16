#include "petri.h"
#include "allcommands.h"
#include <QTimer>
#include <QJsonArray>
#include <QGraphicsView>

int Petri::cnt = 0;

Petri::Petri(const QRect& initialSize):
    name("new" + QString::number(++cnt)) //setze dateiname
{
    //emoegliche effizientes enfernen von ojekten auf der Szene.
    setItemIndexMethod(QGraphicsScene::ItemIndexMethod::NoIndex);
    setBackgroundBrush(QBrush(BACKGROUND)); //hintergrundfarbe
    QGraphicsItem* initialRect = addRect(QRectF(initialSize));  //Rechteck setzt die Minimalgoesse der Szene
    remove(initialRect);
    QTimer::singleShot(1, this, [=](){delete initialRect;});    //spaeter loeschen

    //setSceneRect(QRect());
    undoStack = new QUndoStack(this);
    //bei Aenderungen an der Datei "*" dem namen anfuegen
    connect(undoStack, &QUndoStack::cleanChanged, this, &Petri::documentNotSaved);

}

Petri::Petri(const QJsonObject &json, const QString &path):
    Petri() //leeres Petrinetz
{
    setPath(path);
    name.setValue(path); //name wie pfad.

    //JSON-Arrays
    QJsonArray places = json["places"].toArray();
    QJsonArray transitions = json["transitions"].toArray();
    QJsonArray connectors = json["connectors"].toArray();

    //Stellen hinzufuegen
    for(int i = 0; i< places.size(); i++){
        Item::Place* p = addPlace(pos(places[i].toObject()["pos"].toObject()));
        p->setDesignator(places[i].toObject()["designator"].toString());
        p->addTokens(places[i].toObject()["tokens"].toInt());
    }
    //Transitionen hinzufuegen
    for(int i = 0; i< transitions.size(); i++){
        Item::Transition* t = addTransition(pos(transitions[i].toObject()["pos"].toObject()));
        t->setDesignator(transitions[i].toObject()["designator"].toString());
    }
    //Kanten hinzufuegen
    for(int i = 0; i< connectors.size(); i++){
        Item::Connector* c = startConnector(pos(connectors[i].toObject()["tailPos"].toObject()));
        stopConnector(pos(connectors[i].toObject()["headPos"].toObject()));
        c->setWeight(connectors[i].toObject()["weight"].toInt());
    }
}

Petri::~Petri()
{
    //alles loeschen
    delete undoStack;
    for(auto& item : allItems){
        delete item;
    }
}

void Petri::loadTool(Tool::PetriTool t)
{
    for(auto& p : places)p->loadTool(t);
    for(auto& tr : transitions)tr->loadTool(t);
    for(auto& c : connectors)c->loadTool(t);
}

Item::Place* Petri::addPlace(const QPointF &pos)
{
    Item::Place* p = new Item::Place(pos); //neu
    allItems.append(p);
    addPlace(p);
    return p;
}

Item::Transition* Petri::addTransition(const QPointF &pos)
{
    Item::Transition* t = new Item::Transition(pos); //neu
    allItems.append(t);
    addTransition(t);
    return t;
}

void Petri::addPlace(Item::Place *p)
{
    places.append(p);
    addItem(p);
    p->active = true; //aktiviere
    //fuege allenfalls Vorhandene Kanten hinzu, falls diese nicht selbst entfernt wurden
    for(Item::Connector* c : p->incommingConnectors)
        if(c->autoReactivation)addConnector(c);
    for(Item::Connector* c : p->outgoingConnectors)
        if(c->autoReactivation)addConnector(c);
}

void Petri::addTransition(Item::Transition * t)
{
    addItem(t);
    transitions.append(t);
    t->active = true; //aktiviere
    //fuege allenfalls Vorhandene Kanten hinzu, falls diese nicht selbst entfernt wurden
    for(Item::Connector* c : t->incommingConnectors)
        if(c->autoReactivation)addConnector(c);
    for(Item::Connector* c : t->outgoingConnectors)
        if(c->autoReactivation)addConnector(c);
}

void Petri::addConnector(Item::Connector* c)
{
    if(c->shouldExist()){
        addItem(c);
        connectors.append(c);
        //flags setzen
        c->active = true;
        c->autoReactivation = true;
    }
}

QJsonObject Petri::toJson()
{
    QJsonObject obj;

    //JSON arrays
    QJsonArray places;
    QJsonArray transitions;
    QJsonArray connectors;

    for(Item::Place* p : this->places)
        places.push_back(p->toJson()); //alle Stellen einguegen
    for(Item::Transition* t : this->transitions)
        transitions.push_back(t->toJson()); //alle Transitionen einfuegen
    for(Item::Connector* c : this->connectors)
        connectors.push_back(c->toJson()); //alle Kanten einfuegen

    //Arrays in Objekt einfuegen.
    obj.insert("places", places);
    obj.insert("transitions", transitions);
    obj.insert("connectors", connectors);

    return obj;
}

Item::Connector* Petri::startConnector(const QPointF &pos)
{
    Item::AbstractPT* itemPT = nullptr;

    for(auto& item : items(pos)){
        //finde ob es eine Stelle oder Transition an dieser Position gibt
        if((transitions.contains((Item::Transition*)item)|| places.contains((Item::Place*)item))){
            itemPT = static_cast<Item::AbstractPT*>(item);
            break;
        }
    }
    Item::Connector * c = new Item::Connector(pos, itemPT); //neue Kannte mit Ursprungsgegenstand itemPT.
    allItems.append(c);
    connectors.append(c);
    addItem(c);
    return c;
}

Item::Connector* Petri::stopConnector(const QPointF &pos)
{
    Item::AbstractPT* itemPT = nullptr;
    for(auto& item : items(pos)){
         //finde ob es eine Stelle oder Transition an dieser Position gibt
        if((transitions.contains((Item::Transition*)item)|| places.contains((Item::Place*)item))){
            itemPT = (Item::AbstractPT*)item;
            break;
        }
    }
    if(itemPT && connectors.size())
        connectors.back()->setHeadPT(itemPT); //setze Kantenende auf gefundenes Objekt

    return connectors.back();
}

bool Petri::remove(const QPointF &pos)
{

    if(this->items(pos).size()){
        undoStack->beginMacro("delete items"); //befehle zusammenfassen
        for(auto& item : items(pos)){ //finde alle Gegenstaende
            undoStack->push(new DeleteItemCmd(this, item));
        }
        undoStack->endMacro();
        return true;    //geloescht
    }return false;      //nichts zu loeschen

}

void Petri::remove(QGraphicsItem *item)
{
    switch(item->data(0).toChar().toLatin1()) {
    case 'P':   //eine Stelle
        places.removeAll(static_cast<Item::Place*>(item)); //Enferne Stelle
        //Enferne assozierte Kanten
        for(Item::Connector* c : static_cast<Item::Place*>(item)->incommingConnectors)
            remove(c);
        for(Item::Connector* c : static_cast<Item::Place*>(item)->outgoingConnectors)
            remove(c);
        static_cast<Item::Place*>(item)->active = false;
        break;
    case 'T':   //eine Transition
        transitions.removeAll(static_cast<Item::Transition*>(item));
        //Enferne assozierte Kanten
        for(Item::Connector* c : static_cast<Item::Transition*>(item)->incommingConnectors)
            remove(c);
        for(Item::Connector* c : static_cast<Item::Transition*>(item)->outgoingConnectors)
            remove(c);
        static_cast<Item::Transition*>(item)->active = false;
        break;
    case 'C':   //eine Kante
        //Enferne assozierte Kanten
        connectors.removeAll(static_cast<Item::Connector*>(item));
        static_cast<Item::Connector*>(item)->active = false;
        break;
    }



    //removeItem sollte nicht auserhalb des haupteventloops verwendet werden. Mit einem Timer und lambda wird dies sichergestellt
    //https://forum.qt.io/topic/87303/crash-in-qgraphicsscene-event-when-removing-and-deleting-items-that-use-sceneeventfilters/4
    QTimer::singleShot(0,this,[=]{removeItem(item);});

}

QString Petri::getPath() const
{
    return path;
}

void Petri::setPath(const QString &value)
{
    path = value;
}


inline QPointF Petri::pos(QJsonObject pos)
{
    return QPointF(pos["x"].toDouble(), pos["y"].toDouble());
}

void Petri::documentNotSaved(bool undo_clean)
{
    if(undo_clean){
        name.setValue(name.getValue().remove("*")); //entferne *
    }else{
        if(name.getValue()[0] != '*'){
            name.setValue(name.getValue().prepend('*')); //* einfuegen
        }
    }
}





