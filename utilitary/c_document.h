#ifndef C_DOCUMENT_H
#define C_DOCUMENT_H

#include <QString>
#include <QDate>
#include <QVector>
#include <QSet>
#include "c_tag.h"

class c_document
{
public:
    c_document(QString name = QString(), QString path = QString(), QVector<c_tag> tag_list = QVector<c_tag>(), QDate date = QDate::currentDate());
    QString         get_name() const;
    void            set_name(QString name);
    QString         get_path() const;
    void            set_path(QString path);
    QVector<c_tag>  get_tag_list() const;
    void            set_tag_list(QVector<c_tag> tag_list);
    void            add_tag(c_tag new_tag);
    QDate           get_date() const;
    void            set_date(QDate date);

    bool operator ==(const c_document &doc) const;
    bool operator <(const c_document &doc) const;

private:
    QString         m_name;
    QString         m_path;
    QVector<c_tag>  m_tag_list;
    QDate           m_date;
};

#endif // C_DOCUMENT_H
