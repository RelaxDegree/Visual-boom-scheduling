#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <disk.h>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QtCharts>
using namespace QtCharts;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void slotSpin();

    void on_btnRandomSeq_clicked();

    void on_btnRandomInit_clicked();

    void on_btnReset_clicked();

    void on_btnSave_clicked();

    void on_btnChange_clicked();

private:
    Ui::MainWindow *ui;
    QTimer* timer1;
    Disk disk;
    int toPlace;
    int nowPlace;
    bool moveMode; // 0为单步移动 1为逐步移动
    QVector<int>seq;    // 存储结果序列的向量 可视化磁盘条
    int cnt;        // 当前处理完了序列的个数  用于可视化磁盘条

    void moveToPlace(int _toPlace,int delay);   // 单步移动
    void moveToPlace(QVector<int>_seq,int delay);   // 逐步移动
    void showDirection();

    QChartView* cview;
    QChart* chart;
    QVector<QLineSeries*>series;    // 存储折线的容器
    QValueAxis* axisX;
    QValueAxis* axisY;
    QScatterSeries *series0;
    void paintLine(int x1,int x2,int times);
    void resetSeries();
    void initSeries(int num);
    void initDiskParas();
    void showDiskResults(float moveNum,float seek,float aveRotationDelay,
                        float transmit,float access);
};
#endif // MAINWINDOW_H
