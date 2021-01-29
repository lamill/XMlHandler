#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H
#include <QString>
#include <QStringList>

/**
 * @brief Base class for data store
 * 
 */
class TextEditor
{
public:
    TextEditor();
    ~TextEditor();
    TextEditor( QString _name,
                QStringList _formats,
                QStringList _encoding,
                bool _hasintellisense,
                bool _hasplugins,
                bool _cancompile);
    TextEditor(const TextEditor& other);
    TextEditor& operator=(const TextEditor& second);
    const QString& getName() const;
    bool hasIntellisense() const;
    bool hasplugins() const;
    bool canCompile() const;
    const QStringList& getFormats() const;
    QString getFormatsByString() const;
    QString getEncodingByString() const;
    const QStringList& getEncoding() const;
    void setName(QString);
    void setIntellisense(bool);
    void setCompile(bool);
    void setPlugins(bool);
    void setFormats(const QStringList&);
    void setFormats( QStringList&&);
    void setEncoding(const QStringList&);
    void setEncoding( QStringList&&);
    static const int propCount = 6;

    void print() const;
private:
    QString name;
    QStringList formats;
    QStringList encoding;
    bool intellisense;
    bool plugins;
    bool compile;

};

#endif // TEXTEDITOR_H
