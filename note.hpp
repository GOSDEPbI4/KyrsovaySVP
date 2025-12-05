#include <QString>
#include <QDateTime>
#include <QJsonObject>

class Note
{
public:
    Note();
    Note(QString title, QString text, QString label);

    QString getId() const { return id; }
    QString getTitle() const { return title; }
    QString getText() const { return text; }
    QString getLabel() const { return label; }
    QDateTime getCreated() const { return created; }
    QDateTime getModified() const { return modified; }

    void setTitle(QString newTitle);
    void setText(QString newText);
    void setLabel(QString newLabel);

    QJsonObject toJson() const;
    static Note fromJson(QJsonObject obj);

private:
    QString id;
    QString title;
    QString text;
    QString label;
    QDateTime created;
    QDateTime modified;

    static QString generateId();
};
