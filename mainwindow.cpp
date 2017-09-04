#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QFileInfo>
#include <QFileDialog>
#include "fileutils.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setResultBoxTipText();
    setDefaultCalculationOptions();
    disableSomeControls();
    ui->progressBar->setValue(0);
    ui->progressBar_2->setValue(0);
    setAcceptDrops(true); // 允许拖拽
    connect(ui->textEditResult, SIGNAL(textChanged()), this, SLOT(onResultBoxTextChanged()));
    connect(this, SIGNAL(calculationStateChanged(CalculationState)),
            this, SLOT(onCalculationStateChanged(CalculationState)));
    connect(this, SIGNAL(resultBoxStateChanged(ResultBoxState)),
            this, SLOT(onResultBoxStateChanged(ResultBoxState)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onResultBoxTextChanged()
{
    if(ui->textEditResult->toPlainText().isEmpty()){
        emit resultBoxStateChanged(ResultBoxState::BoxEmpty);
    }else{
        if(ResultBoxState::BoxEmpty == currentBoxState_){
            currentBoxState_ = ResultBoxState::BoxNotEmpty;
            emit resultBoxStateChanged(ResultBoxState::BoxNotEmpty);
        }
    }
}

// 比较用户输入的校验码和计算出来的校验码是否匹配
// 如果匹配，则选中输出结果中的匹配项
// 如果不匹配，则弹出消息框提示用户。
void MainWindow::on_pushButtonComparison_clicked()
{

}

// 浏览待校验的用户文件
void MainWindow::on_pushButtonBrowse_clicked()
{
    QFileDialog dlg(nullptr, "选择要进行校验的文件");
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    // 说明：QFileDialog无法实现在同一对话框中即能选择文件又能选择文件夹
    // 故而，这里退而求其次，只选择打开文件
    dlg.setFileMode(QFileDialog::ExistingFiles);

    QStringList filters{"压缩文件(*.ZIP *.ZIPX *.RAR *.7Z "
                        "*.ISO *.IMG *.ISZ *.CAB *.ARJ *.ACE "
                        "*.ALZ *.UUE *.TAR *.GZ *.GZIP *.TGZ "
                        "*.TPZ *.BZIP2 *.BZ2 *.BZ *.TBZ *.TBZ2 "
                        "*.XZ *.TXZ *.LZH *.LHA *.Z *.TAZ *.XPI "
                        "*.JAR *.WIM *.SWM *.RPM *.XAR *.DEB *.DMG "
                        "*.HFS *.CPIO *.LZMA *.LZMA86 *.SPLIT)",
                        "可执行文件(*.EXE *.MSI)", "所有文件(*.*)"};
    dlg.setNameFilters(filters);
    if(dlg.exec()){
        QStringList filePaths = dlg.selectedFiles();
        if(!filePaths.isEmpty()){
            calculate(filePaths);
        }
    }
}

// 复制计算结果到剪切板中
void MainWindow::on_pushButtonCopy_clicked()
{

}

// 清空计算结果文本框
void MainWindow::on_pushButtonClear_clicked()
{

}

// 保存计算结果到文件中
void MainWindow::on_pushButtonSave_clicked()
{

}

// 关闭程序/停止计算
void MainWindow::on_pushButtonClose_clicked()
{

}

void MainWindow::onCalculationStateChanged(CalculationState stateAfterChanged)
{
    calculationState_ = stateAfterChanged;
    if(CalculationState::Calculating == stateAfterChanged){
        // 程序开始进行计算。需要禁用一些控件
    }else{
        // 程序停止计算。需要启用一些控件
    }
}

void MainWindow::onResultBoxStateChanged(ResultBoxState stateAfterChanged)
{
    if(ResultBoxState::BoxEmpty == stateAfterChanged){
        disableSomeControls();
    }else{
        if(CalculationState::Calculating != calculationState_){
             enableSomeControls();
        }
    }
}

void MainWindow::setResultBoxTipText()
{
    QString tipText = tr("\n\r\n\r\n\r\n\r\n\r\t\t\t\t\t\t\t\t\t\t\t\t\t\t"
                         "\t\t\t\t\t\t\t请拖拽文件或者文件夹到这里进行校验...");
    ui->textEditResult->setPlaceholderText(tipText);
}

void MainWindow::setDefaultCalculationOptions()
{
    ui->checkBoxMD5->setChecked(true);
    ui->checkBoxSHA1->setChecked(true);
    ui->checkBoxCRC32->setChecked(true);
}

void MainWindow::disableSomeControls()
{
    ui->pushButtonComparison->setEnabled(false);
    ui->pushButtonCopy->setEnabled(false);
    ui->pushButtonClear->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
}

void MainWindow::enableSomeControls()
{
    ui->pushButtonComparison->setEnabled(true);
    ui->pushButtonCopy->setEnabled(true);
    ui->pushButtonClear->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //如果为文件，则支持拖放
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    QStringList allFilesPath;
    for(QUrl url : urls) {
        QFileInfo info(url.toLocalFile());
        if(info.isFile()){
            allFilesPath.append(info.absoluteFilePath());
        }else if(info.isDir()){
            QList<QString> paths = FileUtils::enumeratesAllFilesInTheDir(info.absoluteFilePath());
            allFilesPath.append(paths);
        }
    }

    if(!allFilesPath.isEmpty()){
        calculate(allFilesPath);
    }
}

void MainWindow::calculate(const QStringList &filePaths)
{
    bool md5 = ui->checkBoxMD5->isChecked();
    bool sha1 = ui->checkBoxSHA1->isChecked();
    bool sha224 = ui->checkBoxSHA224->isChecked();
    bool sha256 = ui->checkBoxSHA256->isChecked();
    bool sha384 = ui->checkBoxSHA384->isChecked();
    bool sha512 = ui->checkBoxSHA512->isChecked();
    bool crc32 = ui->checkBoxCRC32->isChecked();
    if(md5 || sha1 || sha224 || sha256 || sha384 || sha512 || crc32){
        emit calculationStateChanged(CalculationState::Calculating);

    }
}
