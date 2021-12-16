#ifndef DELETEITEMCMD_H
#define DELETEITEMCMD_H

#include <QUndoCommand>
#include "petri.h"

/*!
 * \brief DeleteItemCmd Klasse fuer rueckgaengigmachbaren Befehl zum entfernen eines Elementes aus dem Petrinetz.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class DeleteItemCmd : public QUndoCommand
{
public:
    /*!
     * \brief DeleteItemCmd Konstruktor.
     * \param petri Zeiger auf das Petrinetz
     * \param item Zeiger auf das Objekt
     */
    DeleteItemCmd(Petri* petri, QGraphicsItem* item)
    {
        this->petri = petri;
        this->item = item;
    }

    /*!
     * \brief redo ausfuehren des Befehls.
     */
    void redo()override
    {
        petri->remove(item);
        if(item->data(0).toChar() == 'C')
            //kein automatisches wiederbeleben der Kante, da sie mit absicht geloescht wird.
            static_cast<Item::Connector*>(item)->autoReactivation = false;
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo()override
    {
        switch (item->data(0).toChar().toLatin1()) {
        case 'P': //eine Stelle
            petri->addPlace(static_cast<Item::Place*>(item));
            break;
        case 'T': //eine Transition
            petri->addTransition(static_cast<Item::Transition*>(item));
            break;
        case 'C': //eine Kante
            petri->addConnector(static_cast<Item::Connector*>(item)); //automatisches wiederbeleben wird wieder aktiviert.
            break;
        }
    }

private:
    Petri* petri;
    QGraphicsItem* item;
};

#endif // DELETEITEMCMD_H
