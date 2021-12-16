#ifndef PT_ELEMENT_H
#define PT_ELEMENT_H
#include "abstractpt.h"


#include "globaltypes.h"
namespace Item {

    /*!
     * \brief Transition Klasse dient der Darstellung und Simulation von Transitionen in einem Petrinetz.
     * \inherits AbstractPT
     * \author Samuel Pitt
     */
    class Transition : public AbstractPT
    {
    public:

        /*!
         * \brief Transition Konstruktor.
         * \param center Mitte der Transition.
         */
        Transition(QPointF center);

        /*!
         * \brief isActivated gibt an ob die Transition aktiviert ist und ausgeloest werden koennte.
         * \return true wenn aktiv, false wenn nicht.
         */
        bool isActivated();

        /*!
         * \brief fire loest die Transition aus, falls moeglich.
         * Nimmt den hineinfuehrenden Stellen Marken weg
         * und fuegt den herausgehenden Stellen Marken ein.
         */
        void fire();

        /*!
         * \brief boundingRect Rechteck in dem gesamter Gegenstand dargestellt wird.
         * \return Recchteck in lokalem Koordinatensystem.
         */
        QRectF boundingRect() const override
        {
            return QRectF(-WIDTH/2, - WIDTH/2, WIDTH, HEIGHT);
        }

        /*!
         * \brief paint zeichnet das Objekt auf die Scene.
         * \param painter
         * \param option
         * \param widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        /*!
         * \brief shape gibt die Form des Rechtecks, welche die Transition ausmacht zurueck.
         * \return Ein Pfad im lokalem Koordinatensystem
         */
        QPainterPath shape() const override;

        /*!
         * \brief loadTool ladet ein Werkzeug und aendert allenfals die Darstellungsweise des Maus-Cursors.
         * \param t das Werkzeug
         * Beim Werkzeug Tool::Fire wird der Cursor für das hovern über dieses Objekt zu einem Zeigefinger geaendert,
         * was die Interaktivität den Usern kommunizieren soll.
         */
        void loadTool(const Tool::PetriTool& t)override;



        //Konstanten
        static const uint8_t WIDTH = 200;
        static const uint8_t HEIGHT = 220;
        static const QBrush inactiveBrush;
        static const QBrush activeBrush;
        static const QPen pen;

        //statische variabeln.
        static int transCnt;


    protected:

        /*!
         * \brief mousePressEvent Maus-event-handler beim drücken einer Maustaste auf diesem Objekt.
         * \param event
         */
        void mousePressEvent(QGraphicsSceneMouseEvent* event)override;

    };

}

#endif // PT_ELEMENT_H
