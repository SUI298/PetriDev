#ifndef ZOOMOBJECT_H
#define ZOOMOBJECT_H

#include <QObject>
#include <QTransform>

/*!
 * \brief ZoomObject Klasse speichert eine Zoom-transformation und sendet ein signal wenn der Zoom verändert wird.
 * \inherits QObject
 * \inherits QTransform
 * \author Samuel Pitt
 */
class ZoomObject : public QObject, public QTransform
{
    Q_OBJECT
public:
    /*!
     * \brief ZoomObject Konstuktor
     * \param parent
     */
    explicit ZoomObject(QObject *parent = nullptr);

    /*!
     * \brief percentVal gibt den Zoom als Prozentwert an.
     * \return Prozentwert
     */
    int percentVal(){return m11()*100;}

public slots:

    /*!
     * \brief relativeZoom zoomt um einen Faktor relativ zum aktuellen wert
     * \param scale zum verkleinern < 1, zum vergrössern > 1
     */
    void relativeZoom(qreal scale);

    /*!
     * \brief absoluteZoom setzt zoom auf einen absoluten wert in prozent.
     * \param percent 100 -> kein zoom, > 100 -> vergrössert, < 100 -> verkleinert.
     */
    void absoluteZoom(int percent);

signals:
    /*!
     * \brief changed Signal nach dem Qt signals and slots mechanismus, wird ausgegeben wenn sich der Zoom-wert veraendert.
     * \param zoom
     */
    void changed(int zoom);
};

#endif // ZOOMOBJECT_H
