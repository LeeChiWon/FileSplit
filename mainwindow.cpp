#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Setting=new QSettings("EachOne","FileSplit",this);
    ui->lineEdit_SaveFolderSelect->setText(Setting->value("SaveFolder").toString());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Change_clicked()
{
    if(ui->lineEdit_SaveFolderSelect->text().isEmpty() || ui->lineEdit_SplitFileSelect->text().isEmpty() || ui->lineEdit_SplitNumber->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Warning"),tr("LineEdit is empty."),QMessageBox::Ok);
        return;
    }
    FileSplitSave();
}

void MainWindow::on_pushButton_SplitFileSelect_clicked()
{
    ui->lineEdit_SplitFileSelect->setText(QFileDialog::getOpenFileName(this,tr("Split File Select"),"",tr("All Files (*.*);")));
}

void MainWindow::on_pushButton_SaveFolderSelect_clicked()
{
    ui->lineEdit_SaveFolderSelect->setText(QFileDialog::getExistingDirectory(this,tr("Choose Directory"),Setting->value("SaveFolder").toString()));
    Setting->setValue("SaveFolder",ui->lineEdit_SaveFolderSelect->text());
}

void MainWindow::FileSplitSave()
{
    QFile File(ui->lineEdit_SplitFileSelect->text());

    if(!File.open(QFile::ReadOnly|QFile::Text) || !File.exists())
    {
        QMessageBox::warning(this,tr("Warning"),tr("File is not Found or File Read Error"),QMessageBox::Ok);
        return;
    }

    QProgressDialog *ProgressDlg=new QProgressDialog(tr("Waiting..."),NULL,0,100,this);
    ProgressDlg->setWindowModality(Qt::WindowModal);
    ProgressDlg->setWindowTitle(tr("FileSplit"));
    ProgressDlg->show();

    QTextStream FileStream(&File);
    int ContextCount=1,FileCount=1;
    QFile SaveFile;
    QTextStream SaveFileStream;
    QString Test=FileStream.readAll();
    int Sum=(float)Test.count("\n")/ui->lineEdit_SplitNumber->text().toInt(),PreSum=0;
    FileStream.seek(0);
    while (!FileStream.atEnd())
    {
        if(ContextCount==1)
        {
            if(FileCount<10)
            {
                SaveFile.setFileName("000");
            }
            else if(FileCount>=10 && FileCount<100)
            {
                SaveFile.setFileName("00");
            }
            else if(FileCount>=100 && FileCount<1000)
            {
                SaveFile.setFileName("0");
            }
            else if(FileCount>=1000 &&FileCount<10000)
            {
                SaveFile.setFileName("");
            }
            SaveFile.setFileName(QString("%1/%2%3.use").arg(ui->lineEdit_SaveFolderSelect->text()).arg(SaveFile.fileName()).arg(FileCount));
            SaveFile.open(QFile::ReadWrite|QFile::Text);
            SaveFileStream.setDevice(&SaveFile);
            FileCount++;
        }
        SaveFileStream<<FileStream.readLine()<<"\n";

        if(ContextCount==ui->lineEdit_SplitNumber->text().toInt())
        {
            //qDebug()<<(float)FileCount/Sum*100;
            if(FileCount>0 && PreSum!=(float)FileCount/Sum*100)
            {
                PreSum=(float)FileCount/Sum*100;
                ProgressDlg->setValue(PreSum);
            }
            SaveFile.close();
            ContextCount=0;
        }
        ContextCount++;
    }

    File.close();
    ProgressDlg->deleteLater();
    QMessageBox::information(this,tr("Information"),tr("Files Created."),QMessageBox::Ok);
}
