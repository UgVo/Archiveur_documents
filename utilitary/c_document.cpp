#include "c_document.h"

c_document::c_document(QString name, QString path, QVector<c_tag> tag_list, QDate date)
    : m_name(name), m_path(path), m_date(date) {
    for (int i = 0; i < tag_list.size(); ++i) {
        add_tag(tag_list[i]);
    }
    qSort(m_tag_list);
}

QString c_document::get_name() const {
    return m_name;
}

void c_document::set_name(QString name) {
    m_name = name;
}

QString c_document::get_path() const {
    return m_path;
}

void c_document::set_path(QString path) {
    m_path = path;
}

QVector<c_tag> c_document::get_tag_list() const {
    return m_tag_list;
}

void c_document::set_tag_list(QVector<c_tag> tag_list) {
    m_tag_list = tag_list;
}

void c_document::add_tag(c_tag new_tag) {
    if (m_tag_list.indexOf(new_tag) == -1) {
        new_tag.set_count(0); // Set count to 0 so that the sorting process only takes in concideration the alphabetic order.
        m_tag_list.push_back(new_tag);
    }
}

QDate c_document::get_date() const {
    return m_date;
}

void c_document::set_date(QDate date) {
    m_date = date;
}

bool c_document::operator ==(const c_document &doc) const {
    return (m_name == doc.get_name() &&
            m_path == doc.get_path() &&
            m_date == doc.get_date() &&
            m_tag_list == doc.get_tag_list());
}

bool c_document::operator <(const c_document &doc) const {
    return m_name < doc.get_name();
}
