#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <c_dbmanager.h>
#include <c_document.h>
#include <c_document_model.h>
#include <c_tag.h>
#include <c_add_document.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_add_document();
    //void on_doubleCliked(QModelIndex index);

private:
    Ui::MainWindow *ui;
    c_dbManager database_manager;
    c_document_model* document_model;
};

#endif // MAINWINDOW_H
