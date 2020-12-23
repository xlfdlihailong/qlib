//
// Created by Administrator on 2020/12/21.
//

#ifndef PLIB_PTCP_H
#define PLIB_PTCP_H
#include "plib.h"

/*
 * 需要自己关闭socket，调用close
 * 考虑多线程的问题,一个界面如果有刷新线程,建议另来一个连接
 * 一个连接干一个事情,server端是来一个连接新建一个线程单独处理不影响
 * client端则要自己注意,一个线程干一个事情,用recvx的时候注意一下,因为recvx用到了全局变量data,不要多个线程收就ok,不然会错乱
 */
class ptcp {
    //接收数据缓冲区，不再用char*，提供专门的转换接口
public:
    ctcp *pCtcp;
    string strCtcptype;// client server 初始是init
    //这三个要考虑多线程同时的问题
    pstring data;
    plist<ptcp *> listclient;
    string type;
    int length;
    string ip;
    int port;
    int sock;
    bool bIsConnecting;//判断是否在重连,防止多线程同时重连
    const char *getData();//接收的数据，用于转换
    pstring getDataToString();//专用于字符串

    void setCtcp(ctcp *ct);

    ctcp *getCtcp();

    //内部删除listclient用
    void deleteClientByIndex(int i);


    //这里返回的是一个新的plist<ptcp *>，相当与复制了一份,只能读取，不能操作，不然出错
    //若想操作应该返回plist<ptcp *>*类型
    plist<ptcp *> getAllClients();

    //通过ip查找ptcp
    ptcp *getPtcpByIp(string ip);

    ptcp();

    //接受连接时用,只会是server //该方法不需要init
    ptcp(int iSockAccept, string ip, int port);

    void init(string ip, int port);

    ptcp(string ip, int port);

    //    int update();//处理初始化失败的问题//默认阻塞3秒超时，一般用于 ftp
    int connect();//若是客户端,则调用该方法
    //针对客户端，断线重连,重试次数，次数之间的间隔,默认参数代表无限重连
    //加了是否在重连判断,因此可以在任何情况下连不上就调用该函数,可以保证
    //同一时间只有一个在重连,重连的标准是发送或者接收失败,只有这两种情况
    int reconnect(int iretrycount = -1, int itimesecond = 1);

    //连接失败后线程重连,不需要提示的话就这样,如果需要提示则在ui中封装
    //void MainWindow::slotThreadConnect()
    //{
    //    if(ptc->connect()<0)
    //    {
    //        sigShowStatusBar("连接服务器失败!重连中......");
    //        ptc->reconnect();
    //    }
    //    sigShowStatusBar("连接服务器成功");
    //}
    //或者做个回调函数
    //这个就包括了连接和线程重连,可以每次发送或者接收失败都调用一次connectx
    //同时reconnect中有判断,可以保证始终只有一个线程在重连
    int connectx() {
        if (this->connect() < 0) {
            hlog(pstring() << "连接" << this->ip << ":" << this->port << "失败,重连中...");
            std::thread(&ptcp::threadReconnect, this).detach();
            return -1;
        }
        hlog(pstring() << "连接" << this->ip << ":" << this->port << "成功!");
        return 1;
    }

    void threadReconnect() {
//        hlog(pstring()<<"连接"<<this->ip<<":"<<this->port<<"失败,重连中...");
//如果返回-的说明已经有其他线程在重连,那么这个重连判定失败sleep1秒
        if (this->reconnect() < 0) {
            sleep(1);
        } else {
            hlog(pstring() << "重新连接" << this->ip << ":" << this->port << "成功!");
        }

    }

    int connect2();//通过域名获取外网ip
    //下面这个构造函数不许要init
    ptcp(ctcp *tcpin);//根据xtcp转换
    //当socket连接之后获取封装对象，仅用与客户端
    ptcp(int sock);//根据sock初始化tcp
    ~ptcp();//因为连接的特殊性，一般手动关闭，所以在此不释放，否则导致多次释放的问题，也不能重载<<

    //也就是关闭连接//使用shutdown，多线程使用socket也都会关闭，如果用close不行
    int close();

    /*    void thread_typestring(ptcp* p)
    //    {
    //        if(p->strtype=="test")
    //        {
    //            pstring pstr=p->getClass<pstring>();
    //            hlog(pstr);
    //        }
    //    }

    //    void thread_typeint(ptcp* p)
    //    {
    //        if(p->getTypeInt()==1111)
    //        {
    //            //        hlog("get");
    //            double dbrecv=*(double*)(p->getData());
    //            hlog(dbrecv);
    //        }
    //    }
    //    //测试客户端
    //    void thread_testtcp(void* para)
    //    {
    //        ptcp tcptest(plib::getIPLocal(),4567);
    //        hlog(tcptest.connect());
    //        sleep(1);
    //    //        hlog(tcptest.sendp(1111,22.334));
    //            hlog(tcptest.sendb("test",pstring("xlfdlihailong@126.com")));

    //    //    char data[5];bzero(data,5);strcpy(data,"xlfd");
    //    //    hlog(tcptest.send(data,4));
    //        sleep(1);
    //        tcptest.close();

    //    }
    //    void thread_selfwithWhile(ptcp* p)
    //    {
    //不用pack的时候这么写
    //        while(1)
    //        {
    //            char data[5];
    //            bzero(data,5);
    //            if(p->recv(data,4)>0)
    //                hlog(data);
    //            else
    //                break;
    //        }
    //    }
    //        ptcp tcp(plib::getIPLocal(),4567);
    //        //测试客户端，本机发送
    //        pthread thtest;thtest.start(thread_testtcp);

    //        tcp.setThreadServer(thread_typestring);
    //    tcp.setThreadServer(thread_typeint,1);
    //    tcp.setThreadServer(thread_selfwithWhile,2);
    //用pack的时候这么写
    //    while(true)
    //    {
    //        if(p->recvx()<0)
    //            break;
    //    。。。
    //}
    //默认用这个来写，因为比较灵活，在业务中有时候需要continue，如连不上数据库的时候
    //server类型,是int类型还是string 0标识string，2是nopack,2要自己处理关闭连接等
    //用2时，还能处理发现连接处理一件事情的情况，如果用0则必须收到包才能处理*/
    int setThreadServer(void (*fun_thread)(ptcp *), int itype = 2);

    //放到线程中去
    int setThreadServerDetach(void (*fun_thread)(ptcp *), int itype = 2);

    //专门为类成员函数准备的
    template<class T>
    int setThreadServerClass(void (T::*funtest)(ptcp *para), T *pthis) {
        int iSock = ctcp_InitTcpSocket();
        if (iSock < 0) {
            hlog("iSock<0");
            ctcp_CloseConnection(iSock);
            return -1;
        }
        int ibind = ctcp_BindSocket(iSock, this->ip.c_str(), this->port);
        if (ibind < 0) {
            hlog("bind fail");
            hlog(strerror(errno));
            ctcp_CloseConnection(iSock);
            return -2;
        }
        this->sock = iSock;

        //不停等待，来一个，创建一个线程
        while (true) {
            int iClientSock = -1;
            int iLength;
            struct sockaddr_in struSin;
            iLength = sizeof(struSin);
            memset(&struSin, 0, iLength);
            iClientSock = accept(this->sock, (struct sockaddr *) &struSin, (socklen_t *) &iLength);
            if (iClientSock < 0)
                continue;
            //            hlog(iClientSock);
            string clientip = string(inet_ntoa(struSin.sin_addr));
            //            hlog(clientip);
            ptcp *ptnew = new ptcp(iClientSock, clientip, this->port);

            //            std::function<void (void)> func = std::bind(funtest,pthis,ptnew);
            //            std::thread thserver(func);

            std::thread thserver(funtest, pthis, ptnew);
            //        //如果不调用,线程完了不自动释放资源
            thserver.detach();

        }
    }

    //专门为类成员函数准备的,直接多线程
    //ps->setTcpServerxClassThread(&MainWindow::slotThreadServer2,this);
    //参数ptcp* p,这个没有封装业务函数,只能自己写while循环
    //    void MainWindow::slotThreadServer2(ptcp *p)
    //    {
    //        while(1)
    //        {
    //            if(p->recvx()<0)
    //                break;
    //            if(p->strtype=="test")
    //            {
    //                int irecv=p->getClass<int>();
    //                hlog(irecv);
    //            }
    //        }
    //    }
    template<class T>
    void setThreadServerClassDetach(void (T::*funtest)(ptcp *para), T *pthis) {
        std::thread thServer(&ptcp::setThreadServerClass<T>, this, funtest, pthis);
        thServer.detach();
    }


    //    map<xptcp*,bool> mapTcpThread;//所有客户端ptcp与线程号的对应关系
    //要map的key使用自定义的值，则必须要重载<，const必须这样，不然不认
    bool operator<(const ptcp &pCtcp) const;

    //不收到指定字节就退出，收多少返回多少，适用于转发,若收20，发46，则收三次20，20，6，若收512k发46则收46
    //=如果实际接收包的长度小于0，说明接收时出现了错误，包括对方关闭了连接，判断时就是小于0就失败
    int recvNoFullPack(char *arrecv, int ilenWantRecv, int timeout = -1);

    //基本接收，发送
    int recv(char *acrecv, int len, int timeout = -1) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpRecvFullPacketTimeout(pCtcp->sock, acrecv, len, timeout);
        //        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
//            hlog(this->strCtcptype);
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }

    int send(const char *acsend, int len) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpSendFullPacketTimeout(pCtcp->sock, acsend, len, 5);
//        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }

    //不能用下边这个，不然就发送send(buf,sizeof(TASK_STATUS_STRUCT)时默认选择这个，就会导致有问题
    //    int send(T t,int timeout=5)
    template<class T>
    int send(T t) {
        if (this->strCtcptype == "init") {
            hlog(pstring() << "TCP未初始化,请先指定tcp的类型是客户端还是服务端 " << this->ip << ":" << this->port);
            return -999;
        }
        int ires = ctcpSendFullPacketTimeout(pCtcp->sock, &t, sizeof(t), 5);
//        //如果发送失败,包括超时,就重连,是客户端才行
        if (ires < 0) {
            if (this->strCtcptype == "client") {
                this->connectx();
            }
        }
        return ires;
    }


    int sendstring(string strtype, string content) {
        return sendx(strtype, content.c_str(), content.size());
    }

    /*序列化方案
     * //send用这个,支持发送类，字符串，不支持结构体
     * 结构体直接用sendx(三参数形式)
     */
    template<class T>
    int sendx(string type, T t) {
        /*如果发的是字符串，则直接发送字符串，不转了，如果不是，则要
        //转成字符串发送，发送的字符串是全的，只有反序列化时才会出现不全
        //的情况，因此发送全用string*/
        if (typeid(t) == typeid(string) || typeid(t) == typeid(pstring)) {
            return this->sendstring(type, *((string *) (&t)));
        }
        return this->sendstring(type, common_toString(t));
    }

    //只有小于0才是发送失败，等于0是只发了类型
    int sendx(pstring type, const char *pdata = NULL, int length = 0) {
        //这是为了防止这样的调用:ptcp->sendx("res","xfdsfds")
        if (pdata != NULL && length == 0)
            return sendx(type, pdata, (string(pdata).size()));

        STRUCT_HEADER sHeader;
        memset(&sHeader, 0, sizeof(STRUCT_HEADER));
        //        sHeader.type=type;
        strcpy(sHeader.arrchType, type.c_str());
        sHeader.length = length;
        pstring strall;
        strall.resize(sizeof(STRUCT_HEADER) + length);
        char *pstart = (char *) (&(*(strall.begin())));
        memcpy(pstart, &sHeader, sizeof(STRUCT_HEADER));
        //非空的话要把数据弄上去
        if (pdata != NULL) {
            memcpy(pstart + sizeof(STRUCT_HEADER), pdata, length);
        }
        int iResSend = this->send(strall.c_str(), strall.size());
        if (iResSend != strall.size()) {
            hlog("发送数据失败，未全部发送：", iResSend, strall.size());
            return -5;
        }
        if (iResSend == sizeof(STRUCT_HEADER)) {
            //如果只发了类型，则返回数据长度0
            return 0;
        }
        return sHeader.length;
    }

    //>0表示接收成功==0表示只发类型
    int recvx(int timeout = -1) {
        STRUCT_HEADER sHeader;
        memset(&sHeader, 0, sizeof(STRUCT_HEADER));
        int iresHeader = -1;
        if ((iresHeader = this->recv((char *) &sHeader, sizeof(STRUCT_HEADER), timeout)) < 0) {
            //            hlog("接收包头失败");
            //不要打印，不然会一直打印，因为如果接收不到就一直打
            return iresHeader;
        }

        this->type.assign(sHeader.arrchType);
        //如果只发了类型，则返回数据长度0
        if (sHeader.length == 0)
            return 0;
        if (sHeader.length < 0) {
            return -9;
        }
        this->length = sHeader.length;
        //pstring
        this->data.resize(sHeader.length);
        //    hlog(sHeader.length);
        int iRecvDataRes = this->recv((char *) (&(*(this->data.begin()))), sHeader.length, timeout);
        if (iRecvDataRes < 0) {
            //            hlog("接收数据失败");
            return iRecvDataRes;
        }
        if (iRecvDataRes != sHeader.length) {
            //            hlog("接收到的数据长度与包头中的长度不一致，接收失败");
            return -8;
        }
        return iRecvDataRes;
    }


    template<class T>
    T getClass() {
        //        hlog(typeid(T).name());

        //如果本来就是string，就别转了，不然出问题
        //        if(typeid(T).name()==typeid(string).name())
        //        return strdata;


        return common_toClass<T>(this->data);
    }
    int recv(pstring& strDataRecv,int len,int iTimeoutSecond=-1)
    {
        strDataRecv.resize(len);
        int iResRecv=recv((char*)strDataRecv.data(),len,iTimeoutSecond);
        return iResRecv;
    }
    presult recv(int len,int iTimeoutSecond=-1)
    {
        pstring strDataRecv;
        int ires=recv(strDataRecv,len,iTimeoutSecond);
        return presult(ires,strDataRecv);
    }

    //>=0表示接收成功==0表示只发类型
    presult recvz(int timeout=-1)
    {
        STRUCT_HEADER sHeader;
        memset(&sHeader,0,sizeof(STRUCT_HEADER));
        int iresHeader=-1;
        if((iresHeader=this->recv((char*)&sHeader,sizeof(STRUCT_HEADER),timeout))<0)
        {
            hlog("###### recv header fail ####");
            //不要打印，不然会一直打印，因为如果接收不到就一直打
            return presult(iresHeader);
        }
        //        hlog(iresHeader);
        this->type.assign(sHeader.arrchType);
        if(sHeader.length==0)
        {
            hlog("recv only type");
            return presult(0);
        }
        if(sHeader.length<0)
        {
            hlog("recv header len<0,fail");
            return presult(-9);
        }
        pstring strData;
        strData.resize(sHeader.length);
        int iRecvDataRes=this->recv((char*)strData.data(),sHeader.length,timeout);
        if(iRecvDataRes<0)
        {
            hlog(pstring()<<"recv data fail:"<<iRecvDataRes);
            return presult(iRecvDataRes);
        }
        if(iRecvDataRes!=sHeader.length)
        {
            hlog("the len of data != len in header, recv fail");
            return presult(-8);
        }
        return presult(iRecvDataRes,strData);
    }

    template <class T>
    T getClassFromPresult(presult res)
    {
        return common_toClass<T>(res.data);
    }

    template <class T>
    T getClassFromDataString(pstring strData)
    {
        return common_toClass<T>(strData);
    }


    //支持hlog，重写cout,前提是不能自动释放
    friend ostream &operator<<(ostream &os, ptcp tcp) {
        os << "[ip: " << tcp.ip << "   port: " << tcp.port << "  sock: " << tcp.sock << "   type: " << tcp.strCtcptype
           << "]";
        if (tcp.strCtcptype == "server") {
            string allclients;
            if (tcp.listclient.size() > 0) {
                allclients = "  当前连接到本tcp服务的客户端如下:\n{\n";
                if (tcp.getAllClients().size() > 0) {
                    for (size_t i = 0; i < tcp.getAllClients().size() - 1; i++) {
                        allclients += tcp.getAllClients()[i]->ip + " ";
                    }
                    allclients += tcp.getAllClients()[tcp.getAllClients().size() - 1]->ip + "\n}";
                } else
                    allclients += "}";
            } else {
                allclients = " 当前无客户端连接到本tcp服务";
            }
            os << allclients;
        }
        return os;
    }

    //可打印ptcp*的，加一个就可以了！
    //支持hlog，重写cout,前提是不能自动释放
    friend ostream &operator<<(ostream &os, ptcp *tcp) {
        os << "[ip: " << tcp->ip << "   port: " << tcp->port << "  sock: " << tcp->sock << "   type: "
           << tcp->strCtcptype << "]";
        if (tcp->strCtcptype == "server") {
            string allclients;
            if (tcp->listclient.size() > 0) {
                allclients = "  当前连接到本tcp服务的客户端如下:\n{\n";
                if (tcp->getAllClients().size() > 0) {
                    for (size_t i = 0; i < tcp->getAllClients().size() - 1; i++) {
                        allclients += tcp->getAllClients()[i]->ip + " ";
                    }
                    allclients += tcp->getAllClients()[tcp->getAllClients().size() - 1]->ip + "\n}";
                } else
                    allclients += "}";
            } else {
                allclients = " 当前无客户端连接到本tcp服务";
            }
            os << allclients;
        }
        return os;
    }

private:

};


#define COMMENTS_LEN               128                          /*描述信息长度*/
#define FMT_TIME_LEN               20                           /*时间格式长度:yyyy-MM-dd hh:mm:ss*/
#define FILENAME_LEN               64                           /*文件名长度*/
#define RAW_FILENAME_LEN           256                          /*RawDataFile文件名长度*/
#define TASK_ID_LEN                42                           /*任务流水号长度*/
#define JOB_ID_LEN                 27                          /*作业任务编号长度*/
#define TASKARRAY_NUM_LEN          18                           /*任务单流水号长度*/
#define ROAD_ID_LEN                (JOB_ID_LEN + 6)             /*路标识长度*/
#define STAR_LEN                   20                           /*卫星名称长度*/
#define TASK_MODE_LEN              9                            /*任务作业方式长度*/
#define DEST_LEN                   6                            /*目的节点长度*/
#define SCHEID_LEN                 13                           //原始计划编号
#define MAX_ROAD_NUM               8                            /*最大路信息个数*/
#define MAX_RAWFILE_NUM           8                           /*最大原始数据传输文件个数*/
#define HTYPEGETCONF 91111111//查询conf
#define HTYPEADDCONF 91111112
#define HTYPEDELCONF 91111113
#define HTYPESETCONF 91111114
#define HTYPE_GETTASKS 0x9115//获取任务信息
#pragma pack(1)

//以下是获取传输所有服务器信息类
class TASK_HEAD {
public:
    int type;
    int length;

    TASK_HEAD() {
        this->length = 0;
    }
};

class TASK_ADDCONF {
public:
    char arrchkey[50];
    char arrchval[50];

    TASK_ADDCONF() {
        bzero(arrchkey, sizeof(arrchkey));
        bzero(arrchval, sizeof(arrchval));
    }
};

//传输通道信息
class ROAD_STATUS_STRUCT {
public:
    char arrchRoadNumber[33];            /*传输通道标识*/
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchRoadFileName[64];         /*本路传输文件名*/
    int iTransferRate;                           /*传输通道实际带宽*/
    short shFinishPercent;                         /*传输通道完成百分比*/
    unsigned long long ulTransferedData;                        /*传输数据量,数据类型需要修改成unsigned long*/
    char arrchServerIP[20];             /*传输服务器IP*/
    char arrchRealStartTime[20];        /*传输实际开始时间*/
    char arrchRealEndTime[20];          /*传输实际完成时间*/
    int iTransfersTime;                          /*传输时间*/
    int iCompressRatio;                          /*传输压缩比*/
    short shRoadState;                             /*传输通道状态：参见任务状态*/
    char arrchSystem[100];                        /*当前通道的系统GF,KJ,Z3*/
    char arrchTaskInfo[256];         /*通道的备注及说明*/
    short shLink;                                  //哪条链路
    char arrchTimeLast[30];                       //上一次更新时间
    char DTCIP[30];

    bool operator<(const ROAD_STATUS_STRUCT &road) const {
        //        if(strcmp(arrchRoadNumber,road.arrchRoadNumber)<0)
        string strall = string(arrchTaskSerialNumber) + string(arrchRoadNumber);
        string strallalien = string(road.arrchTaskSerialNumber) + string(road.arrchRoadNumber);
        if (strall < strallalien)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, ROAD_STATUS_STRUCT x) {
        os << "arrchRoadNumber[33]: ";
        os.write(x.arrchRoadNumber, 33 * sizeof(char));
        os << " ";
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchRoadFileName[64]: ";
        os.write(x.arrchRoadFileName, 64 * sizeof(char));
        os << " ";
        os << "iTransferRate: " << x.iTransferRate << " ";
        os << "shFinishPercent: " << x.shFinishPercent << " ";
        os << "ulTransferedData: " << x.ulTransferedData << " ";
        os << "arrchServerIP[20]: ";
        os.write(x.arrchServerIP, 20 * sizeof(char));
        os << " ";
        os << "arrchRealStartTime[20]: ";
        os.write(x.arrchRealStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchRealEndTime[20]: ";
        os.write(x.arrchRealEndTime, 20 * sizeof(char));
        os << " ";
        os << "iTransfersTime: " << x.iTransfersTime << " ";
        os << "iCompressRatio: " << x.iCompressRatio << " ";
        os << "shRoadState: " << x.shRoadState << " ";
        os << "arrchSystem[100]: ";
        os.write(x.arrchSystem, 100 * sizeof(char));
        os << " ";
        os << "arrchTaskInfo[256]: ";
        os.write(x.arrchTaskInfo, 256 * sizeof(char));
        os << " ";
        os << "shLink: " << x.shLink << " ";
        os << "arrchTimeLast[30]: ";
        os.write(x.arrchTimeLast, 30 * sizeof(char));
        os << " ";
        os << "DTCIP[30]: ";
        os.write(x.DTCIP, 30 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, ROAD_STATUS_STRUCT &x) {
        plib::skipCountch(is, 21);
        is.read((char *) x.arrchRoadNumber, 33);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 23);
        is.read((char *) x.arrchRoadFileName, 64);
        is.get();
        plib::skipCountch(is, 15);
        is >> x.iTransferRate;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.shFinishPercent;
        is.get();
        plib::skipCountch(is, 18);
        is >> x.ulTransferedData;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchServerIP, 20);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchRealStartTime, 20);
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchRealEndTime, 20);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iTransfersTime;
        is.get();
        plib::skipCountch(is, 16);
        is >> x.iCompressRatio;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shRoadState;
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchSystem, 100);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 256);
        is.get();
        plib::skipCountch(is, 8);
        is >> x.shLink;
        is.get();
        plib::skipCountch(is, 19);
        is.read((char *) x.arrchTimeLast, 30);
        is.get();
        plib::skipCountch(is, 11);
        is.read((char *) x.DTCIP, 30);
        is.get();
        return is;
    }

};


/*任务状态数据结构*/
class TASK_STATUS_STRUCT {
public:
    char arrchTaskSerialNumber[42];      /*任务流水号*/
    char arrchTaskArraySerialNumber[18]; //任务单流水号
    char arrchJobTaskID[27];              /*作业任务编号*/
    short shTaskPriority;                          //数据传输级别(0 一般 1较高 2 最高)
    short shInexecutingState;                      /*任务状态*/
    char arrchScheID[13];                 //原始计划编号
    char arrchTaskMode[9];            /*作业方式*/
    char arrchDataFileName[256];     /*原始数据文件名(T51时出现)*/
    char arrchSatelliteName[20];            /*卫星名称*/
    char arrchDataSource[6];               /*数据源地址*/
    char arrchDataDestination[6];          /*数据传输目的节点*/
    char arrchForeseeStartTime[20];     /*传输预计开始时间*/
    char arrchForeseeEndTime[20];       /*传输预计开始时间*/
    short shDestNum;                               //改过了，多目的地个数 %%非实时传输状态(3：上传OK  2：上传数据文件  1：上传DESC  0：未开始)
    short iFileExist;                              //数据文件是否存在(0：存在 1：不存在)
    char arrchFailReason[128];           //失败原因(用于判断T51源的标志和填写失败原因)
    int iRoadNumber;                             /*传输通道信息个数*/
    ROAD_STATUS_STRUCT arrstruRoadStatus[8];      /*传输通道信息*/
    short shSendDESC;                              //是否发送DESC文件()
    short shWhichLink;                             //哪个链路，0表示第一个，1表示第二个
    char arrchTaskInfo[128];             //通过第一个字节转int判断T51是否从接收站重传，已封装如下函数


    //是否从DTC开始重传
    bool isRetransFromDTC() {
        char arrchTmp[4];
        memset(arrchTmp, 0, sizeof(arrchTmp));
        memcpy(arrchTmp, this->arrchTaskInfo, 1);
        int iRet = atoi(arrchTmp);
        //1 从DTC 0从接受站
        if (iRet == 1)
            return true;
        return false;
    }

    bool operator<(const TASK_STATUS_STRUCT &t2) const {
        if (strcmp(arrchTaskSerialNumber, t2.arrchTaskSerialNumber) < 0)
            return true;
        return false;
    }

    friend ostream &operator<<(ostream &os, TASK_STATUS_STRUCT x) {
        os << "arrchTaskSerialNumber[42]: ";
        os.write(x.arrchTaskSerialNumber, 42 * sizeof(char));
        os << " ";
        os << "arrchTaskArraySerialNumber[18]: ";
        os.write(x.arrchTaskArraySerialNumber, 18 * sizeof(char));
        os << " ";
        os << "arrchJobTaskID[27]: ";
        os.write(x.arrchJobTaskID, 27 * sizeof(char));
        os << " ";
        os << "shTaskPriority: " << x.shTaskPriority << " ";
        os << "shInexecutingState: " << x.shInexecutingState << " ";
        os << "arrchScheID[13]: ";
        os.write(x.arrchScheID, 13 * sizeof(char));
        os << " ";
        os << "arrchTaskMode[9]: ";
        os.write(x.arrchTaskMode, 9 * sizeof(char));
        os << " ";
        os << "arrchDataFileName[256]: ";
        os.write(x.arrchDataFileName, 256 * sizeof(char));
        os << " ";
        os << "arrchSatelliteName[20]: ";
        os.write(x.arrchSatelliteName, 20 * sizeof(char));
        os << " ";
        os << "arrchDataSource[6]: ";
        os.write(x.arrchDataSource, 6 * sizeof(char));
        os << " ";
        os << "arrchDataDestination[6]: ";
        os.write(x.arrchDataDestination, 6 * sizeof(char));
        os << " ";
        os << "arrchForeseeStartTime[20]: ";
        os.write(x.arrchForeseeStartTime, 20 * sizeof(char));
        os << " ";
        os << "arrchForeseeEndTime[20]: ";
        os.write(x.arrchForeseeEndTime, 20 * sizeof(char));
        os << " ";
        os << "shDestNum: " << x.shDestNum << " ";
        os << "iFileExist: " << x.iFileExist << " ";
        os << "arrchFailReason[128]: ";
        os.write(x.arrchFailReason, 128 * sizeof(char));
        os << " ";
        os << "iRoadNumber: " << x.iRoadNumber << " ";
        os << "arrstruRoadStatus[8]: ";
        for (int i = 0; i < 8; i++) { os << x.arrstruRoadStatus[i] << " "; }
        os << " ";
        os << "shSendDESC: " << x.shSendDESC << " ";
        os << "shWhichLink: " << x.shWhichLink << " ";
        os << "arrchTaskInfo[128]: ";
        os.write(x.arrchTaskInfo, 128 * sizeof(char));
        os << " ";;
        return os;
    }

    friend istream &operator>>(istream &is, TASK_STATUS_STRUCT &x) {
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchTaskSerialNumber, 42);
        is.get();
        plib::skipCountch(is, 32);
        is.read((char *) x.arrchTaskArraySerialNumber, 18);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchJobTaskID, 27);
        is.get();
        plib::skipCountch(is, 16);
        is >> x.shTaskPriority;
        is.get();
        plib::skipCountch(is, 20);
        is >> x.shInexecutingState;
        is.get();
        plib::skipCountch(is, 17);
        is.read((char *) x.arrchScheID, 13);
        is.get();
        plib::skipCountch(is, 18);
        is.read((char *) x.arrchTaskMode, 9);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchDataFileName, 256);
        is.get();
        plib::skipCountch(is, 24);
        is.read((char *) x.arrchSatelliteName, 20);
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchDataSource, 6);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchDataDestination, 6);
        is.get();
        plib::skipCountch(is, 27);
        is.read((char *) x.arrchForeseeStartTime, 20);
        is.get();
        plib::skipCountch(is, 25);
        is.read((char *) x.arrchForeseeEndTime, 20);
        is.get();
        plib::skipCountch(is, 11);
        is >> x.shDestNum;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.iFileExist;
        is.get();
        plib::skipCountch(is, 22);
        is.read((char *) x.arrchFailReason, 128);
        is.get();
        plib::skipCountch(is, 13);
        is >> x.iRoadNumber;
        is.get();
        plib::skipCountch(is, 22);
        for (int i = 0; i < 8; i++) {
            is >> x.arrstruRoadStatus[i];
            is.get();
        }
        is.get();
        plib::skipCountch(is, 12);
        is >> x.shSendDESC;
        is.get();
        plib::skipCountch(is, 13);
        is >> x.shWhichLink;
        is.get();
        plib::skipCountch(is, 20);
        is.read((char *) x.arrchTaskInfo, 128);
        is.get();
        return is;
    }

};


class ArrstruTask {
public:
    int length;
    TASK_STATUS_STRUCT tasks[400];

    ArrstruTask() {
        this->length = 0;
        for (int i = 0; i < 400; i++) {
            bzero(&tasks[i], sizeof(TASK_STATUS_STRUCT));
        }
    }
};

//获取配置文件端口8888
class CSGLStation {
public:
    pstring strName;//站点名称
    pstring strip;//站点ip
    pmap<pstring, pstring> mapconf;//该站点所有配置
    pstring strPathFullConf;//配置文件路径

    ptcp *tcpc;

    CSGLStation() {
        strPathFullConf = "/opt/sc/csgl/SCService/sc.conf";
    }

    CSGLStation(pstring name, pstring strip, pstring pathconf = "/opt/sc/csgl/SCService/sc.conf") {
        this->strName = name;
        this->strip = strip;
        this->strPathFullConf = pathconf;
        this->tcpc = new ptcp(strip, 8888);
    }

    pmap<pstring, pstring> getConfMap() {
        return mapconf;
    }

    void connect() {
        (this->tcpc->connectx());
    }

    bool delConf(pstring key) {
        hlog(key);
        TASK_HEAD head;
        head.type = HTYPEDELCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,sizeof(bool)) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool setConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPESETCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,sizeof(bool)) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    bool addConf(pstring key, pstring val) {
        hlog(key, val);
        TASK_HEAD head;
        head.type = HTYPEADDCONF;
        TASK_ADDCONF struAddConf;
        strcpy(struAddConf.arrchkey, key.c_str());
        strcpy(struAddConf.arrchval, val.c_str());
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return false;
        }
        if (tcpc->send(struAddConf) < 0) {
            plib::sleep(1000);
            return false;
        }
        //接收bool
        bool bRes;
        if (tcpc->recv((char*)&bRes,sizeof(bool)) < 0) {
            plib::sleep(1000);
            return false;
        }
        hlog(bRes);
        return bRes;
    }

    ArrstruTask getTasks() {
        ArrstruTask arrstruTaskRes;
        TASK_HEAD head;
        head.type = HTYPE_GETTASKS;
        //发过去
        if (tcpc->send(head) < 0) {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        int ires = -1;
        if ((ires = tcpc->recv((char*)&arrstruTaskRes,sizeof(arrstruTaskRes))) < 0) {
            plib::sleep(1000);
            return arrstruTaskRes;
        }
        hlog(ires);
        return arrstruTaskRes;
    }

    //获取配置到map中,用tcp形式,需要server配合
    pmap<pstring, pstring> getConfStation() {
        //先清空,不然一直都有
        this->mapconf.clear();
        //获取配置文件各个行,用xsh返回太慢,改成tcp获取
        pstring strContentConf = getConfUseTcp();
//        pstring strContentConf=plib::xsh(this->strip,"cat "+this->strPathFullConf);
//        hlog(strContentConf.contain("\0"));
//        hlog(strContentConf.size(),strContentConf.find("\0"));
//按行分割,顺便去掉空行
        pliststring listConf = strContentConf.split("\n");

//        hlog(listConf);
        //去掉带#的
        pliststring listConfNoComment;
        for (int i = 0; i < listConf.size(); i++) {
            if (!listConf[i].contain("#"))
                listConfNoComment.append(listConf[i]);
        }
//        hlog(listConfNoComment);
        //然后可以逐条解析然后赋值给map了
        for (int i = 0; i < listConfNoComment.size(); i++) {
            pstring strConfi = listConfNoComment[i];//这就是一条配置A=B
            pliststring listConfi = strConfi.split("=");
//            hlog(listConfi);
//要加个判断,因为还有[basic]这样的就一个,要略过,不然崩溃
            if (listConfi.size() == 2) {
                //                mapconf[listConfi[0]] = listConfi[1];
//                hlog(listConfi);
//                mut.lock();
                mapconf.add(listConfi[0], listConfi[1]);
//                mapconf[listConfi[0]] = listConfi[1];
//                mut.unlock();
            }
        }
        return mapconf;
//        hlog(mapconf);
//        pstring strtest=plib::toString(mapconf);
//        hlog(strtest,strtest.size(),strtest.find("\0"),string::npos);
//        hlog(plib::toClass<pmap<pstring,pstring>>(strtest));
    }

    friend ostream &operator<<(ostream &os, CSGLStation x) {
        os << "strName: " << x.strName << " " << "strip: " << x.strip << " " << "mapconf: " << x.mapconf << " "
           << "strPathFullConf: " << x.strPathFullConf << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLStation &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 7);
        is >> x.strip;
        is.get();
        plib::skipCountch(is, 9);
        is >> x.mapconf;
        is.get();
        plib::skipCountch(is, 17);
        is >> x.strPathFullConf;
        is.get();
        return is;
    }

private:

    //要有服务端,超时1秒
    pstring getConfUseTcp() {
        //获取配置文件
        TASK_HEAD head;
        head.type = HTYPEGETCONF;
        head.length = 0;
        if (tcpc->send(head) < 0) {
//            hlog("send head fail");
            plib::sleep(1000);
            return "";
        }
        //等待接收配置文件
//    先接包头
        TASK_HEAD pheadRecv;
        if (tcpc->recv((char *) &pheadRecv, sizeof(TASK_HEAD), 3) < 0) {
            hlog("recv head fail");
//            plib::sleep(1000);
            return "";
        }
//        hlog(pheadRecv.length);
        char *pdataRecv = HMALLOC(char, pheadRecv.length);
        if (tcpc->recv(pdataRecv, pheadRecv.length, 3) < 0) {
//            hlog("recv data fail");
            return "";
        }
        return (pdataRecv);
    }
};

class CSGLSystem {
public:
    pstring strName;    //系统名称
    pmap<pstring, CSGLStation> mapStation;//各个站点
    CSGLSystem() {

    }

    CSGLSystem(pstring name, pmap<pstring, CSGLStation> mapStation) {
        this->strName = name;
        this->mapStation = mapStation;
    }

    CSGLStation getStationByName(pstring strNameStation) {
        return mapStation[strNameStation];
    }

    void connectAllStation() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).connect();
        }
    }

    //当前所有系统站获取配置函数
    void getConfSystem() {
        for (int i = 0; i < mapStation.size(); i++) {
            mapStation.getValue(i).getConfStation();
        }
    }

    pstring getStationNameByIndex(int index) {
        return mapStation.getKey(index);
    }

    CSGLStation &getStationByIndex(int index) {
        return mapStation.getValue(index);
    }

    size_t size() {
        return mapStation.size();
    }

    friend ostream &operator<<(ostream &os, CSGLSystem x) {
        os << "strName: " << x.strName << " " << "mapStation: " << x.mapStation << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLSystem &x) {
        plib::skipCountch(is, 9);
        is >> x.strName;
        is.get();
        plib::skipCountch(is, 12);
        is >> x.mapStation;
        is.get();
        return is;
    }

};

class CSGLAll {
public:
    pmap<pstring, CSGLSystem> mapSystem;//各个系统
    CSGLAll() {
        pmap<pstring, CSGLStation> mapStationGF;
        mapStationGF.add("DTC", CSGLStation("DTC", "172.16.11.3"));
        mapStationGF.add("KSC", CSGLStation("KSC", "172.16.141.5"));
        mapStationGF.add("MYC", CSGLStation("MYC", "172.16.77.6"));
        mapStationGF.add("SYC", CSGLStation("SYC", "172.16.171.5"));
        mapStationGF.add("KR1", CSGLStation("KR1", "130.242.60.241"));
        mapStationGF.add("YD1", CSGLStation("YD1", "172.16.112.2"));
        mapStationGF.add("YE2", CSGLStation("YE2", "172.17.2.2"));
        CSGLSystem systemGF("高分系统", mapStationGF);

//        this->mapSystem.add("高分系统",systemGF);
        pmap<pstring, CSGLStation> mapStationXD;
        mapStationXD.add("YMY", CSGLStation("YMY", "172.16.78.2"));
        mapStationXD.add("YKS", CSGLStation("YKS", "172.16.172.1"));
        mapStationXD.add("YSY", CSGLStation("YSY", "172.16.142.1"));
        mapStationXD.add("DTC", CSGLStation("DTC", "172.16.12.3"));
        CSGLSystem systemXD("先导测试系统", mapStationXD);
        this->mapSystem.add("先导测试系统", systemXD);

        pmap<pstring, CSGLStation> mapStationKJ;
        mapStationKJ.add("DTC", CSGLStation("DTC", "172.16.14.2"));
        mapStationKJ.add("KSC", CSGLStation("KSC", "172.16.144.2"));
        mapStationKJ.add("MYC", CSGLStation("MYC", "172.16.81.2"));
        mapStationKJ.add("SYC", CSGLStation("SYC", "172.16.174.2"));
        mapStationKJ.add("KR1", CSGLStation("KR1", "130.242.60.243"));
        mapStationKJ.add("YD1", CSGLStation("YD1", "172.16.112.1"));
        mapStationKJ.add("DTCZ3", CSGLStation("DTCZ3", "172.16.13.2"));
        mapStationKJ.add("SYCZ3", CSGLStation("SYCZ3", "172.16.173.1"));
        mapStationKJ.add("KSCZ3", CSGLStation("KSCZ3", "172.16.143.1"));
        mapStationKJ.add("MYCZ3", CSGLStation("MYCZ3", "172.16.80.1"));
        mapStationKJ.add("YE2Z3", CSGLStation("YE2Z3", "172.17.2.1"));
        CSGLSystem systemKJ("空基系统", mapStationKJ);

        this->mapSystem.add("空基系统", systemKJ);

    }

    CSGLStation getStation(pstring strNameSystem, pstring strNameStation) {
        return mapSystem[strNameSystem].getStationByName(strNameStation);
    }

    CSGLStation &getStation(pstring strIpStation) {
        for (int i = 0; i < mapSystem.size(); i++) {
            CSGLSystem &systemi = this->getSystemByIndex(i);
            for (int j = 0; j < systemi.size(); j++) {
                CSGLStation &stationj = systemi.getStationByIndex(j);
                if (stationj.strip == strIpStation)
                    return stationj;
            }
        }
    }

    CSGLSystem getSystemByName(pstring strNameSystem) {
        return mapSystem[strNameSystem];
    }

    pstring getSystemNameByIndex(int index) {
        return mapSystem.getKey(index);
    }

    CSGLSystem &getSystemByIndex(int index) {
        return mapSystem.getValue(index);
    }

    size_t size() {
        return mapSystem.size();
    }

    void connectAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).connectAllStation();
        }

    }

    void getConfAll() {
        for (int i = 0; i < mapSystem.size(); i++) {
            mapSystem.getValue(i).getConfSystem();
        }
    }

    friend ostream &operator<<(ostream &os, CSGLAll x) {
        os << "mapSystem: " << x.mapSystem << " ";
        return os;
    }

    friend istream &operator>>(istream &is, CSGLAll &x) {
        plib::skipCountch(is, 11);
        is >> x.mapSystem;
        is.get();
        return is;
    }

};

#pragma pack()
#endif //PLIB_PTCP_H
