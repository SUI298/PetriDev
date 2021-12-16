#ifndef PETRI_H
#define PETRI_H
#include <QList>
#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QUndoStack>

#include "globaltypes.h"

#include "place.h"
#include "transition.h"
#include "connector.h"
#include "nameobject.h"
#include <QObject>


/*!
 * \brief Petri Klasse ist eine von QGraphicsScene abgeleitete Klasse zur darstellung von Petrinetzen.
 * \inherits QGraphicsScene
 * \author Samuel Pitt
 */
class Petri : public QGraphicsScene
{
public:
    /*!
     * \brief Petri Konstruktor erstellt ein leeres Petrinetz.
     */
    Petri(const QRect& initialSize = QRect());

    /*!
     * \brief Petri Konstruktor erstellt ein Petrinetz anhand einer JSON datei
     * \param json JSON objekt bestehend aus Arrays fuer Stellen "places, Transitionen "transitions" und Knaten "connectors"
     * \param path Dateipfad fuer erneutes abspeichern.
     */
    Petri(const QJsonObject& json, const QString& path);

    /*!
     * \brief ~Petri Destruktor befreit verwndeter Speicher.
     */
    virtual ~Petri();

    /*!
     * \brief loadTool lade Werkzeug auf alle Objekte.
     * \param t Werkzeug
     */
    void loadTool(Tool::PetriTool t);

    /*!
     * \brief addPlace fuege neue Stelle dem Petrinetz hinzu.
     * \param pos Position auf dem Petrinetz
     * \return Zeiger auf die neue Stelle
     */
    Item::Place* addPlace(const QPointF& pos);

    /*!
     * \brief addTransition fuege neue Tansition dem Petrinetz hinzu.
     * \param pos Position auf dem Petrinetz
     * \return Zeiger auf die neue Transition
     */
    Item::Transition* addTransition(const QPointF& pos);

    /*!
     * \brief startConnector starte neue Kante an dier Position
     * \param pos Position auf dem Petrinetz
     * \return Zeiger auf Kante
     */
    Item::Connector* startConnector(const QPointF& pos);

    /*!
     * \brief stopConnector setze Knaten ende auf Position fest.
     * \param pos Position auf dem Petrinetz
     * \return Zeiger auf Kante
     */
    Item::Connector* stopConnector(const QPointF& pos);

    /*!
     * \brief addPlace fuege vorhandene deaktivierte Stelle wieder zu Petrinetz hinzu.
     * \param p zeiger auf Stelle
     */
    void addPlace(Item::Place* p);

    /*!
     * \brief addTransition fuege vorhandene deaktivierte Transition wieder zu Petrinetz hinzu.
     * \param p zeiger auf Transition
     */
    void addTransition(Item::Transition*);

    /*!
     * \brief addConnector fuege vorhandene deaktivierte Kante wieder zu Petrinetz hinzu.
     * \param p zeiger auf Kante
     */
    void addConnector(Item::Connector *);

    /*!
     * \brief toJson wandle ein Petrinetz in ein speicherbares JSON-Objekt um.
     * \return JSON-Objekt bestehend aus Arrays fuer Stellen "places", Transitionen "transitions", Kanten "connectors"
     * \see Item::Place::toJson
     * \see Item::Transition::toJson
     * \see Item::Connector::toJson
     * \see MainWindow::on_actionSave_triggered
     */
    QJsonObject toJson();

    /*!
     * \brief remove entfernt alle an einem Punkt vorkandene Objekte vom Petrinetz.
     * \param pos Punkt auf dem Petrinetz
     * \return ob entfernt wurde (true) oder nicht (false)
     */
    bool remove(const QPointF& pos);

    /*!
     * \brief remove entfernt ein bestimmtes Objekt vom Petrinetz.
     * \param item Zeiger auf Grafikobjekt
     */
    void remove(QGraphicsItem * item);

    /*!
     * \brief undoStack speichert alle Aenderungen am Dokument als ruekgängigmachbare Befehle.
     */
    QUndoStack* undoStack;

    /*!
     * \brief name name des Dokuments, welcher signale aussendet wenn veraenert.
     * \see NameObject
     */
    NameObject name;

    /*!
     * \brief getPath getter-Funktion fuer Dateipfad
     * \return Dateipfad als String
     */
    QString getPath() const;

    /*!
     * \brief setPath setzt dateipfat auf Wert.
     * \param value Wert
     */
    void setPath(const QString &value);

public slots:
    /*!
     * \brief documentNotSaved fuegt dem namen ein "*" davor ein wenn es ungespeicherte Aenderungen gibt, und enfernt "*" wenn alles gespeichert.
     * \param undo_clean true wenn nichts rueckgaengigmachbar ist -> keine ungesteicherten Aenderungen. False wenn es welche gibt.
     * \see name
     */
    void documentNotSaved(bool undo_clean);


private:
    QList<Item::Place *> places;
    QList<Item::Transition*> transitions;
    QList<Item::Connector*> connectors;

    QList<QGraphicsItem*> allItems;

    QString path;

    /*!
     * \brief pos Hilfsfunktion wandelt ein JSON-Objekt in einen Punkt.
     * \param pos JSON-Objekt "pos" mit Zahlen "x", und "y"
     * \return Punkt
     */
    QPointF pos(QJsonObject pos);

    static int cnt;
};

#endif // PETRI_H
