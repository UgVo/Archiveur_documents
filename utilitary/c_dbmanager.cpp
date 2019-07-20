#include "c_dbmanager.h"

c_dbManager::c_dbManager(const QString& path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open())
    {
      qDebug() << "Error: connection with database fail";
    }
}

c_dbManager::c_dbManager() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

c_dbManager::~c_dbManager() {
    m_db.close();
}

bool c_dbManager::open(const QString &path) {
    m_db.setDatabaseName(path);
    if (!m_db.open())
    {
      qDebug() << "Error: connection with database fail";
      return false;
    }
    return true;
}

bool c_dbManager::add_tag(const c_tag& new_tag) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO tag (name,r_color,g_color,b_color)"
                  "VALUES (:name, :r_color, :g_color, :b_color)");
    query.bindValue(":name", new_tag.get_name());
    query.bindValue(":r_color", new_tag.get_color().red());
    query.bindValue(":g_color", new_tag.get_color().green());
    query.bindValue(":b_color", new_tag.get_color().blue());

    return query.exec();
}

bool c_dbManager::remove_tag(const QString& tag_name) {
    c_tag query_tag = get_tag(tag_name);
    if (query_tag == c_tag()) {
        return false;
    }
    remove_relations(query_tag);
    QSqlQuery query;
    query.prepare("DELETE FROM tag WHERE name = (:name)");
    query.bindValue(":name", tag_name);

    return query.exec();
}

bool c_dbManager::remove_tag(const c_tag& tag) {
    return remove_tag(tag.get_name());
}

c_tag c_dbManager::get_tag(const QString tag_name) {
    QSqlQuery query(m_db);
    QSqlQuery query_count(m_db);
    query.prepare("SELECT *  FROM tag "
                  "WHERE name = :name");
    query.bindValue(":name",tag_name);
    if (query.exec()) {
        int idName = query.record().indexOf("name");
        int idR = query.record().indexOf("r_color");
        int idG = query.record().indexOf("g_color");
        int idB = query.record().indexOf("b_color");
        if (query.next()) {
            query_count.prepare("SELECT count(name) FROM tag, tag_document_relation AS td "
                                "WHERE td.id_tag = tag.ROWID AND tag.name = :name");
            query_count.bindValue(":name",query.value(idName).toString());
            if (query_count.exec()) {
                if (query_count.next()) {
                    int idCount = query_count.record().indexOf("count(name)");
                    return c_tag(query.value(idName).toString(),QColor(query.value(idR).toInt(),query.value(idG).toInt(),query.value(idB).toInt()),query_count.value(idCount).toInt());
                }
            }
        }
    }
    else {
       qDebug() << "get_tag error:  "
                 << query.lastError();
    }
    return c_tag();
}

QVector<c_tag> c_dbManager::get_tags() {
    QSqlQuery query(m_db);
    QSqlQuery query_count(m_db);
    QVector<c_tag> tag_list;
    query.prepare("SELECT * FROM tag ");
    if (query.exec()) {
        int idName = query.record().indexOf("name");
        int idR = query.record().indexOf("r_color");
        int idG = query.record().indexOf("g_color");
        int idB = query.record().indexOf("b_color");
        while (query.next()) {
            query_count.prepare("SELECT count(name) FROM tag, tag_document_relation AS td "
                                "WHERE td.id_tag = tag.ROWID AND tag.name = :name");
            query_count.bindValue(":name",query.value(idName).toString());
            if (query_count.exec()) {
                if (query_count.next()) {
                    int idCount = query_count.record().indexOf("count(name)");
                    qDebug() << query.value(idName).toString() << " " << query_count.value(idCount).toInt();
                    tag_list.push_back(c_tag(query.value(idName).toString(),
                                             QColor(query.value(idR).toInt(),query.value(idG).toInt(),query.value(idB).toInt()),
                                             query_count.value(idCount).toInt()));
                }
            }
        }
    } else {
       qDebug() << "get_tag error:  "
                 << query.lastError();
    }
    return tag_list;
}

bool c_dbManager::add_relation(const c_document& doc, const c_tag& tag) {
    QSqlQuery query(m_db);
    query.prepare("SELECT tag.ROWID,document.ROWID from tag,document "
                  "WHERE tag.name = :name AND document.path =  :path");
    query.bindValue(":name", tag.get_name());
    query.bindValue(":path", doc.get_path());
    if (query.exec()) {
        if (query.next()) {
            int id_tag = query.value(query.record().indexOf("tag.ROWID")).toInt();
            int id_doc = query.value(query.record().indexOf("document.ROWID")).toInt();
            query.prepare("INSERT INTO tag_document_relation (id_tag,id_doc)"
                          "VALUES (:id_tag, :id_doc)");
            query.bindValue(":id_tag",id_tag);
            query.bindValue(":id_doc",id_doc);
            return query.exec();
        }
    } else {
        qDebug() << "add_relation get ROWID error:  "
                  << query.lastError();
    }
    return false;
}

bool c_dbManager::remove_relation(const c_document& doc, const c_tag& tag) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM tag_document_relation"
                  "WHERE name = :name AND path = :path");
    query.bindValue(":name", tag.get_name());
    query.bindValue(":path", doc.get_path());

    return query.exec();
}

bool c_dbManager::remove_relations(const QString& path) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM tag_document_relation"
                  "WHERE path = :path");
    query.bindValue(":path", path);

    return query.exec();
}

bool c_dbManager::remove_relations(const c_document& doc) {
    return remove_relations(doc.get_path());
}

bool c_dbManager::remove_relations(const c_tag& tag) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM tag_document_relation"
                  "WHERE name = :name");
    query.bindValue(":name", tag.get_name());

    return query.exec();
}

bool c_dbManager::add_document(const c_document& new_doc) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO document (name,path,date)"
                  "VALUES (:name, :path, :date)");
    query.bindValue(":name", new_doc.get_name());
    query.bindValue(":path", new_doc.get_path());
    query.bindValue(":date", new_doc.get_date().toString(QString("yyyy-MM-dd")));

    QVector<c_tag> tag_list = new_doc.get_tag_list();
    for (int i = 0; i < tag_list.size(); ++i) {
        add_tag(tag_list[i]);
        add_relation(new_doc,tag_list[i]);
    }
    return query.exec();
}

bool c_dbManager::remove_document(const QString& path) {
    c_document query_doc = get_document(path);
    if (query_doc == c_document()) {
        return false;
    }
    remove_relations(path);
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM document "
                  "WHERE path = :path");
    query.bindValue(":path", path);
    return query.exec();
}

bool c_dbManager::remove_document(const c_document& doc) {
    return remove_document(doc.get_path());
}

c_document c_dbManager::get_document(const QString& path) {
    c_document asked_doc;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM document "
                  "WHERE path = :path");
    query.bindValue(":path", path);
    if (query.exec()) {
        int idName = query.record().indexOf("name");
        int idDate = query.record().indexOf("date");
        if (query.next()) {
            asked_doc.set_date(QDate(query.value(idDate).toDate()));
            asked_doc.set_name(QString(query.value(idName).toString()));
            asked_doc.set_path(path);
        }
    }
    query.prepare("SELECT T.name, T.r_color, T.g_color, T.b_color "
                  "FROM tag AS T ,document AS D, tag_document_relation AS TD "
                  "WHERE T.ROWID = TD.id_tag "
                    "AND TD.id_doc = D.ROWID AND D.path = :path");
    query.bindValue(":path",path);
    if (query.exec()) {
        int idName = query.record().indexOf("name");
        int idR = query.record().indexOf("r_color");
        int idG = query.record().indexOf("g_color");
        int idB = query.record().indexOf("b_color");
        while (query.next()) {
            asked_doc.add_tag(c_tag(query.value(idName).toString(),QColor(query.value(idR).toInt(),query.value(idG).toInt(),query.value(idB).toInt())));
        }
    }
    return asked_doc;
}

QVector<c_document> c_dbManager::get_documents() {
    QVector<c_document> asked_docs;
    QSqlQuery query_doc(m_db);
    QSqlQuery query_tag(m_db);
    query_doc.prepare("SELECT * FROM document");
    if (query_doc.exec()) {
        c_document tmp;
        int idName = query_doc.record().indexOf("name");
        int idPath = query_doc.record().indexOf("path");
        int idDate = query_doc.record().indexOf("date");
        while (query_doc.next()) {
            tmp.set_date(QDate(query_doc.value(idDate).toDate()));
            tmp.set_name(QString(query_doc.value(idName).toString()));
            tmp.set_path(QString(query_doc.value(idPath).toString()));
            query_tag.prepare(  "SELECT T.name, T.r_color, T.g_color, T.b_color"
                                "FROM tag AS T ,document AS D, tag_document_relation AS TD "
                                "WHERE T.name = TD.name"
                                    "AND TD.path = D.path AND D.path = :path");
            query_tag.bindValue(":path",tmp.get_path());
            if (query_tag.exec()) {
                int idName = query_tag.record().indexOf("T.name");
                int idR = query_tag.record().indexOf("T.r_color");
                int idG = query_tag.record().indexOf("T.g_color");
                int idB = query_tag.record().indexOf("T.b_color");
                while (query_tag.next()) {
                    tmp.add_tag(c_tag(query_tag.value(idName).toString(),QColor(query_tag.value(idR).toInt(),query_tag.value(idG).toInt(),query_tag.value(idB).toInt())));
                }
            }
            asked_docs.push_back(tmp);
        }
    }
    return asked_docs;
}

