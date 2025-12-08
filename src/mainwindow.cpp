#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , currentNoteId("")
{
    noteManager = new NoteManager();
    setupUI();
    updateNoteList();
}

MainWindow::~MainWindow()
{
    delete noteManager;
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLabel = new QLabel("Фильтр:", this);
    filterCombo = new QComboBox(this);
    filterCombo->addItems(QStringList() << "Все" << "РАБОТА" << "ЛИЧНОЕ" << "УЧЁБА");

    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterCombo);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    QVBoxLayout *listLayout = new QVBoxLayout();
    QLabel *listTitle = new QLabel("Заметки:", this);
    notesList = new QListWidget(this);

    listLayout->addWidget(listTitle);
    listLayout->addWidget(notesList);
    contentLayout->addLayout(listLayout, 1);

    QVBoxLayout *formLayout = new QVBoxLayout();

    QLabel *titleLabel = new QLabel("Название:", this);
    titleEdit = new QLineEdit(this);

    QLabel *textLabel = new QLabel("Текст:", this);
    textEdit = new QTextEdit(this);

    QLabel *labelLabel = new QLabel("Лейбл:", this);
    labelCombo = new QComboBox(this);
    labelCombo->addItems(noteManager->getAvailableLabels());

    formLayout->addWidget(titleLabel);
    formLayout->addWidget(titleEdit);
    formLayout->addWidget(textLabel);
    formLayout->addWidget(textEdit);
    formLayout->addWidget(labelLabel);
    formLayout->addWidget(labelCombo);

    contentLayout->addLayout(formLayout, 2);
    mainLayout->addLayout(contentLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    addButton = new QPushButton("Добавить", this);
    editButton = new QPushButton("Изменить", this);
    deleteButton = new QPushButton("Удалить", this);

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::addNewNote);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editNote);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteNote);
    connect(notesList, &QListWidget::itemClicked, this, &MainWindow::onNoteSelected);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::filterNotes);

    setWindowTitle("Заметки");
    resize(800, 600);
}

void MainWindow::updateNoteList()
{
    notesList->clear();

    QString filter = filterCombo->currentText();
    QList<Note> notesToShow;

    if (filter == "Все") {
        notesToShow = noteManager->getAllNotes();
    } else {
        notesToShow = noteManager->getNotesByLabel(filter);
    }

    for (const Note &note : notesToShow) {
        QString displayText = QString("%1 [%2]")
        .arg(note.getTitle())
            .arg(note.getLabel());

        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, note.getId());
        notesList->addItem(item);
    }
}

void MainWindow::addNewNote()
{
    QString title = titleEdit->text().trimmed();
    QString text = textEdit->toPlainText().trimmed();
    QString label = labelCombo->currentText();

    if (title.isEmpty()) {
        return;
    }

    noteManager->addNote(title, text, label);
    clearForm();
    updateNoteList();
}

void MainWindow::editNote()
{
    QListWidgetItem *item = notesList->currentItem();
    if (!item) {
        return;
    }

    QString noteId = item->data(Qt::UserRole).toString();
    QString title = titleEdit->text();
    QString text = textEdit->toPlainText();
    QString label = labelCombo->currentText();

    noteManager->updateNote(noteId, title, text, label);
    updateNoteList();
}

void MainWindow::deleteNote()
{
    QListWidgetItem *item = notesList->currentItem();
    if (!item) {
        return;
    }

    QString noteId = item->data(Qt::UserRole).toString();

    noteManager->deleteNote(noteId);
    clearForm();
    updateNoteList();
}

void MainWindow::onNoteSelected(QListWidgetItem *item)
{
    if (!item) return;

    QString noteId = item->data(Qt::UserRole).toString();
    currentNoteId = noteId;

    QList<Note> allNotes = noteManager->getAllNotes();
    for (const Note &note : allNotes) {
        if (note.getId() == noteId) {
            titleEdit->setText(note.getTitle());
            textEdit->setText(note.getText());

            int index = labelCombo->findText(note.getLabel());
            if (index != -1) {
                labelCombo->setCurrentIndex(index);
            }
            break;
        }
    }
}

void MainWindow::filterNotes()
{
    updateNoteList();
}

void MainWindow::clearForm()
{
    titleEdit->clear();
    textEdit->clear();
    labelCombo->setCurrentIndex(0);
    currentNoteId = "";
}
