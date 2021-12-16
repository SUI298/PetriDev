#ifndef MOVECMD_H
#define MOVECMD_H

#include <QUndoCommand>
#include <QGraphicsItem>
#include "petri.h"

/*!
 * \brief MoveCmd Klasse ist der rueckgaengigmachbare Befehl zum bewegen eines oder mererrer Grafikobjekte.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class MoveCmd : public QUndoCommand
{
public:
    /*!
     * \brief MoveCmd Konstruktor.
     * \param items Liste der zu verscheibenden Elemete
     * \param delta Verschiebungsvektor
     */
    MoveCmd(const QList<QGraphicsItem*>& items, QPointF delta)
    {
        this->items = items;
        this->delta = delta;
    }

    /*!
     * \brief id die Identitaet dieses Befehls ist 1
     * \return 1
     * \see mergeWith
     */
    int id()const override{return 1;}

    /*!
     * \brief redo ausfuehren des Befehls.
     */
    void redo()override
    {
        for(auto& item : items)
            item->moveBy(delta.x(), delta.y()); //alle elemente verschieben um delta
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo()override
    {
        for(auto& item : items)
            item->moveBy(-delta.x(), -delta.y()); //alle elemente ruckverschieben
    }

    /*!
     * \brief mergeWith fuegt diesen Befehl mit gleichartigen zusammenin einen einzigen Befehl.
     * \param command Zeiger auf anderen Befehl
     * \return true falls erfolgreich.
     */
    bool mergeWith(const QUndoCommand *command)override
    {
        if(id()==command->id()){ //befehle gleich?
            if(items == static_cast<const MoveCmd*>(command)->items){
                //verschiebungsvektoren adieren wenn die Liste der Elemente gleich ist
                delta += static_cast<const MoveCmd*>(command)->delta;
                return true; //Befehle zusammengefuegt.
            }
        }
        return false; //nicht zusammengefuegt.
    }

    QPointF delta;
    QList<QGraphicsItem*> items;

};

#endif // MOVECMD_H
