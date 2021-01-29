#include "mainwidget.h"
#include <QApplication>
#include "customtablemodel.h"
#include "customcontroller.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CustomTableModel model;
    if(!model.initDataBase())
    {
        QMessageBox::critical(
                    nullptr,
                    "Unable to load database",
                    "This demo needs the SQLITE driver"
                    );
        return 1;
    }
    CustomController controller(&model);
    MainWidget w(&model,&controller);
    w.show();
    return a.exec();
}
