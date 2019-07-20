#ifndef C_TAG_H
#define C_TAG_H

#include <QString>
#include <QColor>
#include <QObject>
#include <QDebug>

class c_tag
{
public:
    c_tag(QString name = QString(), QColor color = QColor(0,0,0,255), int count = 0);
    QString get_name() const;
    void    set_name(QString name);
    QColor  get_color() const;
    void    set_color(QColor color);
    bool    get_state() const;
    void    set_state(bool state);
    int     get_count() const;
    void    set_count(int const count);
    bool operator ==(const c_tag &tag) const;
    bool operator !=(const c_tag &tag) const;
    bool operator <(const c_tag &tag) const;

private:
    QString m_name;
    QColor  m_color;
    bool    m_state;
    int     m_count;
};

Q_DECLARE_METATYPE(c_tag);

#endif // C_TAG_H
