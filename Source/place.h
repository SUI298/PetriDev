#ifndef PLACE_H
#define PLACE_H

#include "abstractpt.h"
#include <cmath>
#include "globaltypes.h"

namespace Item {

    /*!
     * \brief Place Klasse dient der Darstellung und Simulation einer Stelle eines Petrinetzes
     * \inherits AbstractPT
     * \author Samuel Pitt
     */
    class Place : public AbstractPT
    {
    public:
        /*!
         * \brief Place Konstruktor
         * \param center Kordinate des Zentrums
         */
        Place(const QPointF& center);

        /*!
         * \brief boundingRect ein Rechteck in dem der gesamte Gegenstand enthalten ist.
         * \return Ein Rechteck im lokalen Koordinatensystem.
         */
        QRectF boundingRect() const override
        {
            return QRectF(-WIDTH/2, - WIDTH/2, WIDTH, HEIGHT);
        }

        /*!
         * \brief paint zeichnet die Stelle.
         * \param painter
         * \param option
         * \param widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        /*!
         * \brief shape gibt die genaue Form der Stelle zurück.
         * \return einen Pfad im lokalen Koordinatensystem
         */
        QPainterPath shape() const override;

        /*!
         * \brief toJson darstellung in form eines JSON-Objektes
         * \return QJsonObject enhält die in der Basisklasse definierten atribute und die Anzahl Marken ("tokens")
         * \see AbstractPT::toJson
         */
        QJsonObject toJson()override;

        /*!
         * \brief loadTool ladet ein Werkzeug und aendert allenfals die Darstellungsweise des Maus-Cursors.
         * \param t das Werkzeug
         * Beim Werkzeug Tool::Token wird der Cursor für das hovern über dieses Objekt zu einem Zeigefinger geaendert,
         * was die Interaktivität den Usern kommunizieren soll.
         */
        void loadTool(const Tool::PetriTool& t)override;

        /*!
         * \brief getTokens gibt Anzahl Marken zuruek.
         * \return anzahl Marken in dieser Stelle.
         */
        uint8_t getTokens()const override{return tokens;}

        /*!
         * \brief addTokens fuegt der Stelle Marken hinzu, falls der Bereich nicht unter- oder ueberschritten wird.
         * \param t anzahl Marken.
         */
        void addTokens(int t)override;



        //constants
        static const uint8_t WIDTH = 200;
        static const uint8_t HEIGHT = 220;
        static const QBrush filling;
        static const QBrush tokenBrush;
        static const QPen pen;

        static int placeCnt;


    protected:

        /*!
         * \brief mousePressEvent Maus-event-handler beim drücken einer Maustaste auf diesem Objekt.
         * \param event
         */
        void mousePressEvent(QGraphicsSceneMouseEvent* event)override;


    private:
        uint8_t tokens;

    };
}
#endif // PLACE_H
