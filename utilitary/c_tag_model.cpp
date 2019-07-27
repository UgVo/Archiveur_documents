#include "c_tag_model.h"

c_tag_model::c_tag_model(c_dbManager *bdd, QWidget *parent) {
    m_bdd = bdd;
    m_parent = parent;
    QVector<c_tag> tags = m_bdd->get_tags();
    for (QVector<c_tag>::iterator it = tags.begin(); it != tags.end(); ++it) {
        insert_row(*it);
    }
    insert_row(c_tag(QString(),QColor(),-1));
}

int c_tag_model::rowCount(const QModelIndex & /*parent*/) const {
    return m_data_tags.size();
}

int c_tag_model::columnCount(const QModelIndex & /*parent*/) const {
    return 1;
}

QVariant c_tag_model::data(const QModelIndex &index, int role) const {

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column())
        {
        case 0:
            return m_data_tags[index.row()].get_name();
        default:
            break;
        }
        break;
    case Qt::CheckStateRole:  // this shows the checkbox
        if (index.row() != m_data_tags.size()-1) {
            if (m_data_tags[index.row()].get_state())
                    return Qt::Checked;
            else
                    return Qt::Unchecked;
        }
        break;
    case Qt::ForegroundRole:
        return QBrush(m_data_tags[index.row()].get_color());

    default:
        break;
    }
    return QVariant();
}

bool c_tag_model::setData(const QModelIndex &index, const QVariant &value, int role) {
    bool success = true;
    bool flag_exit = 0;
    if (index.isValid()) {
        c_tag* tag = get_tag(index);
        if (role == Qt::CheckStateRole) {
            tag->set_state(value.toBool());
            emit tag_list_changed(*tag,value.toBool());
            QModelIndex topLeft = index;
            QModelIndex bottomRight = index;
            emit dataChanged(topLeft, bottomRight);
            success = true;
        }
        if (tag->get_count() == -1) {
            if (role == Qt::EditRole) {
                if (value.toString().isEmpty()) return success;
                QColorDialog color_dialog;
                if (color_dialog.exec()) {
                    QColor tag_color = color_dialog.selectedColor();
                    if (tag_color.lightness() > 190) {
                        while (tag_color.lightness() > 190 && !flag_exit) {
                            QMessageBox::warning(m_parent,tr("Wrong color"),tr("the ligthness of the chosen color is to low to be easily seen"));
                            flag_exit = color_dialog.exec();
                            tag_color = color_dialog.selectedColor();
                        }
                        if (flag_exit != QDialog::Accepted) {
                            return success;
                        }
                    }
                    tag->set_name(value.toString());
                    tag->set_color(tag_color);
                    tag->set_count(0);
                    if(m_bdd->add_tag(*tag)) {
                        setEditable(index,false);
                        insert_row(c_tag(QString(),QColor(),-1));
                    }
                    else {
                        QString message("The tag name " + tag->get_name() + " is already taken, choose an other on to create a new tag");
                        QMessageBox::warning(m_parent,tr("Tag already exists"),tr(message.toStdString().c_str()));
                        tag->set_name(QString());
                        tag->set_count(-1);
                    }
                }
            }
        }
        else {
            if (role == Qt::EditRole) {
                if(m_bdd->update_tag_name(*tag, value.toString())) {
                    tag->set_name(value.toString());
                    setEditable(index,false);
                }
            }
        }
    }
    for (int i = 0; i < m_data_editable.size(); ++i) {
        qDebug() << i << " : " << QString::number(m_data_editable[i]);
    }
    return success;
}

QVariant c_tag_model::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return tr("Name");
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags c_tag_model::flags(const QModelIndex &index) const {
    Qt::ItemFlags flags;
    flags |= Qt::ItemIsSelectable;
    flags |= Qt::ItemIsEnabled;
    if (m_data_editable[index.row()])
        flags |= Qt::ItemIsEditable;
    else
        flags |= Qt::ItemIsUserCheckable;

    return flags;
}

bool c_tag_model::insert_row(c_tag new_tag) {
    beginInsertRows(QModelIndex(),  m_data_tags.size(), m_data_tags.size());
    m_data_tags.push_back(new_tag);
    m_data_editable.push_back(new_tag.get_count()==-1);
    std::sort(m_data_tags.begin(),m_data_tags.end());//.begin(),m_data_tags.end());
    endInsertRows();
    return true;
}

bool c_tag_model::remove_row(QModelIndex tag_mID) {
    beginRemoveRows(QModelIndex(), tag_mID.row(),tag_mID.row());
    m_data_tags.remove(tag_mID.row());
    m_data_editable.removeAt(tag_mID.row());
    endRemoveRows();
    return true;
}

c_tag* c_tag_model::get_tag(QModelIndex mID_tag) {
    return &m_data_tags[mID_tag.row()];
}

QVector<c_tag> c_tag_model::get_checked_tags() {
    QVector<c_tag> tag_list;
    for (QVector<c_tag>::iterator it = m_data_tags.begin(); it != m_data_tags.end(); ++it) {
        if (it->get_state()) {
            tag_list.push_back(*it);
        }
    }
    return tag_list;
}

void c_tag_model::setEditable(QModelIndex index, bool editable) {
    m_data_editable[index.row()] = editable;
}
