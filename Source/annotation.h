#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <QGraphicsTextItem>
#include <QKeyEvent>

/*!
 * \brief Annotation ist ein Text, welcher als Beschreibung eines Petrinetzelementes dienen kann.
 * \inherits QGraphicsTextItem
 * \author Samuel Pitt
 */
class Annotation : public QGraphicsTextItem
{
public:
    /*!
     * \brief Annotation Konstruktor.
     * \param parent Besitzerobjekt
     * \param topcenter Mitte-Oben position des textes
     * \param width Max. Breite des Textes.
     * \param txt Text
     * \param numbersOnly wahr wenn nur zahlen als Input akzeptiert werden sollen
     */
    Annotation(QGraphicsItem * parent, const QPointF& topcenter, int width, const QString& txt, bool numbersOnly = false);

    /*!
     * \brief setText setzt Text und achtet auf korrekte formatierung.
     * \param txt
     */
    void setText(const QString& txt);

    /*!
     * \brief setNumbersOnly setzt ob nur zahlen als Input akzeptiert werden.
     * \param val true -> nur Zahlen
     */
    void setNumbersOnly(bool val){numbersOnly = val;}

protected:

    /*!
     * \brief keyPressEvent Tastatureventhandler für das kontrolierte bearbeiten des Textes.
     * \param event
     */
    void keyPressEvent(QKeyEvent *event) override;


private:
    bool numbersOnly;
};

#endif // ANNOTATION_H
