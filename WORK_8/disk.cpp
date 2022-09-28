#include "disk.h"
#define maxn 200
Disk::Disk() // 初始化
{
    for (int i = 0;i<maxn;i++)
    {
        track[i] = 0;
    }
}
bool Disk::checkTrack() // 检查 全部轨道都为0 则返回1 否则返回0
{
    for (int i = 0;i<maxn;i++)
    {
        if (track[i])   return 0;
    }
    return 1;
}
void Disk::init(float _startUp,float _rotationRate,float _moveSpeed,int _blockNum,int _byteNum)
{
    startUp = _startUp;     moveSpeed = _moveSpeed;     byteNum = _byteNum;
    rotationRate = _rotationRate;   blockNum = _blockNum;
}
// 随机产生当前磁头所在磁道以及磁头移动方向
void Disk::randomInit()
{
    direction = rand()%2;
    nowPlace = rand()%maxn;
}
// 随机产生并显示磁道I/O访问序列S
QVector<int> Disk::randomSeq(int num)
{

    QVector<int> seqReturn;
    for (int i = 0;i<num;i++)
    {
        int id = rand()%maxn;
        seqReturn.push_back(id);
    }
    return seqReturn;
}
// 类内私有函数，输入移动量和访问的扇区数，给出寻道时间、平均旋转延迟、访问时间、总时间
void Disk::getTimes(int trackNum,float moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access)
{
    _seek = moveNum*moveSpeed + startUp;
    _aveRotationDelay = 1/(2*rotationRate) * trackNum;
    _transmit = 1/(rotationRate * blockNum) * trackNum;
    _access = _seek + _aveRotationDelay + _transmit;
}
// 先到先服务算法 输入一个序列 返回一个处理序列 并且返回引臂移动量，寻道时间，旋转延迟，传输时间，访问处理时间
QVector<int> Disk::FCFS(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access)
{
    QVector<int> seqReturn;
    _moveNum = 0;
    int beforePlace = nowPlace;
    for(auto k : seq)
    {
        _moveNum += abs(k - beforePlace);
        beforePlace = k;
        seqReturn.push_back(k);
    }
    int trackNum = seq.size();
    getTimes(trackNum,_moveNum,_seek,
             _aveRotationDelay,_transmit,_access);
    return seqReturn;
}
// 最短寻找时间算法
QVector<int> Disk::SSTF(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access)
{
    QVector<int> seqReturn;
    _moveNum = 0;
    int trackNum = seq.size();
    int beforePlace = nowPlace;
    int min = maxn+1;
    int nextPlace;
    for (int i = 0;i<trackNum;i++)
    {
        min = maxn+1;
        int index;
        for (int k = 0;k<seq.size();k++)
        {
            if(min > abs(beforePlace - seq[k]))
            {
                min = abs(beforePlace - seq[k]);
                nextPlace = seq[k];
                index = k;
            }
        }
        _moveNum += min;
        seq.erase(seq.begin()+index);
        if (min)
        {
            seqReturn.push_back(nextPlace);
        }
        beforePlace = nextPlace;
    }
    getTimes(trackNum,_moveNum,_seek,
             _aveRotationDelay,_transmit,_access);
    return seqReturn;
}
// 扫描算法 SCAN
QVector<int> Disk::SCAN(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access)
{
    QVector<int> seqReturn;
    _moveNum = 0;
    int beforePlace = nowPlace;
    for (auto k :seq)
    {
        track[k]++;
    }
    int cnt = 0;    // 访问扇区数目
    int i;
    while (1)
    {
        if (direction == 1) // 向外移动
        {
            for (i = beforePlace;i<maxn;i++)
            {
                if (track[i])
                {
                    _moveNum += abs(beforePlace - i);
                    seqReturn.push_back(i);
                    track[i] = 0;
                    beforePlace = i;
                    cnt++;
                }
            }
            if (checkTrack())
                break;
            if(beforePlace != maxn-1)
            {
                _moveNum += abs(nowPlace - maxn+1);
                seqReturn.push_back(maxn-1);
            }
            beforePlace = maxn-1;
            direction = 0;
        }
        else    // 向内移动
        {
            for (i = beforePlace;i>=0;i--)
            {
                if (track[i])
                {
                    _moveNum += abs(beforePlace - i);
                    seqReturn.push_back(i);
                    track[i] = 0;
                    beforePlace = i;
                    cnt++;
                }
            }
            if (checkTrack())
                break;
            if (beforePlace != 0)
            {
                _moveNum += abs(beforePlace - 0);
                seqReturn.push_back(0);
            }
            beforePlace = 0;
            direction = 1;
        }

    }

    getTimes(cnt,_moveNum,_seek,
             _aveRotationDelay,_transmit,_access);
    return seqReturn;
}
// 电梯算法 LOOK
QVector<int> Disk::LOOK(QVector<int>seq,float &_moveNum,float &_seek,float &_aveRotationDelay,float &_transmit,float &_access)
{
    QVector<int> seqReturn;
    _moveNum = 0;
    for (auto k :seq)
    {
        track[k]++;
    }
    int beforePlace = nowPlace;
    int cnt = 0;
    int i;
    while (1)
    {
        if (direction == 1) // 向外移动
        {
            for (i = beforePlace;i<maxn;i++)
            {
                if (track[i])
                {
                    _moveNum += abs(beforePlace - i);
                    seqReturn.push_back(i);
                    track[i] = 0;
                    beforePlace = i;
                    cnt++;
                }
            }
            if (checkTrack())
                break;
            direction = 0;
        }
        else    // 向内移动
        {
            for (i = beforePlace;i>=0;i--)
            {
                if (track[i])
                {
                    _moveNum += abs(beforePlace - i);
                    seqReturn.push_back(i);
                    track[i] = 0;
                    beforePlace = i;
                    cnt++;
                }
            }
            if (checkTrack())
                break;
            direction = 1;
        }

    }
    getTimes(cnt,_moveNum,_seek,
             _aveRotationDelay,_transmit,_access);
    return seqReturn;
}
