#ifndef DIALOGCOMMANDSQL_H
#define DIALOGCOMMANDSQL_H

#include <QDialog>

namespace Ui {
class DialogCommandSQL;
}

class DialogCommandSQL : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCommandSQL(QWidget *parent = nullptr);
    ~DialogCommandSQL();

private:
    Ui::DialogCommandSQL *ui;
};

#endif // DIALOGCOMMANDSQL_H
