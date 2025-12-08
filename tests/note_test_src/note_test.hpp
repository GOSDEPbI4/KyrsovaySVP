#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include "../../src/note.hpp"

class NoteTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDefaultConstructor();
    void testParameterizedConstructor();
    void testSettersAndGetters();
    void testJsonSerialization();
    void testUniqueIds();
    void testModifiedTime();
    void testLabelAssignment();
    void testEmptyNote();
    void testNoteCopy();
    void testNoteComparison();

private:
    Note* m_note;
};
