#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Setting=new QSettings("EachOne","FileSplit",this);
    ui->lineEdit_SaveFolderSelect->setText(Setting->value("SaveFolder").toString());
    ui->lineEdit_FileNumber->setValidator(new QIntValidator(1,999999,this));
    ui->lineEdit_SplitNumber->setValidator(new QIntValidator(1,999999,this));
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
    ui->lineEdit_SplitFileSelect->setText(QFileDialog::getOpenFileName(this,tr("Split File Select"),"",tr("All Files (*.*);;")));
}

void MainWindow::on_pushButton_SaveFolderSelect_clicked()
{
    ui->lineEdit_SaveFolderSelect->setText(QFileDialog::getExistingDirectory(this,tr("Choose Directory"),Setting->value("SaveFolder").toString()));
    Setting->setValue("SaveFolder",ui->lineEdit_SaveFolderSelect->text());
}

void MainWindow::FileSplitSave()
{
    QFile File(ui->lineEdit_SplitFileSelect->text());
    bool FileEnd=false;
    if(!File.open(QFile::ReadOnly|QFile::Text) || !File.exists())
    {
        QMessageBox::warning(this,tr("Warning"),tr("File is not Found or File Read Error"),QMessageBox::Ok);
        return;
    }

    QProgressDialog *ProgressDlg=new QProgressDialog(tr("Waiting..."),NULL,0,100,this);
    ProgressDlg->setWindowModality(Qt::WindowModal);
    ProgressDlg->setWindowTitle(tr("FileSplit"));
    ProgressDlg->show();

    QTextStream FileStream(&File),SaveFileStream;
    int ContextCount=0,FileCount=1;
    QFile SaveFile;
    QString TmpFileName=FileStream.readAll();
    int Sum=TmpFileName.count("\n"),PreSum=0,Count=0;
    QString *ListContext=new QString[ui->lineEdit_FileNumber->text().toInt()];
    QString FileContext,TempStr;
    FileStream.seek(0);

    while (!FileStream.atEnd())
    {
        if(ContextCount==ui->lineEdit_SplitNumber->text().toInt())
        {
            ListContext[FileCount-1].append(FileContext);
            ContextCount=0;
            FileContext.clear();

            FileCount++;
        }

        if(FileCount==ui->lineEdit_FileNumber->text().toInt()+1)
        {
            FileCount=1;
        }
        TempStr=FileStream.readLine()+"\n";
        FileContext.append(TempStr);
        ContextCount++;

        if(Count>0 && PreSum!=(float)Count/Sum*100)
        {
            PreSum=(float)Count/Sum*100;
            ProgressDlg->setValue(PreSum);
        }
        Count++;
    }
    FileEnd=true;

    for(int i=0; i<ui->lineEdit_FileNumber->text().toInt(); i++)
    {
        if(i+1<10)
        {
            TmpFileName="000";
        }
        else if(i+1>=10 && i+1<100)
        {
            TmpFileName="00";
        }
        else if(i+1>=100 && i+1<1000)
        {
            TmpFileName="0";
        }
        else
        {
            TmpFileName="";
        }
        QFile OpenFile(QString("%1/%2%3.use").arg(ui->lineEdit_SaveFolderSelect->text()).arg(TmpFileName).arg(i+1));
        OpenFile.open(QFile::ReadWrite|QFile::Text);
        SaveFileStream.setDevice(&OpenFile);
        SaveFileStream<<ListContext[i];
        OpenFile.close();
    }

    if(!FileContext.isEmpty() && FileEnd && ContextCount==ui->lineEdit_SplitNumber->text().toInt())
    {
        if(FileCount<10)
        {
            TmpFileName="000";
        }
        else if(FileCount>=10 && FileCount<100)
        {
            TmpFileName="00";
        }
        else if(FileCount>=100 && FileCount<1000)
        {
            TmpFileName="0";
        }
        else
        {
            TmpFileName="";
        }
        QFile OpenFile(QString("%1/%2%3.use").arg(ui->lineEdit_SaveFolderSelect->text()).arg(TmpFileName).arg(FileCount));
        OpenFile.open(QFile::ReadWrite|QFile::Append|QFile::Text);
        SaveFileStream.setDevice(&OpenFile);
        SaveFileStream<<FileContext;
        OpenFile.close();
    }

    else if(!FileContext.isEmpty())
    {
        SaveFile.setFileName(QString("%1/Remain.use").arg(ui->lineEdit_SaveFolderSelect->text()));
        SaveFile.open(QFile::ReadWrite|QFile::Text);
        SaveFileStream.setDevice(&SaveFile);
        SaveFileStream<<FileContext;
        SaveFile.close();
    }

    delete [] ListContext;
    File.close();
    ProgressDlg->deleteLater();
    QMessageBox::information(this,tr("Information"),tr("Files Created."),QMessageBox::Ok);
}
