#ifndef C_ADD_DOCUMENT_H
#define C_ADD_DOCUMENT_H

#include <QDialog>
#include <c_dbmanager.h>
#include <c_tag_model.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QUrl>
#include <QFontMetrics>
#include <QMenu>
#include <QPushButton>
#include <QGridLayout>
#include <QBoxLayout>
#include <QMouseEvent>

namespace Ui {
class c_add_document;
}

class c_add_document : public QDialog
{
    Q_OBJECT

public:
    explicit c_add_document(c_dbManager *bdd, QWidget *parent = nullptr);
    ~c_add_document();
    QVector<c_tag> get_checked_tags();
    c_document get_new_document();

public slots:
    void on_update_tag_list(const c_tag tag, const bool state);
    void on_accepted();
    void on_search_button();
    void customMenuRequested(QPoint pos);
    void on_edit_name();
    void on_edit_color();
    void on_delete_tag();

private:
    Ui::c_add_document *ui;
    c_dbManager* m_bdd;
    c_tag_model* tag_model;
    QVector<c_tag> active_tags;
    QVector<QPushButton*> active_tag_buttons;
    QModelIndex m_tmp_tag_index;
    QPoint m_tmp_pos;
    QGridLayout *tag_list_grid;
};

#endif // C_ADD_DOCUMENT_H
