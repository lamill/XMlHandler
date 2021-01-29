#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H
#include <QAbstractTableModel>
#include "texteditor.h"
#include "QVector"
#include <QtSql>
#include <utility>
#include <QMutex>

/**
 * @brief QAbstractTableModel implemention
 * @warning formats and encoding stored as string in DB only to simplify the task
 * @todo implement correct way to store formats and encoding
 * 
 * Holds and manipulates all the data\n
 * DataBase connection hardcoded in CustomTableModel#initDataBase\n
 * QtConcurrent::run is used for sql operations\n
 * QVector<std::pair<int,TextEditor>> is used to store the data in order to associate rows in DB and vector
 */
class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    CustomTableModel(QObject *parent = nullptr);
    ~CustomTableModel() override;
    int rowCount(const QModelIndex& parent = QModelIndex())const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    const TextEditor& getDataAt(int index) const;
    bool initDataBase();


public slots:
    /**
     * @brief Edit entry function
     * 
     * there is no editing entry in fact, only replacing
     */
    void replaceAt(int,TextEditor);
    ///delete single entry
    void deleteAt(int);
    /// used to insesrt rows
    void appendData(const QVector<TextEditor>&);
    void clearTable();
private:
    QVector<std::pair<int,TextEditor>> _data;
    QSqlDatabase db;
    QMutex mutex;
    void fillFromDB();
    QVariant getFieldByIndex(const QModelIndex& index) const;



};

#endif // CUSTOMTABLEMODEL_H
