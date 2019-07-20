#include "c_document_model.h"

c_document_model::c_document_model(c_dbManager *bdd, QWidget* parent)
{
    m_bdd = bdd;
    m_parent = parent;
    QVector<c_document> documents = m_bdd->get_documents();
    for (QVector<c_document>::iterator it = documents.begin(); it != documents.end(); ++it) {
        insert_row(*it);
    }
}

int c_document_model::rowCount(const QModelIndex & /*parent*/) const {
    return m_data_documents.size();
}

int c_document_model::columnCount(const QModelIndex & /*parent*/) const {
    return 3;
}

QVariant c_document_model::data(const QModelIndex &index, int role) const {
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column())
        {
        case 0:
            return m_data_documents[index.row()].get_name();
        case 1:
            return m_data_documents[index.row()].get_date();
        case 2:
            return m_data_documents[index.row()].get_path();
        }
    }
    return QVariant();
}

QVariant c_document_model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return tr("Name");
            case 1:
                return tr("Date");
            case 2:
                return tr("path");
            }
        }
    }
    return QVariant();
}

bool c_document_model::insert_row(c_document new_doc) {
    beginInsertRows(QModelIndex(), m_data_documents.size(),m_data_documents.size());
    m_data_documents.push_back(new_doc);
    endInsertRows();
    return true;
}

bool c_document_model::remove_row(QModelIndex doc_mID) {
    beginRemoveRows(QModelIndex(), doc_mID.row(),doc_mID.row());
    m_data_documents.remove(doc_mID.row());
    endRemoveRows();
    return true;
}

c_document c_document_model::get_document(QModelIndex doc_mID)  {

}
