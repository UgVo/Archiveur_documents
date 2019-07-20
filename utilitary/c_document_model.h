#ifndef C_DOCUMENT_MODEL_H
#define C_DOCUMENT_MODEL_H

#include <QAbstractTableModel>
#include "c_dbmanager.h"

class c_document_model : public QAbstractTableModel
{
public:
    c_document_model(c_dbManager *bdd, QWidget* parent);
    virtual int rowCount(const QModelIndex & /*parent*/) const;
    virtual int columnCount(const QModelIndex & /*parent*/) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool insert_row(c_document new_doc);
    bool remove_row(QModelIndex doc_mID);

    c_document get_document(QModelIndex doc_mID);

private:
    QVector<c_document> m_data_documents;
    QWidget* m_parent;
    c_dbManager* m_bdd;
};

#endif // C_DOCUMENT_MODEL_H
