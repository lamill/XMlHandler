#include "customtablemodel.h"
#include <QtConcurrentRun>
#include <QMutexLocker>
CustomTableModel::CustomTableModel(QObject *parent): QAbstractTableModel(parent)
{

    qRegisterMetaType<QVector<int> >("QVector<int>");

}

CustomTableModel::~CustomTableModel()
{

}

int CustomTableModel::rowCount(const QModelIndex &parent) const
{
    return  _data.length();
}

int CustomTableModel::columnCount(const QModelIndex &parent) const
{
    return TextEditor::propCount;
}

QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return getFieldByIndex(index);
    }
    return QVariant();
}

QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole)
        return  QVariant();
    if(orientation==Qt::Horizontal)
    {
        switch (section) {
            case 0:
                return QString("name");
            case 1:
                return QString("hasIntellisense");
            case 2:
                return QString("hasplugins");
            case 3:
                return QString("canCompile");
            case 4:
                return QString("getFormatsByString");
            case 5:
                return QString("getEncodingByString ");
        }
    }
    return  QVariant();
}

const TextEditor &CustomTableModel::getDataAt(int index) const
{
    return  _data.at(index).second;
}

void CustomTableModel::replaceAt(int i , TextEditor editor)
{
    auto lambda = [this,i,editor]()
    {
        QMutexLocker locker(&mutex);
        QString prepare("UPDATE editor SET "
                        "name = '%1',"
                        "formats = '%2',"
                        "encoding = '%3',"
                        "intellisense = %4,"
                        "cancompile = %5,"
                        "plugins = %6 "
                        "WHERE rowid = %7;");


        auto queryStr = prepare.arg(editor.getName())
                                .arg(editor.getFormatsByString())
                                .arg(editor.getEncodingByString())
                                .arg(editor.hasIntellisense())
                                .arg(editor.canCompile())
                                .arg(editor.hasplugins())
                                .arg(_data.at(i).first);

        db.exec(queryStr);
        _data[i].second=editor;
        emit dataChanged(index(i,0),index(i,TextEditor::propCount));
    };

    QtConcurrent::run(lambda);

}

void CustomTableModel::deleteAt(int row)
{
    auto lambda =[this,row]()
    {
        int rowid = _data.at(row).first;
        QString tempelate = "delete from editor where rowid=%1";

        auto query = tempelate.arg(rowid);
        db.exec(query);
        beginRemoveRows(QModelIndex(),row,row);
        _data.remove(row);
        endRemoveRows();
    };

    QtConcurrent::run(lambda);
}

bool CustomTableModel::initDataBase()
{
    QDir dir(QCoreApplication::applicationDirPath());
    QString name = dir.filePath("db.sqlite");
    bool isExisting = dir.exists(name);
    if(!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        return false;
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    if(!db.open())
    {
        return false;
    }
    if(!isExisting)
    {
        db.exec("CREATE TABLE editor ("
                "name TEXT, "
                "formats TEXT, "
                "encoding TEXT, "
                "intellisense INTEGER, "
                "plugins INTEGER, "
                "cancompile INTEGER)"
                ";");
    }
    else
    {
        fillFromDB();
    }
    return true;
}

void CustomTableModel::fillFromDB()
{
    auto lambda = [this]()
    {
        QMutexLocker locker(&mutex);
        auto query = db.exec("select rowid,name,formats,encoding,intellisense,plugins,cancompile from editor;");
        QVector<std::pair<int,TextEditor>> tempVector;
            auto rec = query.record();
            while (query.next()) {
                TextEditor editor;
                editor.setName(query.value(rec.indexOf("name")).toString());
                editor.setCompile(query.value(rec.indexOf("cancompile")).toBool());
                editor.setPlugins(query.value(rec.indexOf("plugins")).toBool());
                editor.setIntellisense(query.value(rec.indexOf("intellisense")).toBool());

                QString formatStr = query.value(rec.indexOf("formats")).toString();
                formatStr.replace(" ","");
                auto formats = formatStr.split(";",QString::SkipEmptyParts);

                QString encStr = query.value(rec.indexOf("encoding")).toString();
                encStr.replace(" ","");
                auto enc = encStr.split(";",QString::SkipEmptyParts);

                editor.setFormats(formats);
                editor.setEncoding(enc);

                int id = query.value(rec.indexOf("rowid")).toInt();
                tempVector.append({id,editor});
            }
            if(_data.size()>0)
            {
                beginRemoveRows(QModelIndex(),0,_data.length());
                _data.clear();
                endRemoveRows();
            }

            if(tempVector.size()>0)
            {
                beginInsertRows(QModelIndex(),0,tempVector.size());
                _data=tempVector;
                endInsertRows();
            }
    };

    QtConcurrent::run(lambda);


}



void CustomTableModel::appendData(const QVector<TextEditor> &another)
{
    auto lambda = [this](const QVector<TextEditor> another)
    {
        QMutexLocker locker(&mutex);
        if(another.length()>0)
        {
            QString tempelate = "INSERT INTO editor (name,formats,encoding,intellisense,plugins,cancompile) "
                                "VALUES( '%1','%2','%3',%4,%5,%6);";


            QVector<std::pair<int,TextEditor>> readyVector;
            for(auto& elem:another){
                auto queryStr = tempelate.arg(elem.getName())
                        .arg(elem.getFormatsByString())
                        .arg(elem.getEncodingByString())
                        .arg(elem.hasIntellisense())
                        .arg(elem.hasplugins())
                        .arg(elem.canCompile());

                db.exec(queryStr);
                auto query = db.exec("select last_insert_rowid();");
                query.first();
                readyVector.append({query.value(0).toInt(),elem});
            }

            int endIndex = _data.length()+another.length()-1;
            beginInsertRows(QModelIndex(),_data.length(),endIndex);
            _data.append(readyVector);
            endInsertRows();
        }

    };

    QtConcurrent::run(lambda,another);

}

void CustomTableModel::clearTable()
{
    auto lambda = [this]()
    {
        QMutexLocker locker (&mutex);
        db.exec("delete from editor");
        beginRemoveRows(QModelIndex(),0,_data.length()-1);
        _data.clear();
        endRemoveRows();
    };
    QtConcurrent::run(lambda);
}



QVariant CustomTableModel::getFieldByIndex(const QModelIndex& index) const
{
    int row = index.row();
    QVariant retVal;
    switch (index.column()) {
        case 0:
            retVal = _data.at(row).second.getName();
            break;
        case 1:
            retVal = _data.at(row).second.hasIntellisense();
            break;
        case 2:
            retVal = _data.at(row).second.hasplugins();
            break;
        case 3:
            retVal = _data.at(row).second.canCompile();
            break;
        case 4:
        {
            retVal = _data.at(row).second.getFormatsByString();
            break;
        }
        case 5:
        {
            retVal = _data.at(row).second.getEncodingByString();
            break;
        }
    }
    return retVal;
}


