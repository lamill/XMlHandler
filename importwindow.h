#ifndef IMPORTWINDOW_H
#define IMPORTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QProgressBar>

/**
 * @brief xml import window
 * 
 * shows all information to user about xml parsing 
 */
class ImportWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ImportWindow(QWidget *parent = nullptr);
    ~ImportWindow();
private:
    QVBoxLayout* mainLayout;
    QTextEdit* infoWindow;
    QPushButton* closeBtn;
    QLineEdit* pathLine;
    QLabel* pathLabel;
    QPushButton* getPathBtn;
    QHBoxLayout* topLayout;
    QHBoxLayout* footerLayout;
    QPushButton* importBtn;
    QLabel* progressLbl;
    QProgressBar *progressBar;
    int successParsed=0;
    int totalParsed=0;
    /// uses QFileDialog to get import folder
    void getFolder();
    /**
     * @brief initiates xml import 
     * 
     * prepares window for new import\n
     * emits @ref ImportWindow#importRequested 
     */
    void beginImport();
public slots:
    /// adds text to information window
    void addRecord(bool, QString);
    void endImport();

signals:
    void importRequested(QStringList);
};

#endif // IMPORTWINDOW_H
