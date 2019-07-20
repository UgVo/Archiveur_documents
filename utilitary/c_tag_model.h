#ifndef C_TAG_MODEL_H
#define C_TAG_MODEL_H

#include <QAbstractTableModel>
#include <c_dbmanager.h>
#include <QBrush>
#include <QColorDialog>
#include <QMessageBox>

class c_tag_model : public QAbstractTableModel
{
    Q_OBJECT
public:
    c_tag_model(c_dbManager *bdd, QWidget* parent);
    virtual int rowCount(const QModelIndex & /*parent*/) const;
    virtual int columnCount(const QModelIndex & /*parent*/) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insert_row(c_tag new_tag);
    bool remove_row(QModelIndex tag_mID);
    c_tag* get_tag(QModelIndex mID_tag);
    QVector<c_tag> get_checked_tags();
    QModelIndex get_ModelIndex(const int row);
    void setEditable(QModelIndex index, bool editable);

signals:
    void tag_list_changed(const c_tag tag, const bool state);

private:
    QVector<c_tag> m_data_tags;
    QVector<bool> m_data_editable;
    c_dbManager* m_bdd;
    QWidget* m_parent;
};

#endif // C_TAG_MODEL_H
