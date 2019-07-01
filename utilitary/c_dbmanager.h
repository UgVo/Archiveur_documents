#ifndef C_DBMANAGER_H
#define C_DBMANAGER_H

#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <c_tag.h>
#include <c_document.h>

class c_dbManager
{
public:
    c_dbManager(const QString &path);
    c_dbManager();
    ~c_dbManager();
    bool open(const QString &path);

    bool add_tag(const c_tag& new_tag);
    bool remove_tag(const QString& tag_name);
    bool remove_tag(const c_tag& tag);
    c_tag get_tag(const QString tag_name);
    QVector<c_tag> get_tags();

    bool add_relation(const c_document& doc, const c_tag& tag);
    bool remove_relation(const c_document& doc, const c_tag& tag);
    bool remove_relations(const QString& path);
    bool remove_relations(const c_document& doc);
    bool remove_relations(const c_tag& tag);

    bool add_document(const c_document& new_doc);
    bool remove_document(const QString& path);
    bool remove_document(const c_document& doc);
    c_document get_document(const QString& path);
    QVector<c_document> get_documents();

private:
    QSqlDatabase m_db;
};

#endif // C_DBMANAGER_H
