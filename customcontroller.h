#ifndef CUSTOMCONTROLLER_H
#define CUSTOMCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QXmlSchema>
#include "texteditor.h"
#include <QXmlSchemaValidator>
#include <QFile>
#include <QAbstractMessageHandler>
#include "customtablemodel.h"
#include <QTextDocument>
#include <QFutureWatcher>


class MsgHandler;

/**
 * @brief Controller for MVC 
 
    \warning This is example realization.Class methods dont make check for invalid values.

    Controller realization fo MVC.
    Handles user actions and prepares data for model.All data passed to model is valid.\n
    Uses QXmlSchemaValidator  for validating xml\n
    QtConcurrent::run is used for multithreading\n
    @ref CustomController#importFolder is a top lvl function used to import xml files


    
 */
class CustomController : public QObject
{
    Q_OBJECT
public:
    explicit CustomController(CustomTableModel*,QObject *parent = nullptr);
    ~CustomController();
private:
    CustomTableModel* model;
    QXmlSchema* schema;
    QXmlSchemaValidator* validator;
    MsgHandler* msgHandler;
    QFutureWatcher<QVector<TextEditor>>* watcher;

    TextEditor parseXML(QFile& file);
    QStringList parseEnumStr(QString) const;
    QVector<TextEditor> runImportFolder(QStringList lst);


public slots:
    /**
     * starts importing xml files in another thread
     * 
     * @param lst  file pathes list 
     */
    void importFolder(QStringList lst);
    /**
     * Sets xsd file to schema\n
     * emits @ref CustomController#schemaSet is successful
     * 
     * @param path path ro xsd file
     */
    void setXSD(QUrl path);
    void clearTable();
    void editHandler(TextEditor,int);
    void deleteHandler(int row);
    void exportRecord(int row, QString Path);
private slots:
    /**
     * Called when parsing xml files is done\n
     * Passes data to model.\n
     * emits @ref CustomController#importFinished
     * 
     */
    void handleFuture();
signals:
    /// emited when valid xsd file is set
    void schemaSet(bool);
    /// emited when sigle file is parsed
    void fileParsed(bool,QString);
    /// emited when all files are parsed
    void importFinished();
};

/**
 * @brief QAbstractMessageHandler implemention
 * 
 */
class MsgHandler: public QAbstractMessageHandler
{
    Q_OBJECT
    CustomController* controller;
    QTextDocument document;
public:
    explicit MsgHandler(CustomController*,QObject *parent = nullptr);
    QString formMsg(bool state,QString filename,QString desc="",int line=-1);
protected:
    void handleMessage(QtMsgType type,
                       const QString &description,
                       const QUrl &identifier = QUrl(),
                       const QSourceLocation &sourceLocation = QSourceLocation()) override;
};

#endif // CUSTOMCONTROLLER_H
