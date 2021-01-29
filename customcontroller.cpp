#include "customcontroller.h"
#include <QUrl>
#include <QFile>
#include "texteditor.h"
#include <QXmlStreamReader>
#include <QString>
#include <QtConcurrent/QtConcurrentRun>
CustomController::CustomController(CustomTableModel* _model,QObject *parent) :QObject(parent),model(_model)
{
    schema = new QXmlSchema();
    validator= new QXmlSchemaValidator();
    watcher = new QFutureWatcher<QVector<TextEditor>>();
    msgHandler = new MsgHandler(this);
    validator->setMessageHandler(msgHandler);

    connect(watcher,SIGNAL(finished()),this,SLOT(handleFuture()));

}

CustomController::~CustomController()
{
    delete schema;
    delete validator;
    delete msgHandler;
}

TextEditor CustomController::parseXML(QFile& file)
{
    QXmlStreamReader reader(&file);
    TextEditor editor;
    while(!reader.atEnd()&&!reader.hasError())
    {
        auto token = reader.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;
        if(token == QXmlStreamReader::StartElement)
        {
            if(reader.name()=="root")
                continue;
            if(reader.name()=="texteditor")
                editor.setName(reader.readElementText());
            else if(reader.name()=="fileformats")
                editor.setFormats(parseEnumStr(reader.readElementText()));
            else if(reader.name()=="encoding")
                editor.setEncoding(parseEnumStr(reader.readElementText()));
            if(reader.name()=="hasintellisense")
            {
                auto val = reader.readElementText();
                if(val=="true")
                    editor.setIntellisense(true);
                else
                    editor.setIntellisense(false);
            }
            if(reader.name()=="hasplugins")
            {
                auto val = reader.readElementText();
                if(val=="true")
                    editor.setPlugins(true);
                else
                    editor.setPlugins(false);
            }
            if(reader.name()=="cancompile")
            {
                auto val = reader.readElementText();
                if(val=="true")
                    editor.setCompile(true);
                else
                    editor.setCompile(false);
            }
        }
    }
    reader.clear();
    return editor;
}

QStringList CustomController::parseEnumStr(QString str) const
{
    str = str.replace(" ","");
    return  str.split(";", QString::SkipEmptyParts);
}

QVector<TextEditor> CustomController::runImportFolder(QStringList lst)
{
    QVector<TextEditor> editorsList;
    if(schema->isValid())
    {

        for(auto& filename: lst)
        {
            QFile file(filename);
            if(!file.open(QIODevice::ReadOnly))
            {
                QString msg = msgHandler->formMsg(false,filename,"Can't open file.");
                emit fileParsed(false,msg);
            }
            if(validator->validate(QUrl::fromLocalFile(filename)))
            {
                editorsList.append(parseXML(file));
                QString msg = msgHandler->formMsg(true,filename);
                emit fileParsed(true,msg);
            }
            file.close();
        }

    }
    return  editorsList;
}

void CustomController::importFolder(QStringList lst)
{
    auto future = QtConcurrent::run(this,&CustomController::runImportFolder,lst);
    watcher->setFuture(future);
}

void CustomController::setXSD(QUrl path)
{
    schema->load(path);
    if(schema->isValid())
    {
        validator->setSchema(*schema);
        emit schemaSet(true);
    }
    else
    {
        emit schemaSet(false);
    }
}

void CustomController::clearTable()
{
    model->clearTable();
}

void CustomController::editHandler(TextEditor editor, int row)
{
    model->replaceAt(row,editor);
}

void CustomController::deleteHandler(int row)
{
    model->deleteAt(row);
}

void CustomController::exportRecord(int row, QString Path)
{
    auto editor = model->getDataAt(row);
    QFile file(Path);
    if(file.open(QIODevice::WriteOnly))
    {
        auto compileStr = editor.canCompile()?"true":"false";
        auto intStr = editor.hasIntellisense()?"true":"false";
        auto pluginsStr = editor.hasplugins()?"true":"false";
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("root");
        writer.writeTextElement("texteditor", editor.getName());
        writer.writeTextElement("fileformats", editor.getFormatsByString());
        writer.writeTextElement("encoding", editor.getEncodingByString());
        writer.writeTextElement("hasintellisense", intStr);
        writer.writeTextElement("hasplugins", pluginsStr);
        writer.writeTextElement("cancompile", compileStr);
        writer.writeEndElement();
        file.close();
    }


}

void CustomController::handleFuture()
{
    model->appendData(watcher->result());
    emit importFinished();
}

MsgHandler::MsgHandler(CustomController *ctrl, QObject *parent):QAbstractMessageHandler(parent),controller(ctrl)
{

}

QString MsgHandler::formMsg(bool state,QString filename, QString desc, int line)
{
    QString resMsg;
    if(state)
        resMsg= "<b>Successfuly parsed file:</b><br>";
    else
        resMsg= "<b>Error while parsing following file:</b><br>";
    resMsg+=filename+"<br>";
    resMsg+=desc;
    if(line>=0)
    {
        resMsg+=".At line ";
        resMsg+=QString::number(line);
        resMsg+="<br>";
    }
    return resMsg;
}

void MsgHandler::handleMessage(QtMsgType type, const QString &description, const QUrl &identifier, const QSourceLocation &sourceLocation)
{
    document.setHtml(description);
    QString resMsg = formMsg(false,sourceLocation.uri().toLocalFile(),document.toPlainText(),sourceLocation.line());
    emit controller->fileParsed(false,resMsg);
}
