#include "note.hpp"
#include <QList>
#include <QString>

class NoteManager
{
public:
    NoteManager();

    bool loadNotes();
    bool saveNotes();

    void addNote(QString title, QString text, QString label);
    bool updateNote(QString id, QString title, QString text, QString label);
    bool deleteNote(QString id);

    QList<Note> getAllNotes() const { return notes; }
    QList<Note> getNotesByLabel(QString label) const;

    QStringList getAvailableLabels() const { return QStringList() << "РАБОТА" << "ЛИЧНОЕ" << "УЧЁБА"; }

private:
    QList<Note> notes;
    const QString NOTES_FILE = "notes.json";
};
