#include "nameobject.h"

NameObject::NameObject(const QString& value)
{
    setValue(value);
}

QString NameObject::getValue() const
{
    return value;
}

void NameObject::setValue(const QString &value)
{
    this->value = value;
    emit changed(value); //signal senden
}
