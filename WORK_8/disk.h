#ifndef DISK_H
#define DISK_H
#define maxn 200
#include <QVector>
#include <random>
#include <math.h>
#include <QDebug>
class Disk
{
public:
    Disk();
    void init(float _startUp,float _rotationRate,float _moveSpeed,int _blockNum,int _byteNum);
    int nowPlace;       // 磁头目前位置
    int track[maxn];   // 磁道请求
    bool direction;     // 磁头方向 1 向外 0 向内
    float startUp;        // 启动时间
    float rotationRate;   // 磁头转速   转/毫秒  = RPM/(60*1000)
    float moveSpeed;      // 跨越1个磁道所用时间
    int blockNum;       // 扇区数
    int byteNum;        // 扇区字节数
    void getTimes(int trackNum,float moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access);
    // 类内私有函数，输入移动量和访问的扇区数，给出寻道时间、平均旋转延迟、访问时间、总时间
    void randomInit();
    // 随机目前位置以及移动方向
    bool checkTrack();
    // 检查当前轨道是否全部空闲 0 不空闲 1 空闲
    QVector<int> randomSeq(int num);
    // 产生随机序列 num是随机序列的数目
    QVector<int> FCFS(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access);
    // 先到先服务算法 输入一个序列 返回一个处理序列 并且返回引臂移动量，寻道时间，旋转延迟，传输时间，访问处理时间
    QVector<int> SSTF(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access);
    // 最短寻找时间算法
    QVector<int> SCAN(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access);
    // 扫描算法
    QVector<int> LOOK(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access);

};

#endif // DISK_H
