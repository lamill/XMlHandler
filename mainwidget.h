#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "customcontroller.h"
#include "importwindow.h"
#include <QMenu>
#include "editwindow.h"

/**
 * @brief main Gui window
 * @warning no items size handled
 * 
 * displays the table and all control buttons\n
 *  connects all signals from gui with @ref CustomController slots
 */
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(CustomTableModel *_model,CustomController * controller,QWidget *parent = nullptr);
    ~MainWidget();
private:
    CustomTableModel *model;
    CustomController * controller;
    ImportWindow* importWnd;
    EditWindow* editWnd;
    QTableView *table;
    QVBoxLayout *mainLayout;
    QHBoxLayout * btnLayout;
    QPushButton * clearBtn;
    QPushButton * importBtn;
    QPushButton * xsdBtn;
    QMenu* menu;
    QAction* delAction;
    QAction* editAction;
    QAction* exportAction;

 private slots:
    void getXsdPath() const;
    void xsdChange(bool);
    void startEdit();
    void delRecord();
    void exportRecord();
    void getContextMenu(const QPoint &pos);
};
#endif // MAINWIDGET_H
