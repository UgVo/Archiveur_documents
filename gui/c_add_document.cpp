#include "c_add_document.h"
#include "ui_c_add_document.h"

c_add_document::c_add_document(c_dbManager *bdd, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::c_add_document)
{
    ui->setupUi(this);

    m_bdd = bdd;
    active_tags = QVector<c_tag>();

    tag_model = new c_tag_model(bdd,this);

    ui->tableView->setModel(tag_model);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setShowGrid(true);
    ui->tableView->sortByColumn(0,Qt::AscendingOrder);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->dateEdit->setDate(QDate::currentDate());
    QObject::connect(tag_model, SIGNAL(tag_list_changed(c_tag,bool)), this, SLOT(on_update_tag_list(c_tag,bool)));
    QObject::connect(ui->buttonBox, SIGNAL(accepted()),this,SLOT(on_accepted()));
    QObject::connect(ui->search_button, SIGNAL(clicked()),this,SLOT(on_search_button()));
    QObject::connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(customMenuRequested(QPoint)));


    tag_list_grid = ui->active_tag_buttons;
    tag_list_grid->setSpacing(1);
    tag_list_grid->setVerticalSpacing(4);
}

c_add_document::~c_add_document()
{
    delete ui;
}

QVector<c_tag> c_add_document::get_checked_tags() {
    return tag_model->get_checked_tags();
}

c_document c_add_document::get_new_document() {
    c_document new_doc;
    new_doc.set_date(ui->dateEdit->date());
    new_doc.set_name(ui->name_lineEdit->text());
    new_doc.set_path(ui->path_lineEdit->text());
    new_doc.set_tag_list(active_tags);
    return new_doc;
}

void c_add_document::on_update_tag_list(const c_tag tag, const bool state) {
    for (int i = 0; i < active_tag_buttons.size(); ++i) {
        tag_list_grid->removeWidget(active_tag_buttons[i]);
    }
    if (state) {
        active_tags.push_back(tag);
        active_tag_buttons.push_back(new QPushButton(tag.get_name()));
        active_tag_buttons.last()->setFlat(true);
        QPalette pal = active_tag_buttons.last()->palette();
        pal.setColor(QPalette::Button, tag.get_color());
        active_tag_buttons.last()->setAutoFillBackground(true);
        active_tag_buttons.last()->setPalette(pal);
        //active_tag_buttons.last()->setStyleSheet("QPushButton {background-color: " + tag.get_color().name() + "; color: white;}");
        active_tag_buttons.last()->setStyleSheet("background-color: "+ tag.get_color().name() + ";"
                                                 "color : white;"
                                                 "border-style: outset;"
                                                 "border-width: 2px;"
                                                 "border-radius: 10px;"
                                                 "border-color: beige;"
                                                 "font: bold 11px;"
                                                 "min-width: 5em;"
                                                 "padding: 4px;");
    } else {
        active_tags.removeOne(tag);
        for (int i = 0; i < active_tag_buttons.size(); ++i) {
            if (active_tag_buttons[i]->text() == tag.get_name()) {
                delete active_tag_buttons[i];
                active_tag_buttons.removeAt(i);
                break;
            }
        }
    }
    for (int i = 0; i < active_tag_buttons.size(); ++i) {
        tag_list_grid->addWidget(active_tag_buttons[i],i/4,i%4,Qt::AlignLeft);
    }
}

void c_add_document::on_accepted() {
    if (ui->name_lineEdit->text().isEmpty()) {
        QMessageBox::warning(this,tr("Incomplet field"),tr("No name specified, please give a name to the document"));
    }
    else if (ui->path_lineEdit->text().isEmpty()) {
        QMessageBox::warning(this,tr("Incomplet field"),tr("No path specified, please give the path of the document"));
    }
    else {
        this->accept();
    }
}

void c_add_document::on_search_button() {
    QUrl path = QFileDialog::QFileDialog::getOpenFileUrl();
    if (!path.isEmpty()) {
        ui->path_lineEdit->setText(path.path().remove(0,1));
        ui->name_lineEdit->setText(path.fileName());
    }
}

void c_add_document::customMenuRequested(QPoint pos) {
    m_tmp_tag_index = ui->tableView->indexAt(pos);
    m_tmp_pos = pos;
    qDebug() << pos;
    QMenu *menu=new QMenu(this);
    menu->addAction(tr("Edit name"),this,SLOT(on_edit_name()));
    menu->addAction(tr("Edit Color"),this,SLOT(on_edit_color()));
    menu->addAction(tr("Delete"),this,SLOT(on_delete_tag()));
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}


void c_add_document::on_edit_name() {
    tag_model->setEditable(m_tmp_tag_index,true);
    ui->tableView->edit(m_tmp_tag_index);
}

void c_add_document::on_edit_color() {
    bool flag_exit = false;
    QColorDialog color_dialog;
    c_tag* m_tmp_tag = tag_model->get_tag(m_tmp_tag_index);
    if (color_dialog.exec()) {
        QColor tag_color = color_dialog.selectedColor();
        if (tag_color.lightness() > 190) {
            while (tag_color.lightness() > 190 && !flag_exit) {
                QMessageBox::warning(this,tr("Wrong color"),tr("the ligthness of the chosen color is to low to be easily seen"));
                flag_exit = color_dialog.exec();
                tag_color = color_dialog.selectedColor();
            }
            if (flag_exit != QDialog::Accepted) {
                return;
            }
        }
        m_tmp_tag->set_color(tag_color);
        m_bdd->update_tag_color(m_tmp_tag->get_name(),tag_color);
    }
    m_tmp_tag = nullptr;
}

void c_add_document::on_delete_tag() {
    qDebug() << m_bdd->remove_tag(tag_model->get_tag(m_tmp_tag_index)->get_name());
    tag_model->remove_row(m_tmp_tag_index);
}
