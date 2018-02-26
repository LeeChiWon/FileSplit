#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Change_clicked();
    void on_pushButton_SplitFileSelect_clicked();
    void on_pushButton_SaveFolderSelect_clicked();

private:
    Ui::MainWindow *ui;
    QSettings *Setting;

    void FileSplitSave();

};

#endif // MAINWINDOW_H
