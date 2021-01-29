#include "mainwidget.h"
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QHeaderView>
MainWidget::MainWidget(CustomTableModel *_model,CustomController * _controller,QWidget *parent)
    : QWidget(parent),model(_model),controller(_controller)
{
    this->resize(800,600);
    table = new QTableView();
    table->setModel(model);
    table->horizontalHeader()->setStretchLastSection(true);

    importWnd = new ImportWindow();

    clearBtn = new QPushButton("Clear");
    importBtn = new QPushButton("Import");
    importBtn->setDisabled(true);
    xsdBtn = new QPushButton("Set XSD");
    menu = new QMenu();
    delAction = new QAction("Delete");
    editAction = new QAction("Edit");
    exportAction = new QAction("Export");
    editWnd = new EditWindow();

    menu->addAction(delAction);
    menu->addAction(editAction);
    menu->addAction(exportAction);

    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(clearBtn);
    btnLayout->addWidget(importBtn);
    btnLayout->addWidget(xsdBtn);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(table);
    this->setLayout(mainLayout);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setContextMenuPolicy(Qt::CustomContextMenu);



    connect(clearBtn,SIGNAL(clicked()),controller,SLOT(clearTable()));
    connect(importBtn,SIGNAL(clicked()),importWnd,SLOT(show()));
    connect(xsdBtn,SIGNAL(clicked()),this,SLOT(getXsdPath()));
    connect(controller,SIGNAL(schemaSet(bool)),this,SLOT(xsdChange(bool)));
    connect(importWnd,&ImportWindow::importRequested,controller,&CustomController::importFolder);
    connect(controller,&CustomController::fileParsed,importWnd,&ImportWindow::addRecord);
    connect(table,&QTableView::customContextMenuRequested,this,&MainWidget::getContextMenu);
    connect(editAction,&QAction::triggered,this,&MainWidget::startEdit);
    connect(delAction,&QAction::triggered,this,&MainWidget::delRecord);
    connect(exportAction,&QAction::triggered,this,&MainWidget::exportRecord);
    connect(editWnd,&EditWindow::editFinished,controller,&CustomController::editHandler);
    connect(controller,&CustomController::importFinished,importWnd,&ImportWindow::endImport);
}

MainWidget::~MainWidget()
{
    delete table;
    delete mainLayout;
    delete clearBtn;
    delete importBtn;
    delete  importWnd;
    delete menu;
    delete editWnd;
}



void MainWidget::getXsdPath() const
{
    QUrl path = QFileDialog::getOpenFileUrl();
    if(!path.isEmpty())
        controller->setXSD(path);
}

void MainWidget::xsdChange(bool state)
{
    if(state){
        importBtn->setDisabled(false);
    }
    else
        importBtn->setDisabled(true);
}

void MainWidget::startEdit()
{
    int row = table->selectionModel()->selectedRows()[0].row();
    editWnd->edit(model->getDataAt(row),row);
}

void MainWidget::delRecord()
{
    int row = table->selectionModel()->selectedRows()[0].row();
    controller->deleteHandler(row);
}

void MainWidget::exportRecord()
{
    int row = table->selectionModel()->selectedRows()[0].row();
    QString path = QFileDialog::getSaveFileName(this, "Save File",
                                                QDir::currentPath(),
                                                tr("XML files (*.xml)"));
    controller->exportRecord(row,path);
}

void MainWidget::getContextMenu(const QPoint &pos)
{
    menu->exec(QWidget::mapToGlobal(pos),delAction);
}

