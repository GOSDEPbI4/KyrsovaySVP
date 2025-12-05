#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QList>
#include "note.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addNewNote();
    void editNote();
    void deleteNote();
    void onNoteSelected(QListWidgetItem *item);
    void filterNotes();

private:
    void setupUI();
    void loadNotes();
    void saveNotes();
    void updateNoteList();

    QListWidget *notesList;
    QLineEdit *titleEdit;
    QTextEdit *textEdit;
    QComboBox *labelCombo;
    QComboBox *filterCombo;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QList<Note> notes;
    QString currentNoteId;

    const QString NOTES_FILE = "notes.json";
    const QStringList LABELS = {"РАБОТА", "ЛИЧНОЕ", "УЧЁБА"};
};
