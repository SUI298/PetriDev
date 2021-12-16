#ifndef NAMEOBJECT_H
#define NAMEOBJECT_H

#include <QObject>
#include <QString>

/*!
 * \brief NameObject Klasse entaelt einen String und gibt ein signal aus wenn dieser veraender wird.
 * \inherits QObject
 * \author Samuel Pitt
 */
class NameObject : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief NameObject Konstruktor.
     * \param value Textwert
     */
    NameObject(const QString& value);

    /*!
     * \brief getValue getter-Funktion fuer Textwert.
     * \return Textwert.
     */
    QString getValue() const;

    /*!
     * \brief setValue setter-Funktion fuer Textwert sendet changed-Signal.
     * \param value Textwert
     * \see changed
     */
    void setValue(const QString &value);

signals:
    /*!
     * \brief changed Signal wenn Textwert veraendert wurde (gemaess Qt signals and slots).
     * \param txt neuer Text
     */
    void changed(QString txt);

private:
    QString value;
};

#endif // NAMEOBJECT_H
