#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer::singleShot(0, this, [=](){
        //ein leeres Petrinetz einfuegen sobald das Fenster korrekt inititialisiert wurde und frameRect die korrekte dimension hat
        addPetri(new Petri(ui->graphicsView->frameRect()));
        ui->comboBox->addItem(ui->graphicsView->petri->name.getValue());
    });


    //QT signals and slots *Magie* ;) alles automatisch
    //siehe https://doc.qt.io/qt-5/signalsandslots.html
    //Zoom + Taste zoomt 1%
    connect(ui->plusBtn, &QPushButton::clicked, &ui->graphicsView->zoom,
            [=] () { //lambda
                ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
                ui->graphicsView->zoom.relativeZoom(1.01);
            });

    //Zoom - Taste zoomt -1%
    connect(ui->minusBtn, &QPushButton::clicked, &ui->graphicsView->zoom,
            [=] () { //lambda
                ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
                ui->graphicsView->zoom.relativeZoom(0.99);
            });

    //Zoom-Slider setzt zoom auf absoluten Wert
    connect(ui->zoomSlider, &QAbstractSlider::valueChanged, ui->graphicsView,
            [=](int value){ //lambda
                if(value != ui->graphicsView->zoom.percentVal()){
                    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
                    ui->graphicsView->zoom.absoluteZoom(ui->zoomSlider->value());
                }
            });

    //Zoom anzeige wird automatisch aktualisiert
    connect(&ui->graphicsView->zoom, &ZoomObject::changed, this,
            [=] (int val) { //lambda
                ui->zoomLbl->setText(QString::number(val)+ "%");
                ui->zoomSlider->setValue(val);
            });

}

MainWindow::~MainWindow()
{
    //loeschen und schliessen
    for(auto& petri : ui->graphicsView->openPetris){
        closePetri(petri);
    }
    delete ui;
}

void MainWindow::addPetri(Petri *petri)
{
    selectPetri(petri);
    ui->graphicsView->openPetris.append(petri);
    ui->comboBox->addItem(petri->name.getValue());
    ui->comboBox->setCurrentText(petri->name.getValue());
}

void MainWindow::selectPetri(Petri *petri)
{
    ui->graphicsView->petri = petri;
    ui->graphicsView->setScene(petri);
}

void MainWindow::closePetri(Petri *petri)
{
    if(petri==nullptr)return;

    if(!petri->undoStack->isClean()){ //nicht gespeicherte Änderungen.
        //User nach vorgehen fragen
        QMessageBox msgBox;
        msgBox.setText("Das Dokument " + petri->name.getValue() + " wurde verändert.");
        msgBox.setInformativeText("Möchten Sie die Änderungen Speichern?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int btn = msgBox.exec();

        switch(btn){
        case QMessageBox::Cancel:
            //Abbrechen
            return;
        case QMessageBox::Save:
            //speichern vor dem schliesen
            on_actionSave_triggered();
            break;
        default:
            break;
        }
    }

    //SCHLIESSEN:
    //überall entfernen
    ui->comboBox->removeItem(ui->comboBox->findText(petri->name.getValue()));
    ui->graphicsView->openPetris.removeAll(petri);

    //nächstes petri-netz anzeigen, falls vorhanden
    if(!ui->graphicsView->openPetris.isEmpty()){
        ui->comboBox->setCurrentText(ui->graphicsView->openPetris.first()->name.getValue());
    }else
        selectPetri(nullptr);

    //löschen
    delete petri;
}


//Werkzeuge laden
void MainWindow::on_connectorBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Connectror);
    selectBtn(ui->connectorBtn); //Visuelles Feedback an User.
}

void MainWindow::on_moveBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Select);
    selectBtn(ui->moveBtn); //Visuelles Feedback an User.
}

void MainWindow::on_placeBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Place);
    selectBtn(ui->placeBtn); //Visuelles Feedback an User.
}

void MainWindow::on_runBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Fire);
    selectBtn(ui->runBtn); //Visuelles Feedback an User.
}

void MainWindow::on_transitionBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Transition);
    selectBtn(ui->transitionBtn); //Visuelles Feedback an User.
}

void MainWindow::on_deleteBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Delete);
    selectBtn(ui->deleteBtn); //Visuelles Feedback an User.
}

void MainWindow::on_tokenBtn_clicked()
{
    ui->graphicsView->loadTool(Tool::Token);
    selectBtn(ui->tokenBtn); //Visuelles Feedback an User.
}

void MainWindow::on_undoBtn_clicked(){ui->graphicsView->petri->undoStack->undo();}
void MainWindow::on_redoBtn_clicked(){ui->graphicsView->petri->undoStack->redo();}
void MainWindow::on_actionUndo_triggered(){ ui->graphicsView->petri->undoStack->undo();}
void MainWindow::on_actionRedo_triggered(){ ui->graphicsView->petri->undoStack->redo();}

void MainWindow::on_actionSave_triggered()
{
    if(ui->graphicsView->petri == nullptr)return;

    if(ui->graphicsView->petri->getPath().isEmpty())
        //falls kein Speicherort, wie speichern als.
        on_actionSave_As_triggered();
    else{
        //sonst an pfad abspeicern

        QFile file(ui->graphicsView->petri->getPath());
        //Datei zum schreiben oeffnen
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text ))
            return;

        QJsonDocument json(ui->graphicsView->petri->toJson());
        QTextStream out(&file);
        out << json.toJson(QJsonDocument::Compact); //Datei schreiben

        file.close(); //datei schliessen

        //Aenderungen als gespeichert markieren.
        ui->graphicsView->petri->undoStack->clear();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    if(ui->graphicsView->petri == nullptr)return;
    //Pfad von User erlangen
    QString path = QFileDialog::getSaveFileName(this, "Speichern als", "", "JSON(*.json)");

    if (path.isEmpty())return; //wenn kein Pfad -> abbruch

    ui->comboBox->findText(ui->graphicsView->petri->name.getValue());
    ui->graphicsView->petri->name.setValue(path); //neuer name setzen

    ui->graphicsView->petri->setPath(path); //pfad setzen

    on_actionSave_triggered(); //speichern der datei unter neu gefundenem Pfad
}

void MainWindow::on_actionOpen_triggered()
{
    //zu oeffnende Datei finden
    QString path = QFileDialog::getOpenFileName(this, "Öffnen", "", "JSON(*.json)");

    QFile file(path);

    //Datei zum lesen oeffnen.
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
        return;

    QJsonDocument doc;
    QByteArray data = file.readAll();
    doc = QJsonDocument::fromJson(data);

    file.close(); //datei schliessen
    addPetri( new Petri(doc.object(),path) );//Petri erstellen ond darstellen
}




void MainWindow::on_actionNew_triggered()
{
    addPetri(new Petri(ui->graphicsView->frameRect())); //neues, leeres Petri hinzu
}

void MainWindow::on_actionClose_triggered()
{
    closePetri(ui->graphicsView->petri); //aktuelles Petri schliessen
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    for(Petri* p : ui->graphicsView->openPetris){
        //Petri in offenen Petris suchen
        if(ui->comboBox->itemText(index) == p->name.getValue()){
            //gefunden -> auswaehlen
            selectPetri(p);
            //Namensaenderung mit comboBox verbinden
            connect(&(p->name), &NameObject::changed, ui->comboBox,
                    [=](QString txt){
                        ui->comboBox->setItemText(index, txt);
                    });
            return;
        }
    }
}

void MainWindow::selectBtn(QPushButton *btn)
{
    ui->transitionBtn->setChecked(false);
    ui->placeBtn->setChecked(false);
    ui->connectorBtn->setChecked(false);
    ui->tokenBtn->setChecked(false);
    ui->deleteBtn->setChecked(false);
    ui->moveBtn->setChecked(false);
    ui->runBtn->setChecked(false);

    btn->setChecked(true);
}
