#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QJsonObject>
#include "abstractpt.h"


#include "globaltypes.h"
namespace Item {
    class AbstractPT;

    /*!
     * \brief Connector Klasse dieht der Darstellung und Simulation einer Kante in einem Petrinetz.
     * \inherits QGraphicsItem
     * \author Samuel Pitt
     */
    class Connector : public QGraphicsItem
    {
    public:
        /*!
         * \brief Connector Konstruktor
         * \param start Startposition
         * \param tailPT Zeiger zu AbstactPT welches am Ursprung des Pfeiles sein soll
         */
        Connector(QPointF start, AbstractPT * tailPT = nullptr);

        /*!
         * \brief boundingRect Rechteck in dem sich der Gegenstand Bebindet
         * \return Rechteck in Lokalkoordinaten
         */
        QRectF boundingRect() const override
        {
            qreal minX = qMin(head.x(), 0.0);
            qreal minY = qMin(head.y(), 0.0);
            qreal maxX = qMax(head.x(), 0.0);
            qreal maxY = qMax(head.y(), 0.0);
            return QRectF(minX-10*arrowsize, minY-10*arrowsize, maxX- minX + 20*arrowsize, maxY - minY + 20*arrowsize);
        }

        /*!
         * \brief paint zeichnet den Pfeil
         * \param painter
         * \param option
         * \param widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        /*!
         * \brief shape gibt die ungefaehre Form zum ermiteln von Maus events zurueck
         * \return
         */
        QPainterPath shape() const override;

        /*!
         * \brief toJson wird zum speichern in einer Datei im JSON format verwendet.
         * \return JSON-Objekt mit Ursprungsposition "tailPos", Endposition "headPos" und Gewichtung "weight".
         */
        QJsonObject toJson();

        /*!
         * \brief shouldExist sagt ob alle existenzbedingungen fuer die Kante erfuellt sind. Falls nicht sollte sollte die Kannte zersoert werden.
         * \return true wenn Existenz berechtigt.
         */
        bool shouldExist();

        bool active;
        bool autoReactivation;

        /*!
         * \brief getWeight gibt gewichtung der Kannte zurueck
         * \return Gewichung
         */
        uint8_t getWeight();

        /*!
         * \brief setWeight setzt Gewichtung
         * \param w Gewichtung
         */
        void setWeight(uint8_t w){weight.setText(QString::number(w));}

        /*!
         * \brief placeHasEnoughTokens sagt ob es in der Stelle am Ursprung mindestens so viel Marken hat wie die Gewichtung.
         * \return true wenn genug vorhanden sonnst false
         */
        bool placeHasEnoughTokens();

        /*!
         * \brief deliverTokens fuegt der Stelle am ende der Kannte getWeight() Marken hinzu.
         * \see getWeight
         */
        void deliverTokens();

        /*!
         * \brief removeTokens nimmt der Stelle am Ursprung der Kannte getWeight() Marken weg.
         * \see getWeight
         */
        void removeTokens();

        /*!
         * \brief loadTool lade Wekzeug.
         * \param t
         */
        void loadTool(const Tool::PetriTool& t);


        /*!
         * \brief setHeadPT setz den zeiger auf das Kanntenende auf value
         * \param value Zeiger auf eine Stelle oder Transition
         */
        void setHeadPT(AbstractPT *value);

        /*!
         * \brief setTailPT setz den zeiger auf das Kanntenursprung auf value
         * \param value Zeiger auf eine Stelle oder Transition
         */
        void setTailPT(AbstractPT *value);


    protected:
        /*!
         * \brief mouseMoveEvent Mausbewegungsevent-handler.
         * \param event
         */
        void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;

    private:
        QPointF head;
        Annotation weight;


        Tool::PetriTool currentTool;


        AbstractPT* headPT;
        AbstractPT* tailPT;

        /*!
         * \brief updatePoints fordert von den Stellen und Transitionen an beiden Enden der Kannte die optimalen Koordinaten an.
         */
        void updatePoints();

        static const QPen pen;
        static const QBrush arrowBrush;
        static const qreal arrowsize;
    };

}
#endif // CONNECTOR_H
