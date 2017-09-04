#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum ResultBoxState{
        BoxEmpty, BoxNotEmpty
    };

    enum CalculationState{
        Calculating, Stopped
    };

signals:
    void resultBoxStateChanged(ResultBoxState stateAfterChanged);
    void calculationStateChanged(CalculationState stateAfterChanged);

public slots:
    void onResultBoxTextChanged();

private slots:
    void on_pushButtonComparison_clicked();

    void on_pushButtonBrowse_clicked();

    void on_pushButtonCopy_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonClose_clicked();

    void onCalculationStateChanged(CalculationState stateAfterChanged);

    void onResultBoxStateChanged(ResultBoxState stateAfterChanged);
protected:
    void setResultBoxTipText();

    void setDefaultCalculationOptions();

    void disableSomeControls();

    void enableSomeControls();

    // 当用户拖动文件到窗口上的时候，就会触发dragEnterEvent事件
    void dragEnterEvent(QDragEnterEvent *event) override;

    // 当用户放下这个文件后，就会触发dropEvent事件
    void dropEvent(QDropEvent *event) override;

    // 计算
    void calculate(const QStringList& filePaths);
private:
    Ui::MainWindow *ui;
    ResultBoxState currentBoxState_;
    CalculationState calculationState_;
};

#endif // MAINWINDOW_H
