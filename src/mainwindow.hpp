#include "notemanager.hpp"
#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

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
    void updateNoteList();
    void clearForm();

    NoteManager *noteManager;

    QListWidget *notesList;
    QLineEdit *titleEdit;
    QTextEdit *textEdit;
    QComboBox *labelCombo;
    QComboBox *filterCombo;

    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    QString currentNoteId;
};
