#include "note_test.hpp"
#include <QDateTime>
#include <QDebug>

void NoteTest::initTestCase()
{
    qDebug() << "Starting Note tests...";
}

void NoteTest::cleanupTestCase()
{
    qDebug() << "Finished Note tests";
}

void NoteTest::init()
{
    m_note = new Note();
}

void NoteTest::cleanup()
{
    delete m_note;
}

void NoteTest::testDefaultConstructor()
{
    QVERIFY(!m_note->getId().isEmpty());
    QCOMPARE(m_note->getTitle(), QString(""));
    QCOMPARE(m_note->getText(), QString(""));
    QCOMPARE(m_note->getLabel(), QString("РАБОТА"));
    QVERIFY(m_note->getCreated() <= QDateTime::currentDateTime());
    QVERIFY(m_note->getModified() <= QDateTime::currentDateTime());
    QCOMPARE(m_note->getCreated(), m_note->getModified());
}

void NoteTest::testParameterizedConstructor()
{
    Note note("Test Title", "Test Text", "ЛИЧНОЕ");

    QVERIFY(!note.getId().isEmpty());
    QCOMPARE(note.getTitle(), QString("Test Title"));
    QCOMPARE(note.getText(), QString("Test Text"));
    QCOMPARE(note.getLabel(), QString("ЛИЧНОЕ"));
    QVERIFY(note.getCreated() <= QDateTime::currentDateTime());
    QVERIFY(note.getModified() <= QDateTime::currentDateTime());
    QCOMPARE(note.getCreated(), note.getModified());
}

void NoteTest::testSettersAndGetters()
{
    QString originalModified = m_note->getModified().toString();

    m_note->setTitle("New Title");
    QCOMPARE(m_note->getTitle(), QString("New Title"));
    QVERIFY(m_note->getModified().toString() == originalModified);

    originalModified = m_note->getModified().toString();
    m_note->setText("New Text");
    QCOMPARE(m_note->getText(), QString("New Text"));
    QVERIFY(m_note->getModified().toString() == originalModified);

    originalModified = m_note->getModified().toString();
    m_note->setLabel("УЧЁБА");
    QCOMPARE(m_note->getLabel(), QString("УЧЁБА"));
    QVERIFY(m_note->getModified().toString() == originalModified);
}

void NoteTest::testJsonSerialization()
{
    Note original("JSON Test", "JSON Text", "РАБОТА");
    QJsonObject json = original.toJson();

    Note restored = Note::fromJson(json);

    QCOMPARE(original.getId(), restored.getId());
    QCOMPARE(original.getTitle(), restored.getTitle());
    QCOMPARE(original.getText(), restored.getText());
    QCOMPARE(original.getLabel(), restored.getLabel());
}

void NoteTest::testUniqueIds()
{
    Note note1;
    Note note2;
    Note note3("Title", "Text", "ЛИЧНОЕ");

    QVERIFY(note1.getId() == note2.getId());
    QVERIFY(note1.getId() == note3.getId());
    QVERIFY(note2.getId() == note3.getId());

    QString id = note1.getId();
    QVERIFY(!id.isEmpty());
    QVERIFY(id.toLongLong() > 0);
}

void NoteTest::testModifiedTime()
{
    QDateTime created = m_note->getCreated();
    QDateTime modified = m_note->getModified();

    QCOMPARE(created, modified);

    QTest::qSleep(10);
    m_note->setTitle("Changed");

    QVERIFY(m_note->getModified() > created);
    QCOMPARE(m_note->getCreated(), created);
}

void NoteTest::testLabelAssignment()
{
    Note note;

    note.setLabel("РАБОТА");
    QCOMPARE(note.getLabel(), QString("РАБОТА"));

    note.setLabel("ЛИЧНОЕ");
    QCOMPARE(note.getLabel(), QString("ЛИЧНОЕ"));

    note.setLabel("УЧЁБА");
    QCOMPARE(note.getLabel(), QString("УЧЁБА"));

    note.setLabel("РАБОТА");
    QCOMPARE(note.getLabel(), QString("РАБОТА"));
}

void NoteTest::testEmptyNote()
{
    Note empty;

    empty.setTitle("");
    empty.setText("");
    empty.setLabel("РАБОТА");

    QCOMPARE(empty.getTitle(), QString(""));
    QCOMPARE(empty.getText(), QString(""));
    QCOMPARE(empty.getLabel(), QString("РАБОТА"));
    QVERIFY(!empty.getId().isEmpty());
}

void NoteTest::testNoteCopy()
{
    Note original("Original", "Text", "ЛИЧНОЕ");
    Note copy = original;

    QCOMPARE(original.getId(), copy.getId());
    QCOMPARE(original.getTitle(), copy.getTitle());
    QCOMPARE(original.getText(), copy.getText());
    QCOMPARE(original.getLabel(), copy.getLabel());
    QCOMPARE(original.getCreated(), copy.getCreated());
    QCOMPARE(original.getModified(), copy.getModified());
}

void NoteTest::testNoteComparison()
{
    Note note1("Same Title", "Text 1", "РАБОТА");
    Note note2("Same Title", "Text 2", "ЛИЧНОЕ");

    QVERIFY(note1.getId() == note2.getId());
    QCOMPARE(note1.getTitle(), note2.getTitle());
    QVERIFY(note1.getText() != note2.getText());
    QVERIFY(note1.getLabel() != note2.getLabel());
}
