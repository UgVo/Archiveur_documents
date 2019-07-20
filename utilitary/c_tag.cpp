#include "c_tag.h"

c_tag::c_tag(QString name, QColor color, int count) : m_color(color) {
    m_name = name.toLower();
    m_state = false;
    m_count = count;
}

QString c_tag::get_name() const{
    return m_name;
}

void c_tag::set_name(QString name) {
    m_name = name.toLower();
}

QColor c_tag::get_color() const {
    return m_color;
}

void c_tag::set_color(QColor color) {
    m_color = color;
}

bool c_tag::get_state() const {
    return m_state;
}

void c_tag::set_state(bool state) {
    m_state = state;
}

int c_tag::get_count() const {
    return m_count;
}
void c_tag::set_count(int const count) {
    m_count = count;
}

bool c_tag::operator ==(const c_tag& tag) const {
    return (m_color == tag.get_color() && m_name == tag.get_name());
}

bool c_tag::operator !=(const c_tag &tag) const {
    return !(*this == tag);
}

bool c_tag::operator <(const c_tag &tag) const {
    return this->m_count == tag.get_count() ? this->m_name < tag.get_name() : this->m_count > tag.get_count();
}
