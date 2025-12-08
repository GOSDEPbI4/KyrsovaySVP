#include <QObject>
#include <QtTest>
#include <QSignalSpy>
#include "../../src/notemanager.hpp"

class NoteManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testInitialState();
    void testAddNote();
    void testUpdateNote();
    void testDeleteNote();
    void testGetAllNotes();
    void testGetNotesByLabel();
    void testFileOperations();
    void testAvailableLabels();
    void testMultipleOperations();
    void testNoteRetrieval();
    void testEmptyOperations();
    void testBoundaryCases();
    void testLabelFiltering();

private:
    NoteManager* m_manager;
    QString m_testFileName;

    void removeTestFile();
};
