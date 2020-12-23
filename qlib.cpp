#include "qlib.h"
QString strFullPathLog("xlfd.log");
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString level;
    switch (type) {
    case QtDebugMsg:
        level = "Debug";
        break;
//    case QtInfoMsg:
//        level = "Info ";
//        break;
    case QtWarningMsg:
        level = "Warn ";
        break;
    case QtCriticalMsg:
        level = "Error";
        break;
    case QtFatalMsg:
        level = "Fatal";
        break;
    default:;
    }


    // 输出到日志文件, 格式: 时间 - [Level] (文件名:行数, 函数): 消息
    QString fileName = context.file;
    int index = fileName.lastIndexOf(QDir::separator());
    fileName = fileName.mid(index + 1);


    QString strlog=QString("[%1]-[tid: %2]-[%3:%4]: %5")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"))
               .arg(plib::getTid())
               .arg(fileName)
               .arg(context.line)
               .arg(msg);


    // 输出到标准输出
    QByteArray localMsg = strlog.toLocal8Bit();
    std::cout << std::string(localMsg)<<std::endl;

    //输出到文件
    QString strToday=QString("%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

    //处理过的,主要针对xlfd.log这种不带全路径的
    QString strFullPathLogProcess=strFullPathLog;
    //如果路径中不带/或者\\,则就是上述这种的,自己加上
    if(!strFullPathLog.contains("\\")&&!strFullPathLog.contains("/"))
    {
        strFullPathLogProcess="./"+strFullPathLogProcess;
    }
    //    hlog(strFullPathLogProcess.toStdString());
    //解析路径,文件名用的
    QFileInfo fileInfo(strFullPathLogProcess);
    //    hlog(strFullPathLogProcess.toStdString());
    //    hlog(fileInfo.path().toStdString());
    //先判断有没有这个目录,没有的话先递归创建
    QDir mDir2;
    if(!mDir2.exists(fileInfo.path()))
    {
        mDir2.mkpath(fileInfo.path());
    }
    // 这个地方根据linux和windows的不同要改
    QFile file(fileInfo.path()+"/"+fileInfo.baseName()+strToday+"."+fileInfo.suffix());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    //保存为utf8格式,编码也是这个格式,否则会乱码
    text_stream.setCodec("UTF-8");
    text_stream << strlog << "\n";
    file.flush();
    file.close();

    mutex.unlock();
}
//为了使qDebug支持打印string或其他结构体,加这个
QDebug operator<<(QDebug debug, const std::string &s)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << s.c_str();

    return debug;
}
qlib::qlib(QObject *parent) : QObject(parent)
{

}



QString qlib::toQStringHex(char ch)
{
    QString str = QString("%1").arg(ch&0xFF, 2, 16, QLatin1Char('0'));
    return str;
}



void qlib::messageBox(QString strContent,QString strTitle)
{
    QMessageBox qMsgBox(QMessageBox::Information,strTitle,strContent);
    qMsgBox.setStandardButtons(QMessageBox::Ok);
    qMsgBox.setButtonText(QMessageBox::Ok,("确定"));
    qMsgBox.exec();
}

QString qlib::getIPLocal()
{
    string ip=plib::getIPLocal();
    return QString::fromStdString(ip);
}


QString qlib::saveDialogFile(QString qstrPathDefault)
{
    QFileDialog *fd=new QFileDialog(NULL,QString::fromLocal8Bit("保存文件"),qstrPathDefault,"zip(*.zip *.zip)");
    fd->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式

    if(fd->exec()==QFileDialog::Accepted) // ok
    {
        QString file=fd->selectedFiles()[0];
        qDebug()<<file;
        return file;
    }
    else
        return NULL;
}
/*
 * 打开对话框浏览文件，返回路径，若取消了返回NULL
 */
QString qlib::openDialogFile(QString qstrPathDefault)
{
//    QFileDialog *fd=new QFileDialog(NULL,QString::fromLocal8Bit("打开文件"),qstrPathDefault,"zip(*.zip *.zip)");
    QFileDialog *fd=new QFileDialog(NULL,QString::fromLocal8Bit("打开文件"),qstrPathDefault,"*(*.* *.*)");
//     fd->setFilter(QString("zip(*.zip *.zip)"));
    if(fd->exec()==QFileDialog::Accepted) // ok
    {
        QString file=fd->selectedFiles()[0];
        qDebug()<<file;
        return file;
    }
    else
        return NULL;
}
//打开对话框多选并返回所选文件的所有路
QStringList qlib::openDialogFiles(QString qstrPathDefault)
{
    QStringList files;
    QFileDialog *fd=new QFileDialog(NULL,"打开文件",qstrPathDefault);
    //多
    fd->setFileMode(QFileDialog::ExistingFiles);
    if(fd->exec()==QFileDialog::Accepted) // ok
    {
        files=fd->selectedFiles();
        qDebug()<<files;
        return files;
    }
    else
        return files;
}

QString qlib::openDialogSelectDir(QString qstrPathDefault)
{
    QFileDialog* openFilePath = new QFileDialog(NULL, "select dirs",qstrPathDefault); //打开一个目录选择对话
    openFilePath-> setFileMode( QFileDialog::DirectoryOnly );
    QString filepath= openFilePath->getExistingDirectory();
    //如果最后不是斜杠,要加上
    if(filepath[filepath.length()-1]!='\/')
    {
        filepath.append('\/');
    }
    qDebug()<<filepath;
    openFilePath->deleteLater();
    return filepath;
}


bool qlib::inputbox(QString &strInput, QString strdefault)
{
    bool isOK;
    strInput = QInputDialog::getText(NULL, QString::fromLocal8Bit("文本输入框"),
                                                       QString::fromLocal8Bit("请输入文本"),
                                                       QLineEdit::Normal,
                                                       strdefault,
                                                       &isOK);
//    if(isOK) {QMessageBox::information(NULL, QString::fromLocal8Bit("信息提示框"),
//                                               QString::fromLocal8Bit("您输入的文本是: <b>") + strInput + "</b>",
//                                               QMessageBox::Yes | QMessageBox::No,
//                                               QMessageBox::Yes);
//        return true;
//    }
    if(isOK)
        return true;
    else
        return false;
}

bool qlib::confirmbox(QString strcontent)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(NULL,
                                  QString::fromLocal8Bit("确认信息"),
                                        strcontent,
                                        QMessageBox::Yes | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
        return true;
    else if (reply == QMessageBox::Cancel)
        return false;
}


//手机设备暂不支持qprocess
void qlib::loginSsh(QString program, QString ip, QString user, QString pwd, QString port)
{
//    QStringList arguments;
//    arguments<< "-pw" << pwd<< QString("%1@%2").arg(user).arg(ip) << port;
//    QProcess *process = new QProcess();
//    process->setProcessChannelMode(QProcess::SeparateChannels);
//    process->setReadChannel(QProcess::StandardOutput);
//    process->start(program, arguments, QIODevice::ReadWrite);
}
qlibRand::qlibRand()
{
    qsrand(QTime::currentTime().msec());
}

int qlibRand::rand(int s, int e)
{
    return s+qrand()%(e-s);
}


btcp::btcp()
{

}

btcp::btcp(ctcp *tcpin)
{
//    typeSorC=tcpin->typeSorC;
//    tcp=tcpin;
//    this->setTypeServerOrClient(tcpin->typeSorC);
    this->setCtcp(tcpin);
    this->ip=string(tcpin->ip);
    this->port=tcpin->port;
    this->sock=this->getCtcp()->sock;
}

btcp::btcp(string ip, int port)
{
    this->ip=ip;
    this->port=port;
}

btcp::btcp(QString ip, int port)
{
    this->ip=ip.toStdString();
    this->port=port;
}


btcp::btcp(int iSockAccept, string ip, int port)
{
    this->sock=iSockAccept;
    this->ip=ip;
    this->port=port;
}
typedef struct STRUCT_PARA
{
    btcp* ptcpAll;
    btcp* ptcpClient;
    void (*thread_user)(btcp* para);
}STRUCT_PARA;
//static void* thread_process(void* para)
void*  thread_processxb(void* para)
{
//    hlog("in thread_process");
    STRUCT_PARA* pmix=(STRUCT_PARA*)para;

    while(true)
    {
        //执行子函数
        if(pmix->ptcpClient->recvb()>0)
        {
            pmix->thread_user(pmix->ptcpClient);
        }
        else
        {
            //            hlog("客户端断开，接收失败");
            break;
        }
    }
    //查找
//    hlog(pmix->ptcpClient->sock);
//    for(pvector<ptcp*>::iterator it=pmix->ptcpAll->plistClient.begin();it!=pmix->ptcpAll->plistClient.end();it++)
//    {
//        ptcp* tcptmp=*it;
//        hlog(tcptmp->sock);
//        hlog(pmix->ptcpClient->sock);
//        if(pmix->ptcpClient->sock==(tcptmp->sock))
//        {
//            //            hlog("before close");
//            pmix->ptcpClient->close();
//            //            hlog("before delete");
//            delete pmix->ptcpClient;
//            //            hlog("before erase");
//            pmix->ptcpAll->plistClient.erase(it);

//            //打印当前
//            hlog("#######delete a client,now all clients:");
//            hlog(pmix->ptcpAll->plistClient.size());
//            for(int i=0;i<pmix->ptcpAll->plistClient.size();i++)
//            {
//                hlog(*(pmix->ptcpAll->plistClient[i]));
//            }
//            delete pmix;
//            break;
//        }
//    }
    return NULL;
}
//已经集成了线程自动释放，tcp关闭连接，其它资源自动释放等，后两个在netmanage线程中
int btcp::setThreadServerb(void (*fun_thread)(btcp* para))
{
//    tcp=ctcpInit(this->ip.c_str(),this->port,server);//,c中已增加了，阻止SIGPIPE信号，不会在对方断开后发送数据导致崩溃
    this->setCtcp(ctcpInit(this->ip.c_str(),this->port));


    int iSock=ctcp_InitTcpSocket();
    if(iSock<0)
    {
        HLOG_STRING_STRINGA("Init tcp socket failed:",strerror(errno));
        return -1;
    }
    int ibind=ctcp_BindSocket(iSock,this->getCtcp()->ip,this->getCtcp()->port);
    if(ibind<0)
    {
        HLOG_STRING_STRINGA("bind tcp socket failed:",strerror(errno));
        return -2;
    }
//    tcp->sock=iSock;
    this->getCtcp()->sock=iSock;
//    this->sock=tcp->sock;
    this->sock=this->getCtcp()->sock;
//    this->typeSorC=server;
//    this->setTypeServerOrClient(server);

    //不停等待，来一个，创建一个线程
    while(true)
    {
        ctcp* xtcpClient=ctcpAccept(this->getCtcp());
        //        HLOG_XP(xtcpClient);
        if(xtcpClient==NULL)
        {
            HLOG("accept error:%s\n",strerror(errno));
            continue;
        }

        //这个地方必须要new，不然传参之后会被释放，覆盖,在线程中释放去
        STRUCT_PARA *ppara=new STRUCT_PARA;
        bzero(ppara,sizeof(STRUCT_PARA));
        ppara->ptcpClient=new btcp(xtcpClient);
        ppara->ptcpAll=this;
        ppara->thread_user=fun_thread;


        pthread_t tid_t;
        pthread_create(&tid_t,NULL,thread_processxb,ppara);
    }
}

