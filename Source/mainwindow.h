#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>

#include "petri.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief MainWindow Klasse stellt das Hauptfenster dar.
 * \inherits QMainWindow
 * \author Samuel Pitt
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow Konstruktor.
     * \param parent
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*!
     * \brief addPetri fuegt den offenen Dateien ein Petri-Opjekt hinzu
     * \param petri Zeiger auf das Petri-Objekt
     * \see Petri
     */
    void addPetri(Petri *petri);

    /*!
     * \brief selectPetri ziege ein Petri-Objekt als aktuelles Petrinetz an.
     * \param petri Zeiger auf das Petri-Objekt
     * \see Petri
     */
    void selectPetri(Petri *petri);

    /*!
     * \brief closePetri schliese ein Petri-Objekt mit eventuellem abspeichern von Aenderungen.
     * \param petri Zeiger auf das Petri-Objekt
     * \see Petri
     */
    void closePetri(Petri *petri);

private slots:

    /*!
     * \brief on_connectorBtn_clicked Event-Handler fuer Kantenwerkzeugtaste.
     */
    void on_connectorBtn_clicked();

    /*!
     * \brief on_moveBtn_clicked Event-Handler fuer Auswahlwerkzeugtaste.
     */
    void on_moveBtn_clicked();

    /*!
     * \brief on_placeBtn_clicked Event-Handler fuer Stellenwerkzeugtaste.
     */
    void on_placeBtn_clicked();

    /*!
     * \brief on_runBtn_clicked Event-Handler fuer Ausloesewerkzeugtaste.
     */
    void on_runBtn_clicked();

    /*!
     * \brief on_transitionBtn_clicked Event-Handler fuer Transitionenwerkzeugtaste.
     */
    void on_transitionBtn_clicked();

    /*!
     * \brief on_deleteBtn_clicked Event-Handler fuer Loeschwerkzeugtaste.
     */
    void on_deleteBtn_clicked();

    /*!
     * \brief on_tokenBtn_clicked Event-Handler fuer Markenwerkzeugtaste.
     */
    void on_tokenBtn_clicked();

    /*!
     * \brief on_undoBtn_clicked Event-Handler fuer Ruckgaengigtaste.
     */
    void on_undoBtn_clicked();

    /*!
     * \brief on_redoBtn_clicked Event-Handler fuer Wiederholentaste.
     */
    void on_redoBtn_clicked();

    /*!
     * \brief on_actionSave_triggered Event-Handler fuer Speichern.
     */
    void on_actionSave_triggered();

    /*!
     * \brief on_actionSave_As_triggered Event-Handler fuer Speichern als
     */
    void on_actionSave_As_triggered();

    /*!
     * \brief on_actionOpen_triggered Event-Handler fuer Oeffnen
     */
    void on_actionOpen_triggered();

    /*!
     * \brief on_actionUndo_triggered Event-Handler fuer Ruckgaengig.
     */
    void on_actionUndo_triggered();

    /*!
     * \brief on_actionRedo_triggered Event-Handler fuer Wiederholen.
     */
    void on_actionRedo_triggered();

    /*!
     * \brief on_actionNew_triggered Event-Handler fuer Neu.
     */
    void on_actionNew_triggered();

    /*!
     * \brief on_actionClose_triggered Event-Handler fuer Schliessen.
     */
    void on_actionClose_triggered();

    /*!
     * \brief on_comboBox_currentIndexChanged Event-Handler fuer Indexwechsel der Dateiauswahlbox
     * \param index
     */
    void on_comboBox_currentIndexChanged(int index);

private:

    /*!
     * \brief selectBtn setzt Taste auf ausgewählt und alle anderen Werkzeugtasten auf nicht ausgewaehlt.
     * \param btn Taste
     */
    void selectBtn(QPushButton* btn);

    Ui::MainWindow *ui;
    
};
#endif // MAINWINDOW_H
