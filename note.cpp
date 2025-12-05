#include "note.hpp"
#include <QDateTime>

Note::Note()
{
    id = generateId();
    title = "";
    text = "";
    label = "РАБОТА";
    created = QDateTime::currentDateTime();
    modified = created;
}

Note::Note(QString title, QString text, QString label)
{
    this->id = generateId();
    this->title = title;
    this->text = text;
    this->label = label;
    this->created = QDateTime::currentDateTime();
    this->modified = this->created;
}

void Note::setTitle(QString newTitle)
{
    title = newTitle;
    modified = QDateTime::currentDateTime();
}

void Note::setText(QString newText)
{
    text = newText;
    modified = QDateTime::currentDateTime();
}

void Note::setLabel(QString newLabel)
{
    label = newLabel;
    modified = QDateTime::currentDateTime();
}

QJsonObject Note::toJson() const
{
    QJsonObject obj;
    obj["id"] = id;
    obj["title"] = title;
    obj["text"] = text;
    obj["label"] = label;
    obj["created"] = created.toString(Qt::ISODate);
    obj["modified"] = modified.toString(Qt::ISODate);
    return obj;
}

Note Note::fromJson(QJsonObject obj)
{
    Note note;
    note.id = obj["id"].toString();
    note.title = obj["title"].toString();
    note.text = obj["text"].toString();
    note.label = obj["label"].toString();
    note.created = QDateTime::fromString(obj["created"].toString(), Qt::ISODate);
    note.modified = QDateTime::fromString(obj["modified"].toString(), Qt::ISODate);
    return note;
}

QString Note::generateId()
{
    return QString::number(QDateTime::currentMSecsSinceEpoch());
}
