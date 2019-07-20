#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // =========== Mise en place de l'interface ===========
    ui->setupUi(this);

    // ========= Création BDD  =========
    database_manager.open(QString("../../bdd/database.db"));

    // =========== Gestion Documents ===========
    document_model = new c_document_model(&database_manager,this);
    ui->tableView_documents->setModel(document_model);

    ui->tableView_documents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_documents->resizeColumnsToContents();
    ui->tableView_documents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_documents->setShowGrid(true);
    ui->tableView_documents->sortByColumn(0,Qt::AscendingOrder);

    // =========== Connexions ===========
    QObject::connect(ui->action_add_document,SIGNAL(triggered()),this,SLOT(on_add_document()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_document() {
    c_add_document gui(&database_manager);
    int res = gui.exec();
    qDebug() << gui.get_checked_tags().size();
}
