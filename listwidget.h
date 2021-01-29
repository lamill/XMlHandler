#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>
#include <QStringListModel>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

/**
 * @brief help window for edit entry window
 * 
 * allow to display and manipulate QStringList\n
 * used in @ref EditWindow to display @ref TextEditor#formats and @ref TextEditor#encoding
 */
class ListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = nullptr);
    virtual ~ListWidget();
    QStringList getList();
    void SetList(const QStringList&);
private:
    QPushButton* addBtn;
    QPushButton* delBtn;
    QLineEdit* addLine;
    QListView* view;
    QStringListModel* model;
    QVBoxLayout* mainLayout;
    QHBoxLayout* ctrlLayout;
private slots:
    void addData();
    void delData();

signals:

};

#endif // LISTWIDGET_H
