#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

/*!
 * \brief main main-Funktion.
 * \param argc
 * \param argv
 * \return 0 wenn kein Absturz
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Dunkler Stil von QuantumCD:
    //https://gist.github.com/QuantumCD/6245215
    qApp->setStyle(QStyleFactory::create("Fusion"));
    QColor darkGray(53, 53, 53);
    QColor gray(128, 128, 128);
    QColor black(25, 25, 25);
    QColor blue(42, 130, 218);
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, darkGray);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, black);
    darkPalette.setColor(QPalette::AlternateBase, darkGray);
    darkPalette.setColor(QPalette::ToolTipBase, blue);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, gray.darker(150));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Link, blue);
    darkPalette.setColor(QPalette::Highlight, blue);
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, gray);
    darkPalette.setColor(QPalette::Disabled, QPalette::Light, darkGray);
    qApp->setPalette(darkPalette);


    w.show();
    return a.exec();
}
