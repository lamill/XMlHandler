#include "texteditor.h"
#include <QDebug>
TextEditor::TextEditor()
{
    name = "default";
    intellisense = false;
    plugins = false;
    compile = false;
}

TextEditor::~TextEditor()
{

}

TextEditor::TextEditor( QString _name,
                        QStringList _formats,
                        QStringList _encoding,
                        bool _hasintellisense,
                        bool _hasplugins,
                        bool _cancompile)
{
    name = _name;
    formats = _formats;
    encoding = _encoding;
    intellisense = _hasintellisense;
    plugins = _hasplugins;
    compile = _cancompile;
}

TextEditor::TextEditor(const TextEditor &second)
{
    name=second.name;
    formats=second.formats;
    plugins=second.plugins;
    compile=second.compile;
    intellisense=second.intellisense;
    encoding=second.encoding;
}

TextEditor &TextEditor::operator=(const TextEditor &second)
{
    name=second.name;
    formats=second.formats;
    plugins=second.plugins;
    compile=second.compile;
    intellisense=second.intellisense;
    encoding=second.encoding;
    return *this;
}

const QString &TextEditor::getName() const
{
    return  name;
}

bool TextEditor::hasIntellisense() const
{
    return  intellisense;
}

bool TextEditor::hasplugins() const
{
    return  plugins;
}

bool TextEditor::canCompile() const
{
    return compile;
}

const QStringList &TextEditor::getFormats() const
{
    return  formats;
}

QString TextEditor::getFormatsByString() const
{
    auto str = formats.join("; ");
    if(formats.length()>1)
        str+=";";
    return str;
}

QString TextEditor::getEncodingByString() const
{
    auto str = encoding.join("; ");
    if(encoding.length()>1)
        str+=";";
    return str;
}

const QStringList &TextEditor::getEncoding() const
{
    return  encoding;
}

void TextEditor::setName(QString  _name)
{
    name = _name;
}


void TextEditor::setIntellisense(bool state)
{
    intellisense = state;
}

void TextEditor::setCompile(bool state)
{
    compile = state;
}

void TextEditor::setPlugins(bool state)
{
    plugins = state;
}

void TextEditor::setFormats(QStringList &&lst)
{
    formats = lst;
}

void TextEditor::setFormats(const QStringList &lst)
{
    formats = lst;
}

void TextEditor::setEncoding(const QStringList &lst)
{
    encoding = lst;
}

void TextEditor::setEncoding(QStringList &&lst)
{
    encoding=lst;
}

void TextEditor::print() const
{
    qDebug()<<name;
    qDebug()<<formats.length();
    qDebug()<<encoding.length();
    qDebug()<<plugins;
    qDebug()<<intellisense;
    qDebug()<<compile;
}
