#include "notemanager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

NoteManager::NoteManager()
{
    loadNotes();
}

bool NoteManager::loadNotes()
{
    QFile file(NOTES_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return false;
    }

    QJsonArray notesArray = doc.array();
    notes.clear();

    for (int i = 0; i < notesArray.size(); ++i) {
        Note note = Note::fromJson(notesArray[i].toObject());
        notes.append(note);
    }

    return true;
}

bool NoteManager::saveNotes()
{
    QJsonArray notesArray;

    for (const Note &note : notes) {
        notesArray.append(note.toJson());
    }

    QJsonDocument doc(notesArray);
    QFile file(NOTES_FILE);

    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

void NoteManager::addNote(QString title, QString text, QString label)
{
    Note newNote(title, text, label);
    notes.append(newNote);
    saveNotes();
}

bool NoteManager::updateNote(QString id, QString title, QString text, QString label)
{
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].getId() == id) {
            notes[i].setTitle(title);
            notes[i].setText(text);
            notes[i].setLabel(label);
            saveNotes();
            return true;
        }
    }
    return false;
}

bool NoteManager::deleteNote(QString id)
{
    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].getId() == id) {
            notes.removeAt(i);
            saveNotes();
            return true;
        }
    }
    return false;
}

QList<Note> NoteManager::getNotesByLabel(QString label) const
{
    if (label == "Все") {
        return notes;
    }

    QList<Note> filtered;
    for (const Note &note : notes) {
        if (note.getLabel() == label) {
            filtered.append(note);
        }
    }
    return filtered;
}
