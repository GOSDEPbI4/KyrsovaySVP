#include "mainwindow.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , currentNoteId("")
{
    setupUI();
    loadNotes();
    updateNoteList();
}

MainWindow::~MainWindow()
{
    saveNotes();
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
    labelCombo->addItems(LABELS);

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

void MainWindow::loadNotes()
{
    QFile file(NOTES_FILE);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        return;
    }

    QJsonArray notesArray = doc.array();
    notes.clear();

    for (int i = 0; i < notesArray.size(); ++i) {
        Note note = Note::fromJson(notesArray[i].toObject());
        notes.append(note);
    }
}

void MainWindow::saveNotes()
{
    QJsonArray notesArray;

    for (const Note &note : notes) {
        notesArray.append(note.toJson());
    }

    QJsonDocument doc(notesArray);
    QFile file(NOTES_FILE);

    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    file.write(doc.toJson());
    file.close();
}

void MainWindow::updateNoteList()
{
    notesList->clear();

    QString filter = filterCombo->currentText();

    for (const Note &note : notes) {
        if (filter != "Все" && note.getLabel() != filter) {
            continue;
        }

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

    Note newNote(title, text, label);
    notes.append(newNote);

    updateNoteList();
    saveNotes();
}

void MainWindow::editNote()
{
    QListWidgetItem *item = notesList->currentItem();
    if (!item) {
        return;
    }

    QString noteId = item->data(Qt::UserRole).toString();

    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].getId() == noteId) {
            notes[i].setTitle(titleEdit->text());
            notes[i].setText(textEdit->toPlainText());
            notes[i].setLabel(labelCombo->currentText());

            updateNoteList();
            saveNotes();
            return;
        }
    }
}

void MainWindow::deleteNote()
{
    QListWidgetItem *item = notesList->currentItem();
    if (!item) {
        return;
    }

    QString noteId = item->data(Qt::UserRole).toString();

    for (int i = 0; i < notes.size(); ++i) {
        if (notes[i].getId() == noteId) {
            notes.removeAt(i);
            break;
        }
    }

    updateNoteList();
    saveNotes();
}

void MainWindow::onNoteSelected(QListWidgetItem *item)
{
    if (!item) return;

    QString noteId = item->data(Qt::UserRole).toString();

    for (const Note &note : notes) {
        if (note.getId() == noteId) {
            titleEdit->setText(note.getTitle());
            textEdit->setText(note.getText());

            int index = labelCombo->findText(note.getLabel());
            if (index != -1) {
                labelCombo->setCurrentIndex(index);
            }

            currentNoteId = noteId;
            break;
        }
    }
}

void MainWindow::filterNotes()
{
    updateNoteList();
}
