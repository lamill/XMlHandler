#include "importwindow.h"
#include <QFileDialog>
ImportWindow::ImportWindow(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
    infoWindow = new QTextEdit();
    closeBtn= new QPushButton("close");
    pathLine = new QLineEdit();
    pathLabel = new QLabel("import Folder");
    getPathBtn = new QPushButton("Choose Folder");
    importBtn = new QPushButton("Import");
    topLayout = new QHBoxLayout();
    footerLayout = new QHBoxLayout();
    progressLbl = new QLabel();
    progressBar = new QProgressBar();

    progressBar->setMinimum(0);
    progressBar->setHidden(true);

    infoWindow->setReadOnly(true);
    pathLine->setReadOnly(true);
    importBtn->setDisabled(true);


    topLayout->addWidget(pathLabel);
    topLayout->addWidget(pathLine);
    topLayout->addWidget(getPathBtn);

    footerLayout->addWidget(importBtn);
    footerLayout->addWidget(closeBtn);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(infoWindow);
    mainLayout->addWidget(progressLbl);
    mainLayout->addWidget(progressBar);
    mainLayout->addLayout(footerLayout);

    this->setWindowFlag(Qt::WindowCloseButtonHint,false);
    this->setLayout(mainLayout);

    connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(getPathBtn,&QPushButton::clicked,this,&ImportWindow::getFolder);
    connect(importBtn,&QPushButton::clicked,this,&ImportWindow::beginImport);

}

ImportWindow::~ImportWindow()
{
    delete mainLayout;
    delete infoWindow;
    delete closeBtn;
    delete importBtn;
    delete getPathBtn;
    delete pathLabel;
    delete pathLine;
}

void ImportWindow::getFolder()
{
    QString path = QFileDialog::getExistingDirectory();
    if(!path.isEmpty())
    {
        importBtn->setDisabled(false);
        pathLine->setText(path);
    }

}

void ImportWindow::beginImport()
{
    QString path = pathLine->text();
    if(!path.isEmpty())
    {
        QDir dir(path);
        auto lst = dir.entryList({"*.xml"});

        totalParsed=lst.length();
        progressBar->setMaximum(totalParsed);
        progressLbl->setText("");
        importBtn->setDisabled(true);
        infoWindow->setText("Start import");
        infoWindow->append(QString("Files to import:%1").arg(totalParsed));
        successParsed=0;
        progressBar->setValue(0);
        progressBar->setHidden(false);


        for(auto& elem: lst)
        {
            elem=dir.filePath(elem);
        }
        emit importRequested(lst);
    }
}

void ImportWindow::addRecord(bool state,QString msg)
{
    progressBar->setValue(progressBar->value()+1);
    if(state)
        progressLbl->setText("Successfuly parsed:"+QString::number(++successParsed));
    infoWindow->append(msg);
}

void ImportWindow::endImport()
{
    infoWindow->append("Import finished");
    infoWindow->append(QString("Successfuly imported %1 files from %2").arg(successParsed).arg(totalParsed));
    importBtn->setDisabled(false);
}
