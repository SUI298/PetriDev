#ifndef UNDOADDPLACE_H
#define UNDOADDPLACE_H

#include <QUndoCommand>

#include "petri.h"
#include "place.h"

/*!
 * \brief AddPlaceCmd Klasse ist der rueckgaengigmachbare Befehl zum erstellen einer Stelle
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class AddPlaceCmd : public QUndoCommand
{
public:
    /*!
     * \brief AddPlaceCmd Konstruktor
     * \param petri Zeiger zum Petrinetz
     * \param pos Einfuegeposition.
     */
    AddPlaceCmd(Petri* petri, QPointF pos):
        pos(pos)
    {
        this->petri = petri;
        place = nullptr;
    }

    /*!
     * \brief redo ausfuehren des Befehls.
     * Wenn Stelle noch nicht existiert wird sie neu erstellt, sonnst nur wiederhinzugefuegt.
     */
    void redo() override
    {
        if(place == nullptr)
            place = petri->addPlace(pos);
        else
            petri->addPlace(place);
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls
     */
    void undo() override{petri->remove(dynamic_cast<QGraphicsItem*>(place)); }

private:
    Petri* petri;
    QPointF pos;
    Item::Place* place;
};

#endif // UNDOADDPLACE_H
