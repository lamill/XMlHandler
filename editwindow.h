#ifndef EDITWINDOW_H
#define EDITWINDOW_H
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGridLayout>
#include "texteditor.h"
#include "listwidget.h"

/**
 * @brief edit entry window
 * 
 * used in @ref MainWidget for editing entry
 */
class EditWindow : public QDialog
{
    Q_OBJECT
public:
    explicit EditWindow(QWidget* parent = nullptr);
    ~EditWindow();
    /**
     * @brief show editing window
     * 
     */
    void edit(const TextEditor&,int);
signals:
    /// emited when user ends editing
    void editFinished(TextEditor,int);
private:
    QLineEdit* nameLine;
    ListWidget* formatWnd;
    ListWidget* encodingWnd;

    QCheckBox* compileBox;
    QCheckBox* pluginBox;
    QCheckBox* intBox;

    QLabel* nameLbl;
    QLabel* formatLbl;
    QLabel* encodingLbl;
    QLabel* compileLbl;
    QLabel* pluginLbl;
    QLabel* intLbl;

    QPushButton* okBtn;
    QPushButton* noBtn;

    QGridLayout* mainLayOut;

    QLabel* errorLbl;

    int currentRow;

private slots:
    /**
     * @brief handle edit ending
     * 
     * forms @ref TextEditor and emits @ref EditWindow#editFinished
     */
    void endEditing();
};

#endif // EDITWINDOW_H
