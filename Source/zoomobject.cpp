#include "zoomobject.h"

ZoomObject::ZoomObject(QObject *parent)
    : QObject(parent)
{}

void ZoomObject::relativeZoom(qreal scale)
{
    this->scale(scale, scale); //zoomen
    emit changed(m11()*100);   //signal ausgeben
}

void ZoomObject::absoluteZoom(int percent)
{
    reset(); //ruecksetzen auf 100%
    relativeZoom(0.01*double(percent)); //zoomen & signal ausgeben
}
