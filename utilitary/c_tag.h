#ifndef C_TAG_H
#define C_TAG_H

#include <QString>
#include <QColor>

class c_tag
{
public:
    c_tag(QString name = QString(), QColor color = QColor(0,0,0,255));
    QString get_name() const;
    void    set_name(QString name);
    QColor  get_color() const;
    void    set_color(QColor color);
    bool operator ==(const c_tag &tag) const;
    bool operator !=(const c_tag &tag) const;
    bool operator <(const c_tag &tag) const;

private:
    QString m_name;
    QColor  m_color;
};

#endif // C_TAG_H
