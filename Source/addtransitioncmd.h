#ifndef ADDTRANSITIONCMD_H
#define ADDTRANSITIONCMD_H

#include <QUndoCommand>
#include "petri.h"
#include "transition.h"

/*!
 * \brief AddTransitionCmd Klasse ist der rueckgangigmachbare Befehl zum hinzufuegen einer Transition.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class AddTransitionCmd : public QUndoCommand
{
public:
    /*!
     * \brief AddTransitionCmd Konstruktor.
     * \param petri Zeiger auf Petrinetz.
     * \param pos Position zu einfuegen.
     */
    AddTransitionCmd(Petri* petri, QPointF pos):
        pos(pos)
    {
        this->petri = petri;
        transition = nullptr;
    }

    /*!
     * \brief redo ausfuehren des Befehls.
     * Wenn Transition noch nicht existiert wird sie neu erstellt, sonnst nur wiederhinzugefuegt.
     */
    void redo() override
    {
        if(transition==nullptr)
            transition = petri->addTransition(pos);
        else
            petri->addTransition(transition);
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo() override
    {
        petri->remove(dynamic_cast<QGraphicsItem*>(transition));
    }


private:
    Petri* petri;
    QPointF pos;
    Item::Transition* transition;
};

#endif // ADDTRANSITIONCMD_H
