#include "notemanager_test.hpp"
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>

void NoteManagerTest::initTestCase()
{
    qDebug() << "Starting NoteManager tests...";
    m_testFileName = "test_notes.json";

    removeTestFile();
}

void NoteManagerTest::cleanupTestCase()
{
    removeTestFile();
    qDebug() << "Finished NoteManager tests";
}

void NoteManagerTest::init()
{
    m_manager = new NoteManager();

    QFile notesFile("notes.json");
    if (notesFile.exists()) {
        notesFile.remove();
    }
}

void NoteManagerTest::cleanup()
{
    delete m_manager;

    QFile notesFile("notes.json");
    if (notesFile.exists()) {
        notesFile.remove();
    }
}

void NoteManagerTest::testInitialState()
{
    QList<Note> notes = m_manager->getAllNotes();
    QVERIFY(notes.isEmpty());

    QStringList labels = m_manager->getAvailableLabels();
    QCOMPARE(labels.size(), 3);
    QVERIFY(labels.contains("РАБОТА"));
    QVERIFY(labels.contains("ЛИЧНОЕ"));
    QVERIFY(labels.contains("УЧЁБА"));
}

void NoteManagerTest::testAddNote()
{
    int initialCount = m_manager->getAllNotes().size();

    m_manager->addNote("Test Note 1", "Text 1", "РАБОТА");
    QList<Note> notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), initialCount + 1);

    m_manager->addNote("Test Note 2", "Text 2", "ЛИЧНОЕ");
    notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), initialCount + 2);

    Note lastNote = notes.last();
    QCOMPARE(lastNote.getTitle(), QString("Test Note 2"));
    QCOMPARE(lastNote.getText(), QString("Text 2"));
    QCOMPARE(lastNote.getLabel(), QString("ЛИЧНОЕ"));
}

void NoteManagerTest::testUpdateNote()
{
    m_manager->addNote("Original Title", "Original Text", "РАБОТА");
    QList<Note> notes = m_manager->getAllNotes();
    QString noteId = notes.first().getId();

    bool result = m_manager->updateNote(noteId, "Updated Title", "Updated Text", "УЧЁБА");
    QVERIFY(result);

    notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 1);
    QCOMPARE(notes.first().getTitle(), QString("Updated Title"));
    QCOMPARE(notes.first().getText(), QString("Updated Text"));
    QCOMPARE(notes.first().getLabel(), QString("УЧЁБА"));

    bool invalidResult = m_manager->updateNote("invalid_id", "Title", "Text", "РАБОТА");
    QVERIFY(!invalidResult);
}

void NoteManagerTest::testDeleteNote()
{
    m_manager->addNote("Note 1", "Text 1", "РАБОТА");
    m_manager->addNote("Note 2", "Text 2", "ЛИЧНОЕ");

    QList<Note> notes = m_manager->getAllNotes();
    int initialCount = notes.size();
    QString noteId = notes.first().getId();

    bool result = m_manager->deleteNote(noteId);
    QVERIFY(result);

    notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), initialCount - 1);

    bool invalidResult = m_manager->deleteNote("invalid_id");
    QVERIFY(!invalidResult);
}

void NoteManagerTest::testGetAllNotes()
{
    m_manager->addNote("Work Note", "Work text", "РАБОТА");
    m_manager->addNote("Personal Note", "Personal text", "ЛИЧНОЕ");
    m_manager->addNote("Study Note", "Study text", "УЧЁБА");

    QList<Note> notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 3);

    bool hasWork = false;
    bool hasPersonal = false;
    bool hasStudy = false;

    for (const Note& note : notes) {
        if (note.getLabel() == "РАБОТА") hasWork = true;
        if (note.getLabel() == "ЛИЧНОЕ") hasPersonal = true;
        if (note.getLabel() == "УЧЁБА") hasStudy = true;
    }

    QVERIFY(hasWork);
    QVERIFY(hasPersonal);
    QVERIFY(hasStudy);
}

void NoteManagerTest::testGetNotesByLabel()
{
    m_manager->addNote("Work 1", "Text", "РАБОТА");
    m_manager->addNote("Work 2", "Text", "РАБОТА");
    m_manager->addNote("Personal 1", "Text", "ЛИЧНОЕ");
    m_manager->addNote("Study 1", "Text", "УЧЁБА");
    m_manager->addNote("Work 3", "Text", "РАБОТА");

    QList<Note> workNotes = m_manager->getNotesByLabel("РАБОТА");
    QCOMPARE(workNotes.size(), 3);

    QList<Note> personalNotes = m_manager->getNotesByLabel("ЛИЧНОЕ");
    QCOMPARE(personalNotes.size(), 1);

    QList<Note> studyNotes = m_manager->getNotesByLabel("УЧЁБА");
    QCOMPARE(studyNotes.size(), 1);

    QList<Note> allNotes = m_manager->getNotesByLabel("Все");
    QCOMPARE(allNotes.size(), 5);

    QList<Note> invalidLabelNotes = m_manager->getNotesByLabel("INVALID");
    QVERIFY(invalidLabelNotes.isEmpty());
}

void NoteManagerTest::testFileOperations()
{
    m_manager->addNote("Saved Note", "This should be saved", "РАБОТА");

    NoteManager newManager;
    QList<Note> notes = newManager.getAllNotes();

    QVERIFY(!notes.isEmpty());
    QCOMPARE(notes.first().getTitle(), QString("Saved Note"));
    QCOMPARE(notes.first().getText(), QString("This should be saved"));
    QCOMPARE(notes.first().getLabel(), QString("РАБОТА"));
}

void NoteManagerTest::testAvailableLabels()
{
    QStringList labels = m_manager->getAvailableLabels();

    QCOMPARE(labels.size(), 3);
    QVERIFY(labels.contains("РАБОТА"));
    QVERIFY(labels.contains("ЛИЧНОЕ"));
    QVERIFY(labels.contains("УЧЁБА"));

    QCOMPARE(labels[0], QString("РАБОТА"));
    QCOMPARE(labels[1], QString("ЛИЧНОЕ"));
    QCOMPARE(labels[2], QString("УЧЁБА"));
}

void NoteManagerTest::testMultipleOperations()
{
    for (int i = 0; i < 10; i++) {
        m_manager->addNote(QString("Note %1").arg(i),
                           QString("Text %1").arg(i),
                           i % 3 == 0 ? "РАБОТА" : (i % 3 == 1 ? "ЛИЧНОЕ" : "УЧЁБА"));
    }

    QList<Note> notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 10);

    QString noteId = notes[5].getId();
    bool updateResult = m_manager->updateNote(noteId, "Updated", "Updated Text", "РАБОТА");
    QVERIFY(updateResult);

    bool deleteResult = m_manager->deleteNote(noteId);
    QVERIFY(deleteResult);

    notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 9);

    bool found = false;
    for (const Note& note : notes) {
        if (note.getId() == noteId) {
            found = true;
            break;
        }
    }
    QVERIFY(!found);
}

void NoteManagerTest::testNoteRetrieval()
{
    m_manager->addNote("Note A", "Text A", "РАБОТА");
    m_manager->addNote("Note B", "Text B", "ЛИЧНОЕ");
    m_manager->addNote("Note C", "Text C", "УЧЁБА");

    QList<Note> allNotes = m_manager->getAllNotes();
    QCOMPARE(allNotes.size(), 3);

    for (const Note& note : allNotes) {
        QVERIFY(!note.getId().isEmpty());
        QVERIFY(!note.getTitle().isEmpty());
        QVERIFY(!note.getText().isEmpty());
        QVERIFY(!note.getLabel().isEmpty());
        QVERIFY(note.getCreated() <= QDateTime::currentDateTime());
        QVERIFY(note.getModified() <= QDateTime::currentDateTime());
    }
}

void NoteManagerTest::testEmptyOperations()
{
    bool result = m_manager->updateNote("non_existent_id", "Title", "Text", "РАБОТА");
    QVERIFY(!result);

    result = m_manager->deleteNote("non_existent_id");
    QVERIFY(!result);

    QList<Note> notes = m_manager->getAllNotes();
    QVERIFY(notes.isEmpty());

    QList<Note> filteredNotes = m_manager->getNotesByLabel("РАБОТА");
    QVERIFY(filteredNotes.isEmpty());
}

void NoteManagerTest::testBoundaryCases()
{
    m_manager->addNote("", "", "РАБОТА");
    QList<Note> notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 1);
    QCOMPARE(notes.first().getTitle(), QString(""));
    QCOMPARE(notes.first().getText(), QString(""));

    m_manager->addNote("Very Long Title Very Long Title Very Long Title Very Long Title",
                       "Very Long Text Very Long Text Very Long Text Very Long Text Very Long Text",
                       "ЛИЧНОЕ");
    notes = m_manager->getAllNotes();
    QCOMPARE(notes.size(), 2);
}

void NoteManagerTest::testLabelFiltering()
{
    for (int i = 0; i < 5; i++) {
        m_manager->addNote(QString("Work %1").arg(i), "Text", "РАБОТА");
    }
    for (int i = 0; i < 3; i++) {
        m_manager->addNote(QString("Personal %1").arg(i), "Text", "ЛИЧНОЕ");
    }
    for (int i = 0; i < 2; i++) {
        m_manager->addNote(QString("Study %1").arg(i), "Text", "УЧЁБА");
    }

    QList<Note> workNotes = m_manager->getNotesByLabel("РАБОТА");
    QCOMPARE(workNotes.size(), 5);

    QList<Note> personalNotes = m_manager->getNotesByLabel("ЛИЧНОЕ");
    QCOMPARE(personalNotes.size(), 3);

    QList<Note> studyNotes = m_manager->getNotesByLabel("УЧЁБА");
    QCOMPARE(studyNotes.size(), 2);

    QList<Note> allNotes = m_manager->getNotesByLabel("Все");
    QCOMPARE(allNotes.size(), 10);
}


void NoteManagerTest::removeTestFile()
{
    QFile file(m_testFileName);
    if (file.exists()) {
        file.remove();
    }
}
