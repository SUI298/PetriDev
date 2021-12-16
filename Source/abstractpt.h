#ifndef ABSTRACTPT_H
#define ABSTRACTPT_H

#include <QList>
#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtDebug>
#include <QJsonObject>

#include "annotation.h"
#include "connector.h"
#include "globaltypes.h"

namespace Item {

    class Connector;

    /*!
     * \brief AbstractPT Klasse ist eine Abstrakte Klasse für Stellen (Place) und Transitionen (Transition)
     * \inherits QGraphicsItem
     * \see Place
     * \see Transition
     * \author Samuel Pitt
     */
    class AbstractPT : public QGraphicsItem
    {
    public:


        /*!
         * \brief AbstractPT Konstruktor.
         * \param center Mittenposition.
         * \param d_topcenter Position des Beschreibungstextes
         * \param d_width Textbreite
         * \param d_txt Text
         */
        AbstractPT(const QPointF& center, const QPointF& d_topcenter, int d_width, const QString& d_txt);

        /*!
         * \brief boundingRect muss in Abgeleiteten Klassen implemetiert werden.
         * \return das Rechteck in dem sich der Gegenstand befindet.
         */
        virtual QRectF boundingRect() const override = 0;

        /*!
         * \brief paint zeichnet den Gegenstand auf die Scene
         * \param painter
         * \param option
         * \param widget
         * Zeichnet von \see QGraphicsItem::shape gegebene Form. Die Farbliche Gestaltung wird von den abgeleiteten Klassen vorgenommen.
         */
        virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

        /*!
         * \brief toJson wird zum speichern in einer Datei im JSON format verwendet.
         * \return QJsonObject mit Position "pos" (enhält "x" und "y" Koordinaten) und Bezeichnung "designator"
         */
        virtual QJsonObject toJson();

        /*!
         * \brief getTokens für Polymorphe Verwendung, nur für abgeleitete Klasse Place.
         * \return 0;
         * \see Place
         */
        virtual uint8_t getTokens()const{return 0;}

        /*!
         * \brief addTokens für Polymorphe Verwendung, nur für abgeleitete Klasse Place.
         * \param t nicht verwendet.
         * \see Place
         */
        virtual void addTokens(int t){(void)t;/*tell the compiler t is unused*/}

        /*!
         * \brief loadTool speichert neues Werkzeug currentTool.
         * \param t Werkzeug.
         */
        virtual void loadTool(const Tool::PetriTool& t);

        /*!
         * \brief addConnectorIn fuege Kante in Gegenstand rein hinzu.
         * \param c Zeiger zu Kante
         * \see Connector
         */
        virtual void addConnectorIn(Connector* c){if(c != nullptr)incommingConnectors.append(c);}

        /*!
         * \brief addConnectorOut füge Kante aus Gegenstand raus hinzu.
         * \param c Zeiger zu Kante
         * \see Connector
         */
        virtual void addConnectorOut(Connector* c){if(c != nullptr)outgoingConnectors.append(c);}

        /*!
         * \brief removeConnectorIn entferne Kante in Gegenstand rein.
         * \param c Zeiger zu Kante
         * \see Connector
         */
        virtual void removeConnectorIn(Connector* c){incommingConnectors.removeAll(c);}

        /*!
         * \brief removeConnectorOut entferne Kante aus Gegenstand raus.
         * \param c Zeiger zu Kante
         * \see Connector
         */
        virtual void removeConnectorOut(Connector* c){outgoingConnectors.removeAll(c);}

        /*!
         * \brief setDesignator setze Beschreibungstext.
         * \param txt neuer Beschreibungstext.
         */
        void setDesignator(const QString& txt){designator.setText(txt);}

        /*!
         * \brief getConnectionPoint finde ideale koordinate eines Endpunktes einer Kante.
         * \param other gegenüberliegender Endpunkt
         * \return Koordinate im Koordinatensystem der QGraphicsScene
         * Ausgehend vom Zemtrum dieses Objektes wird in einer Geraden, in Richtung other,
         * der erste Punkt, der nicht mehr im shape des Gegenstands enthalten ist zurükgegeben
         */
        QPointF getConnectionPoint(const QPointF& other)const;

        /*!
         * \brief getCenter gibt das Zenrum des Gegenstandes zurück.
         * \return im Koordinatensystem der QGraphicsScene.
         */
        QPointF getCenter() const;

        /*!
         * \brief getType gibt einen Zeichen, welches für einen Gegenstandsty steht zurück
         * \return 'P' fuer Stellen, 'T' für Transitionen.
         */
        QChar getType()const{return data(0).toChar();}

        QList<Connector*> incommingConnectors;
        QList<Connector*> outgoingConnectors;

        bool active;
    protected:

        /*!
         * \brief mouseMoveEvent Mausbewegungs event-handler.
         * \param event
         */
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;

        Annotation designator;
        Tool::PetriTool currentTool;

    };
}

#endif // ABSTRACTPT_H
