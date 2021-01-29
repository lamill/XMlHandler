#include "editwindow.h"
EditWindow::EditWindow(QWidget* parent):QDialog(parent,Qt::WindowCloseButtonHint)
{
    setModal(true);
    setWindowTitle("Edit");

    nameLine = new QLineEdit();
    formatWnd = new ListWidget();
    encodingWnd = new ListWidget();

    compileBox=new QCheckBox();
    pluginBox=new QCheckBox();
    intBox=new QCheckBox();

    nameLbl=new QLabel("TextEditor:");
    compileLbl=new QLabel("Can compile");
    pluginLbl=new QLabel("Has plugins");
    intLbl=new QLabel("Has intellisense");
    formatLbl=new QLabel("Formats");
    encodingLbl=new QLabel("encoding");



    okBtn=new QPushButton("Save");
    noBtn=new QPushButton("Cancel");

    errorLbl=new QLabel;

    mainLayOut=new QGridLayout;

    mainLayOut->addWidget(nameLbl,0,0);
    mainLayOut->addWidget(nameLine,0,1);

    mainLayOut->addWidget(formatLbl,1,0);
    mainLayOut->addWidget(formatWnd,1,1);

    mainLayOut->addWidget(encodingLbl,2,0);
    mainLayOut->addWidget(encodingWnd,2,1);

    mainLayOut->addWidget(compileLbl,3,0);
    mainLayOut->addWidget(compileBox,3,1);

    mainLayOut->addWidget(pluginLbl,4,0);
    mainLayOut->addWidget(pluginBox,4,1);

    mainLayOut->addWidget(intLbl,5,0);
    mainLayOut->addWidget(intBox,5,1);

    mainLayOut->addWidget(errorLbl,6,0);

    mainLayOut->addWidget(okBtn,7,0);
    mainLayOut->addWidget(noBtn,7,1);

    this->setLayout(mainLayOut);

    connect(okBtn,&QPushButton::clicked,this,&EditWindow::accept);
    connect(noBtn,&QPushButton::clicked,this,&EditWindow::reject);
    connect(this,&EditWindow::accepted,this,&EditWindow::endEditing);
}

EditWindow::~EditWindow()
{
    delete nameLine;


    delete compileBox;
    delete pluginBox;
    delete intBox;

    delete nameLbl;
    delete formatLbl;
    delete encodingLbl;
    delete compileLbl;
    delete pluginLbl;
    delete intLbl;

    delete okBtn;
    delete noBtn;

    delete mainLayOut;

    delete errorLbl;

    delete formatWnd;
    delete encodingWnd;
}

void EditWindow::edit(const TextEditor &editor, int row)
{
    nameLine->setText(editor.getName());
    compileBox->setChecked(editor.canCompile());
    pluginBox->setChecked(editor.hasplugins());
    intBox->setChecked(editor.hasIntellisense());
    formatWnd->SetList(editor.getFormats());
    encodingWnd->SetList(editor.getEncoding());
    currentRow = row;
    this->open();
}

void EditWindow::endEditing()
{
    TextEditor editor;
    editor.setName(nameLine->text());
    editor.setCompile(compileBox->isChecked());
    editor.setPlugins(pluginBox->isChecked());
    editor.setIntellisense(intBox->isChecked());
    editor.setFormats(formatWnd->getList());
    editor.setEncoding(encodingWnd->getList());

    emit editFinished(editor,currentRow);
}
