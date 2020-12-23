//
// Created by Administrator on 2020/12/21.
//

#include "ptcp.h"

//接收一次后使用，根据使用的结构体转换
void ptcp::setCtcp(ctcp *ct) {
    this->pCtcp = ct;
}

ctcp *ptcp::getCtcp() {
    return this->getCtcp();
}


void ptcp::deleteClientByIndex(int i) {
    this->listclient.deleteByIndex(i);
}


plist<ptcp *> ptcp::getAllClients() {
    return this->listclient;
}

ptcp *ptcp::getPtcpByIp(string ip) {
    for (int i = 0; i < this->getAllClients().size(); i++) {
        ptcp *pti = this->getAllClients()[i];
        if (pti->ip == ip)
            return pti;
    }
    return NULL;
}

const char *ptcp::getData() {
    //    return (char*)(this->data.begin().base());
    return this->data.c_str();

}

pstring ptcp::getDataToString() {
    return this->data;
}


//啥也不用干，因为一般不干
ptcp::ptcp() {

}

void ptcp::init(string ip, int port) {
    this->bIsConnecting = false;
    if(ip=="localhost")
        this->ip="127.0.0.1";
    else
        this->ip = ip;
    this->port = port;
    this->strCtcptype = "init";
}

ptcp::ptcp(string ip, int port) {
    this->init(ip, port);
}

ptcp::ptcp(ctcp *tcpin) {
    this->pCtcp = tcpin;
    this->sock = pCtcp->sock;
    this->init(string(tcpin->ip), tcpin->port);
    //接收的连接过来,在服务端统一认为其是服务端,连接断开表示客户端关闭,就不要再重连了
    if (tcpin->typeSorC == server)
        this->strCtcptype = "server";
    else if (tcpin->typeSorC == client)
        this->strCtcptype = "server";
}

//当socket连接之后获取封装对象，仅用于客户端
ptcp::ptcp(int sock) {
    this->sock = sock;
    this->pCtcp = ctcpInitFromSocket(sock);


    this->init(string(pCtcp->ip), pCtcp->port);

    this->strCtcptype = "client";
}

ptcp::~ptcp() {
    //    close();
}


//默认阻塞3秒超时，一般用于 ftp
int ptcp::connect() {
    this->strCtcptype = "client";
    //    tcp=ctcpInit(this->ip.c_str(),this->port,client);//,c中已增加了，阻止SIGPIPE信号，不会在对方断开后发送数据导致崩溃
    //    this->sock=tcp->sock;
    //    this->typeSorC=client;
    //    return ctcpConnect(tcp);


    string strRealIP;
    //通过ip中是否有字母来判断是否是域名
    if (plib::isContaiAlpha(this->ip))
        strRealIP = plib::hostToIp(this->ip.c_str());
    else
        strRealIP = this->ip;

    this->ip = strRealIP;

    pCtcp = ctcpInit(this->ip.c_str(), this->port);//,c中已增加了，阻止SIGPIPE信号，不会在对方断开后发送数据导致崩溃
    //    this->typeSORC=client;

//    HLOG("cbefore");
    int ires = ctcpConnect(pCtcp);
//    HLOG("cafter");
    this->sock = pCtcp->sock;
    return ires;
}

int ptcp::connect2() {
    if (connect() < 0) {
        return -1;
    }
    hlog("连接固定域名成功");
    if (sendx("getIpWaiwang") < 0) {
        return -2;
    }
    if (recvx() < 0) {
        return -3;
    }
    pstring strhostww = getClass<pstring>();
    hlog(strhostww);
    close();
    this->init(strhostww, 1234);
    if (connect() < 0) {
        return -4;
    }
    hlog("连接外网ip成功");
    return 0;
}

//使用shutdown，多线程使用socket也都会关闭，如果用close不行
int ptcp::close() {
    //    return this->tcp->destroy(this->tcp);
    hlog(pstring() << "关闭TCP连接: " << this->ip + ":" << this->port);
    return ctcpDestroy(this->pCtcp);
}


typedef struct STRUCT_PARA {
    int itype;//server类型,是int类型还是string 0标识string，1标识int,2是nopack
    ptcp *ptcpClient;
    ptcp *ptcpOriginal;

    void (*thread_user)(ptcp *para);
} STRUCT_PARA;


void *thread_processx(void *para) {
    pthread_detach(pthread_self());
    STRUCT_PARA *pmix = (STRUCT_PARA *) para;
    //    hlog(pmix->itype);

    hlog(pstring() << "####### 新增了客户端 " << pmix->ptcpClient->ip << " 线程id:" << (plib::getTid()) << ",当前客户端个数:"
                   << pmix->ptcpOriginal->getAllClients().size() << " #########");
    //    hlog(*(pmix->ptcpOriginal));
    //    hlog(pmix->ptcpOriginal->listclient.size());

    //没有封装int类型或者string类型，纯发送接收，适合有固定接口的对外项目
    if (pmix->itype == 2) {
        pmix->thread_user(pmix->ptcpClient);
    } else {
        while (true) {//执行子函数
            if (pmix->itype == 0) {
                if (pmix->ptcpClient->recvx() < 0)
                    break;
                pmix->thread_user(pmix->ptcpClient);
            }
        }
    }

    string strIpExit;
    //删除列表中的客户端
    for (int i = 0; i < pmix->ptcpOriginal->listclient.size(); i++) {
        //找到然后删除
        ptcp *pti = pmix->ptcpOriginal->listclient[i];
        if (pti->sock == pmix->ptcpClient->sock) {
            strIpExit = pti->ip;
            mut.lock();
            //如果提取出来再删除就没用，必须得内部删除，一定要注意
            pmix->ptcpOriginal->deleteClientByIndex(i);
            mut.unlock();
            break;
        }
    }
    hlog(pstring() << "####### 客户端 " << strIpExit << " 退出，线程id:" << (plib::getTid()) << ",当前客户端个数:"
                   << pmix->ptcpOriginal->getAllClients().size() << " #########");
    //关闭这个socket
    pmix->ptcpClient->close();
    delete (pmix->ptcpClient);
    delete (pmix);


    return NULL;
}

//已经集成了线程自动释放，tcp关闭连接，其它资源自动释放等，后两个在netmanage线程中
int ptcp::setThreadServer(void (*fun_thread)(ptcp *para), int itype) {
    //检查一下,server要绑定具体地址,不能是127.0.0.1这种
    if(this->ip=="127.0.0.1")
    {
        hlog("tcp作为server端要绑定具体地址!");
        exit(0);
    }
    pCtcp = ctcpInit(this->ip.c_str(), this->port);//,c中已增加了，阻止SIGPIPE信号，不会在对方断开后发送数据导致崩溃
    int iSock = ctcp_InitTcpSocket();
    if (iSock < 0) {
        HLOG_STRING_STRINGA("Init tcp socket failed:", strerror(errno));
        return -1;
    }
    int ibind = ctcp_BindSocket(iSock, pCtcp->ip, pCtcp->port);
    if (ibind < 0) {
        HLOG_STRING_STRINGA("bind tcp socket failed:", strerror(errno));
        return -2;
    }
    pCtcp->sock = iSock;
    this->sock = pCtcp->sock;
    //    this->typeSORC=server;
    this->strCtcptype = "server";

    int count = 0;
    //来一个连接创建一个线程
    while (true) {
        //阻塞，等到有一个连接来
        ctcp *xtcpClient = ctcpAccept(pCtcp);
        //        HLOG_XP(xtcpClient);
        if (xtcpClient == NULL) {
            //accept在关闭程序的时候会返回
            hlog(strerror(errno));
            break;
        }
        //        hlog(xtcpClient->ip);
        //这个地方必须要new，不然传参之后会被释放，覆盖,在线程中释放去
        STRUCT_PARA *ppara = new STRUCT_PARA;
        bzero(ppara, sizeof(STRUCT_PARA));
        ppara->ptcpClient = new ptcp(xtcpClient);
        //这样接收的连接我再服务端认为它是server,不是client
        ppara->thread_user = fun_thread;
        ppara->itype = itype;

        mut.lock();
        listclient.append((ppara->ptcpClient));
        mut.unlock();
        //        hlog(*this);

        //把this指针传进去
        ppara->ptcpOriginal = this;

        pthread_t tid_t;
        pthread_create(&tid_t, NULL, thread_processx, ppara);


        //        //进行内存检测时加上这几句，表示来两个连接之后就退出不再等待accpet了
        //        count++;
        //        if(count==2)
        //        {
        //            hlog("enough");
        //            break;
        //        }
    }

    sleep(5);
    ctcpDestroy(pCtcp);
}

typedef struct STRUCT_AUXTCP_PARA {
    void (*thread_user)(class ptcp *para);

    int itype;
    ptcp *pOriginal;
} STRUCT_AUXTCP_PARA;

void *threadTcpServerNoDetach(void *para) {
    pthread_detach(pthread_self());
    STRUCT_AUXTCP_PARA *psp = (STRUCT_AUXTCP_PARA *) para;
    //    hlog(psp->itype);
    //    hlog(psp->pOriginal->ip);
    psp->pOriginal->setThreadServer(psp->thread_user, psp->itype);
    delete psp;
    return NULL;
}

int ptcp::setThreadServerDetach(void (*fun_thread)(ptcp *), int itype) {
    //    this->typeSORC=server;
    this->strCtcptype = "server";
    pthread_t tidServerTest;
    STRUCT_AUXTCP_PARA *psp = new STRUCT_AUXTCP_PARA;
    psp->itype = itype;
    psp->thread_user = fun_thread;
    psp->pOriginal = this;

    //要在这里设置type，不然进了线程没法控制，set之后打印啥都没有
    pthread_create(&tidServerTest, NULL, threadTcpServerNoDetach, psp);
}


bool ptcp::operator<(const ptcp &tcp) const {
    return this->sock < tcp.sock;
}

//不收到指定字节就退出，收多少返回多少，适用于转发,若收20，发46，则收三次20，20，6，若收512k发46则收46
//=如果实际接收包的长度小于0，说明接收时出现了错误，包括对方关闭了连接
int ptcp::recvNoFullPack(char *arrecv, int ilenWantRecv, int timeout) {
    if (arrecv == NULL)
        return -4;
    if (ilenWantRecv <= 0)
        return -5;
    if (timeout > 0) {
        struct timeval timeout = {3, 0};//秒和微妙
        setsockopt(this->sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(struct timeval));
    }
    int iRes = ctcp_RecvPacket(this->sock, arrecv, ilenWantRecv);
    if (iRes == 0) {
        //        hlog(pstring()<<this->ip<<"对方关闭了连接");
        return -1;
    }
}


int ptcp::reconnect(int iretrycount, int itimesecond) {
    //先判断是否在重连,如果在重连,说明别的线程在连,就不重连了
    if (this->bIsConnecting) {
        hlog(pstring() << "已经有线程在重连" << this->ip << ":" << this->port << ",无需重连");
        return -78;
    }
    //如果没有再重连,则置为true标示我在重连
    mut.lock();;
    this->bIsConnecting = true;
    mut.unlock();;
    int ires = ctcpReconnect(pCtcp, iretrycount, itimesecond);

    mut.lock();;
    this->bIsConnecting = false;
    mut.unlock();;
    return ires;
}

ptcp::ptcp(int iSockAccept, string ip, int port) {
    this->sock=iSockAccept;
    this->init(ip,port);
    this->strCtcptype="server";
}