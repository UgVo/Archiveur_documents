#include "dialogcommandsql.h"
#include "ui_dialogcommandsql.h"

DialogCommandSQL::DialogCommandSQL(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCommandSQL)
{
    ui->setupUi(this);
}

DialogCommandSQL::~DialogCommandSQL()
{
    delete ui;
}
