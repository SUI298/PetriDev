#ifndef ADDTOKENCMD_H
#define ADDTOKENCMD_H

#include <QUndoCommand>
#include "petri.h"

/*!
 * \brief AddTokenCmd Klasse ist der rueckgaengigmachbare Befehl zum hinzufuegen von Marken.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class AddTokenCmd : public QUndoCommand
{
public:
    /*!
     * \brief AddTokenCmd Konstruktor.
     * \param place zeiger aus Stelle
     * \param n anzahl
     */
    AddTokenCmd(Item::AbstractPT* place, int n)
    {
        this->place = place;
        this->n = n;
    }

    /*!
     * \brief redo ausfuehren des Befehls.
     */
    void redo()override
    {
        place->addTokens(n);
        place->update();
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo()override
    {
        place->addTokens(-n);
        place->update();
    }

private:
    Item::AbstractPT* place;
    int n;
};

#endif // ADDTOKENCMD_H
