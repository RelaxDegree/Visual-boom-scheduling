#include "mainwindow.h"
#include "ui_mainwindow.h"
#define maxn 200
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 设置Slider */
    ui->horizontalSlider->setRange(0,maxn-1);
    ui->horizontalSlider->setValue(0);  nowPlace = 0; // 初始化滚动条
    ui->horizontalSlider_2->setRange(0,maxn-1);
    ui->horizontalSlider_2->setTickInterval(10);  // 设置刻度间隔
    ui->horizontalSlider_2->setTickPosition(QSlider::TicksAbove);

    /* 设置计时器 */
    timer1 = new QTimer(this);  // 计时器 每隔若干秒触发一次
    connect(timer1,SIGNAL(timeout()),this,SLOT(slotSpin()));    // 计时器与槽函数绑定

    /* 设置折线图 */
    cview = new QChartView();
    chart = new QChart();
    chart->legend()->hide();
    cview->setChart(chart);

    /* 设置折线图坐标 */
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    axisX->setRange(0,199);
    axisX->setTickCount(20);
    axisY->hide();

    /* 磁盘参数初始化 */
    initDiskParas();

    QGridLayout *baseLayout = new QGridLayout(); //便于显示，创建网格布局
    baseLayout->addWidget(cview, 200, 10);
    ui->widgetGraph->setLayout(baseLayout); //显示到QWidget控件
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::paintLine(int x1,int x2,int times) // 画线函数 x1 x2为横坐标 times为纵坐标
{
    if (x1>x2)  std::swap(x1,x2);
    for (float i=x1;i<=x2;i+=0.1)
    {
        series[times-1]->append(i,times);
    }

}
void MainWindow::moveToPlace(int _toPlace,int delay) // Slider 移动 移动到指定位置
{
    moveMode = 0;   // 单步模式
    toPlace = _toPlace;
    timer1->start(delay);
}
void MainWindow::moveToPlace(QVector<int>_seq,int delay)// Slider 移动 根据序列移动
{
    moveMode = 1;   // 逐步模式
    axisY->setTickCount(_seq.size()+1);
    axisY->setRange(1, _seq.size()+1);
    cnt = 0;
    seq = _seq;
    timer1->start(delay);
}
void MainWindow::on_pushButton_clicked() // 选择算法与速度 开始移动
{
    float moveNum,seek,aveRotationDelay,transmit,access;
    int num = ui->editNum->text().toInt();
    QVector<int>t = disk.randomSeq(num);    // 产生num随机序列
    QString str;
    for (auto k : t)
    {
        str = str + " " + QString::number(k);
    }
    ui->labelSeq->setText(str);
    resetSeries();

    if (ui->comboBox->currentText() == "FCFS")
    {
        seq = disk.FCFS(t,moveNum,seek,aveRotationDelay,transmit,access);
    }
    else if (ui->comboBox->currentText() == "SSTF")
    {
        seq = disk.SSTF(t,moveNum,seek,aveRotationDelay,transmit,access);
    }
    else if (ui->comboBox->currentText() == "SCAN")
    {
        seq = disk.SCAN(t,moveNum,seek,aveRotationDelay,transmit,access);
    }
    else if (ui->comboBox->currentText() == "LOOK")
    {
        seq = disk.LOOK(t,moveNum,seek,aveRotationDelay,transmit,access);
    }
    int delay;
    if (ui->comboBox_2->currentText() == "一档速度")
        delay = 15;
    else if (ui->comboBox_2->currentText() == "二档速度")
        delay = 10;
    else if (ui->comboBox_2->currentText() == "三档速度")
        delay = 5;
    else
        delay = 20;
    initSeries(seq.size());     // 初始化折线图
    moveToPlace(seq,delay);    // 发出开始移动的信号
    showDirection();           // 显示移动方向
    showDiskResults(moveNum,seek,aveRotationDelay,transmit,access);
}

void MainWindow::slotSpin()
{
    if (!moveMode)  // 单步移动
    {
        if(nowPlace<toPlace)
        {
           disk.direction = 1;showDirection();
           nowPlace+=1;
           ui->horizontalSlider->setValue(nowPlace);
           ui->horizontalSlider_2->setValue(nowPlace);
        }
        else if(nowPlace>toPlace)
        {
            disk.direction = 0;showDirection();
            nowPlace-=1;
            ui->horizontalSlider->setValue(nowPlace);
            ui->horizontalSlider_2->setValue(nowPlace);
        }
        else
        {
            timer1->stop();
        }
    }
    else    // 根据序列移动
    {
        toPlace = seq[cnt];
        if(disk.nowPlace<toPlace)
        {
           disk.direction = 1;showDirection();  // 显示方向
           paintLine(disk.nowPlace,disk.nowPlace+1,seq.size()-cnt); // 画线
            disk.nowPlace+=1;
           ui->horizontalSlider->setValue(disk.nowPlace);
           ui->horizontalSlider_2->setValue(disk.nowPlace);


        }
        else if(disk.nowPlace>toPlace)
        {
            disk.direction = 0;showDirection();
            paintLine(disk.nowPlace,disk.nowPlace-1,seq.size()-cnt);
            disk.nowPlace-=1;
            ui->horizontalSlider->setValue(disk.nowPlace);
            ui->horizontalSlider_2->setValue(disk.nowPlace);
        }
        else
        {
            series0->append(disk.nowPlace,seq.size()-cnt);

            ui->labelNowPlace->setText(QString::number(disk.nowPlace)); // 显示当前位置
            cnt++;
        }
        if (cnt == seq.size())
        {
            nowPlace = disk.nowPlace;

            timer1->stop();
        }
    }
}
// 产生随机序列
void MainWindow::on_btnRandomSeq_clicked()
{

}
// 随机初始化
void MainWindow::on_btnRandomInit_clicked()
{
    disk.randomInit();
    int delay;
    if (ui->comboBox_2->currentText() == "一档速度")
        delay = 15;
    else if (ui->comboBox_2->currentText() == "二档速度")
        delay = 10;
    else if (ui->comboBox_2->currentText() == "三档速度")
        delay = 5;
    else
        delay = 20;
    moveToPlace(disk.nowPlace,delay);
    ui->labelNowPlace->setText(QString::number(disk.nowPlace));
    showDirection();
}
void MainWindow::showDirection()    // 显示当前位置
{
    if (disk.direction)
    {
        ui->labelDirection->setText("→");
    }
    else
    {
        ui->labelDirection->setText("←");
    }
}

void MainWindow::on_btnReset_clicked()  // 清屏按钮
{
    resetSeries();
}
void MainWindow::resetSeries()  // 清除所有的折线
{
    for (int i = 0;i<series.size();i++)
    {
        delete series[i];
    }
    series.clear();
    delete series0;
    series0 = new QScatterSeries();
    chart->addSeries(series0);
    chart->setAxisX(axisX,series0);
    chart->setAxisY(axisY,series0);
    series0->setPointLabelsFormat("@xPoint");
    series0->setMarkerSize(15);
    series0->setPointLabelsVisible();
}
void MainWindow::initSeries(int num)    // 初始化num条数目的折线
{
    for (int i = 0;i<num;i++)
    {
        QLineSeries * ss = new QLineSeries;
        series.push_back(ss);
        chart->addSeries(ss);
        chart->setAxisX(axisX,ss);
        chart->setAxisY(axisY,ss);
    }
}

void MainWindow::on_btnSave_clicked()
{
    initDiskParas();
    ui->editNum->setEnabled(0);    ui->editMove->setEnabled(0);
    ui->editRR->setEnabled(0);    ui->editStartup->setEnabled(0);
    ui->editByteNum->setEnabled(0);    ui->editBlockNum->setEnabled(0);
}

void MainWindow::on_btnChange_clicked()
{
    ui->editNum->setEnabled(1);    ui->editMove->setEnabled(1);
    ui->editRR->setEnabled(1);    ui->editStartup->setEnabled(1);
    ui->editByteNum->setEnabled(1);    ui->editBlockNum->setEnabled(1);
}
void MainWindow::initDiskParas()
{
    float RR = ui->editRR->text().toFloat()/60000;
    disk.init(ui->editStartup->text().toFloat(),RR,
              ui->editMove->text().toFloat(),ui->editBlockNum->text().toFloat(),
              ui->editByteNum->text().toFloat());
}
void MainWindow::showDiskResults(float moveNum,float seek,float aveRotationDelay,
                                float transmit,float access)
{
    ui->labelMove->setText(QString("%1").arg(moveNum));
    ui->labelMoveTime->setText(QString("%1").arg(seek));
    ui->labelRR->setText(QString("%1").arg(aveRotationDelay));
    ui->labelTrans->setText(QString("%1").arg(transmit));
    ui->labelVisit->setText(QString("%1").arg(access));
}
