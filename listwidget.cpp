#include "listwidget.h"

ListWidget::ListWidget(QWidget *parent) : QWidget(parent)
{
   addLine = new QLineEdit();
   addBtn = new QPushButton("add");
   delBtn = new QPushButton("delete");
   model = new QStringListModel();
   view = new QListView();
   mainLayout = new QVBoxLayout();
   ctrlLayout = new QHBoxLayout();

   view->setModel(model);
   view->setSelectionMode(QAbstractItemView::SingleSelection);

   ctrlLayout->addWidget(addLine);
   ctrlLayout->addWidget(addBtn);
   ctrlLayout->addWidget(delBtn);

   mainLayout->addWidget(view);
   mainLayout->addLayout(ctrlLayout);

   this->setLayout(mainLayout);

   connect(addBtn,&QPushButton::clicked,this,&ListWidget::addData);
   connect(delBtn,&QPushButton::clicked,this,&ListWidget::delData);

}

ListWidget::~ListWidget()
{
    delete addBtn;
    delete delBtn;
    delete addLine;
    delete view;
    delete model;
    delete mainLayout;
}

QStringList ListWidget::getList()
{
    return  model->stringList();
}

void ListWidget::SetList(const QStringList &lst)
{
    model->setStringList(lst);
}

void ListWidget::addData()
{
    if(addLine->text().length()!=0)
    {
        auto lst = model->stringList();
        lst.append(addLine->text());
        model->setStringList(lst);
        addLine->clear();
    }
}

void ListWidget::delData()
{
    int row = view->currentIndex().row();
    model->removeRows(row,1);
}
