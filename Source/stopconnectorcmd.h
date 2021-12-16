#ifndef STOPCONNECTORCMD_H
#define STOPCONNECTORCMD_H


#include "petri.h"
#include "connector.h"
class Petri;

/*!
 * \brief StopConnectorCmd Klasse ist der rueckgaengigmachbare Befehl zum fertigstellen einer Kante.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class StopConnectorCmd : public QUndoCommand
{
public:
    /*!
     * \brief StopConnectorCmd Konstruktor.
     * \param petri Zeiger auf das Petrinetz
     * \param pos Einfuegeposition.
     */
    StopConnectorCmd(Petri * petri, QPointF pos)
    {
        this->petri = petri;
        this->pos = pos;
        this->c = nullptr;
    }

    /*!
     * \brief redo ausfuehren des Befehls.
     * Bei der ersren Durchfuerung wird das zeichnen einer Kante nur beendet.
     * Biem erneuten ausfuehren wird das gesamte zeichnen der Kante durchgefuert.
     */
    void redo()override
    {
        if(c == nullptr){
            c = petri->stopConnector(pos);
            shouldExist = c->shouldExist();
            if(!shouldExist){ //wenn existenz der Kante nicht berechtigt
                petri->remove(c); //Kante entfernen
                setObsolete(true);//Befehl ist obsolet
            }
        }else
            petri->addConnector(c); //wiedereinfuegen

    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo()override
    {
        if(shouldExist)petri->remove(c);
    }

private:
    Petri* petri;
    Item::Connector* c;
    QPointF pos;
    bool shouldExist;
};

#endif // STOPCONNECTORCMD_H
