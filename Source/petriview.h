#ifndef PETRIVIEW_H
#define PETRIVIEW_H

#include <QGraphicsView>
#include <QtWidgets>
#include <QWidget>

#include "globaltypes.h"
#include "petri.h"
#include "zoomobject.h"

/*!
 * \brief PetriView Klasse ist eine Grafikansicht zur ansicht und bearbitung von Petrinetzen der Klasse "Petri"
 * \inherits QGraphicsView
 * \see Petri
 * \author Samuel Pitt
 */
class PetriView : public QGraphicsView
{
public:
    /*!
     * \brief PetriView Konstruktor.
     * \param parent
     */
    PetriView(QWidget* parent = 0);

    /*!
     * \brief loadTool ladet Werkzeug bi allen offenen Petri-Objekten.
     * \param t Werkzeug
     */
    void loadTool(const Tool::PetriTool& t);


    Petri* petri;

    QList<Petri*> openPetris;

    ZoomObject zoom;

protected:
    /*!
     * \brief mousePressEvent Mausdruck-Event-handler.
     * \param event
     */
    virtual void mousePressEvent(QMouseEvent* event)override;

    /*!
     * \brief mouseReleaseEvent Mausdloslass-Event-handler.
     * \param event
     */
    virtual void mouseReleaseEvent(QMouseEvent* event)override;

    /*!
     * \brief wheelEvent Mausrad-Event-handler fuers scrolen und zoomen.
     * \param event
     */
    virtual void wheelEvent(QWheelEvent* event)override;

    /*!
     * \brief keyPressEvent Tastendruck-Event-handler zum speichern von aktuell gedrueckten Modifiziertasen wie Strg.
     * \param event
     */
    virtual void	keyPressEvent(QKeyEvent *event) override
    {
        keymods = event->modifiers();
        QGraphicsView::keyPressEvent(event);
    }

    /*!
     * \brief keyReleaseEvent Tastenloslass-Event-handler zum loeschen von nicht mehr gedrueckten Modifiziertasen wie Strg.
     * \param event
     */
    virtual void	keyReleaseEvent(QKeyEvent *event) override
    {
        keymods = Qt::NoModifier;
        QGraphicsView::keyReleaseEvent(event);
    }




private:
    Tool::PetriTool currentTool;

    Qt::KeyboardModifiers keymods;

};

#endif // PETRIVIEW_H
