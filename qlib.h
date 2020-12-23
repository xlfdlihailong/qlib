#ifndef QLIB_H
#define QLIB_H

#include <QObject>
#include <QDebug>
#include <QTime>

#include <QProcess>

#include <QThread>
#include "plib/plib.h"
#include <QTextCodec>
#include <QSettings>
#include <QMutex>
#include <QDir>
#include <QPoint>
#include <QDataStream>
#include <QDebug>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QMap>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QCoreApplication>
#include <QMap>
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
//使用前需要在主函数中调用qInstallMessageHandler(outputMessage); //注册MsgHandler回调函数
extern QString strFullPathLog;//若路径不存在则会自动递归创建
//#define qlog(var) qDebug().noquote()<<#var<<":"<<var;

//为了处理直接打印字符串的尴尬
#define qlog(var){\
    QString strtype=typeid(var).name();\
    if(strtype.contains("_c"))\
    qDebug().noquote()<<var;\
    else\
    qDebug().noquote()<<#var<<":"<<var;\
    }\


//自定义输出日志--控制台和文件都输出,文件以日期为单位
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);


//为了使qDebug支持打印string或其他结构体,加这个
QDebug operator<<(QDebug debug, const std::string &s);
static int qpHLOGFunp(const char *file, int line,string str)//配套重载打印函数时不要加&
{
    int iFd;                              /*文件描述符*/
    int iFd1;
    int len=str.size()+1024;
    char *arrchBuf=(char*)malloc(len);
    memset(arrchBuf,0,len);
    char     *pchIndex;                        /*当前位置*/
    struct tm struTm;
    time_t   tmNow;                            /*当前时间*/
    char         pcHLOGPath[1024 + 1];    /*日志路径*/
    strcpy(pcHLOGPath,strPathLog.c_str());
    iFd1 = STDOUT_FILENO;
    pchIndex = strrchr(pcHLOGPath, '.');
    //根据当前日期写入日志文件
    if (pchIndex == NULL)
    {
        pchIndex = pcHLOGPath + strlen(pcHLOGPath);
    }
    memcpy(arrchBuf, pcHLOGPath, pchIndex - pcHLOGPath);
    time(&tmNow);
    memset(&struTm,0,sizeof(struct tm));
    localtime_r(&tmNow,&struTm);//线程安全需要，否则多线程不行
    strftime(arrchBuf + strlen(arrchBuf), 20, "%Y-%m-%d", &struTm);
    sprintf(arrchBuf + strlen(arrchBuf), "%s", pchIndex);
    //打开自定义的文件,如果文件打开失败，则退出
    if ((iFd = open(arrchBuf, O_RDWR | O_APPEND | O_CREAT, 0666)) == -1)
    {
        free(arrchBuf);
        arrchBuf=NULL;
        return -1;
    }
    //定义信息说明的格式，并写入缓存
    memset(arrchBuf, 0, sizeof(arrchBuf));
    time(&tmNow);
    memset(&struTm,0,sizeof(struct tm));
    localtime_r(&tmNow,&struTm);//线程安全
    strftime(arrchBuf , 20, "[%T", &struTm);
    struct timeval tvt;
    gettimeofday(&tvt,NULL);
    sprintf(arrchBuf+strlen(arrchBuf),".%03d",(int)((double)tvt.tv_usec/1000));
    string str2(file);
    string pattern="/";
    string::size_type pos;
    vector<string> result;
    str2+=pattern;
    string::size_type size=str2.size();
    for(string::size_type i=0;i<size;i++)
    {
        pos=str2.find(pattern,i);
        if(pos<size)
        {
            string s=str2.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    const char* fileOnly=result[result.size()-1].c_str();
    sprintf(arrchBuf + strlen(arrchBuf), "]-[tid:%5ld]-[%s:%d]", syscall(__NR_gettid),fileOnly,line);
    sprintf(arrchBuf,"%-20s",arrchBuf);
    //    不用原来的方式，因为发现中间夹杂\0时会导致显示不全
    //    //如果信息说明格式未写入成功，则直接写入参数列表，否则按照信息说明格式写入
    //    if (strlen(arrchBuf) == 0)
    //        sprintf(arrchBuf, "%s\n", str.c_str());
    //    else
    //        sprintf(arrchBuf + strlen(arrchBuf), " %s\n", str.c_str());
    //    write(iFd, arrchBuf, strlen(arrchBuf));
    //    write(iFd1, arrchBuf, strlen(arrchBuf));
    str+='\n';
    int lenRes=strlen(arrchBuf)+str.size();
    memcpy(arrchBuf+strlen(arrchBuf),str.c_str(),str.size());
    write(iFd, arrchBuf, lenRes);
    write(iFd1, arrchBuf, lenRes);

    close(iFd);
    free(arrchBuf);
    arrchBuf=NULL;
    return 0;
}

template<class T>
static QString QCommon_toString(const T & t)
{
    QBuffer bug;
    bug.open(QIODevice::ReadWrite);
    QDebug * deb = new QDebug(&bug);
    (*deb)<<t;
    delete deb;
    QString str(bug.data());
    //不知道为何qDebug会增加一个字符,强行去掉
    return str.mid(0,str.size()-1);
}
template<typename T>
void qprintarg(ostream& os,T a){
    //隔开，防止打印字符串时乱码不能分割，与序列化一个原理
    //但是不能用跟序列化一样的字符1，不然会导致序列化的混乱
    //    if(typeid(a).name()==typeid(char).name())
    //        hlogone("char");
    //    hlogone(typeid(a).name());
    //    hlogone(typeid(char).name());
    //    string str=(typeid(a).name());
    QString qstra=QCommon_toString(a);
    //    hlog(qstra,qstra.size(),qstra[qstra.size()-1]=='\0');
    string stres=qstra.toStdString();
    //    if(str.find("pstring")!=string::npos)
    //    {
    //        ostringstream oss;

    //        oss<<stres;
    //        string strtmp=oss.str();
    //        string strlast=strtmp.substr(0,strtmp.size()-1);
    //        os<<strlast<<"\2";
    //    }
    //    else
    //        os<<stres<<"\2";
    os<<stres<<"\2";
}

template<class...Args>
void qexpand(Args... args)
{
    ostringstream os;
    int arr[] = {(qprintarg(os,args), 0)...};
    //或者改进为 std::initializer_list<int>{(printarg(args), 0)...};lumda
    string stres=os.str();
    //        hlogone(stres);
    //这个地方不能用多个字符串分割的，不然结果是空字符串的时候会自动去掉
    vector<string> ls=common_split_one(stres,string("\2"));
    ls.erase(ls.end()-1);
    //        for(size_t i=0;i<ls.size();i++)
    //        {
    //            hlog(ls[i],ls[i].size(),i);
    //        }
    const char* file=ls[0].c_str();
    int line=atoi(ls[1].c_str());

    //这个一定是实际参数个数，
    vector<string> listValues;
    //    hlogone(ls.size());
    for(size_t i=3;i<ls.size();i++)
    {
        listValues.push_back(ls[i]);
    }



    //真实参数个数
    //    hlogone(listValues.size());
    //最终结果
    vector<string> listNames;
    //如果真是参数个数是1，则不用分了
    if(listValues.size()==1)
    {
        listNames.push_back(ls[2]);

    }
    else//否则要根据逗号分
    {
        //先按逗号分，后来再整理ls[2]是传进来的参数名字，带逗号的
        vector<string> listNamesOrigin=common_split(ls[2],",");
        //                hlog(listNamesOrigin.size());

        //如果name数正好等于value数，那就正好了，不然就要考虑其他情况
        if(listNamesOrigin.size()==listValues.size())
        {
            for(size_t i=0;i<listNamesOrigin.size();i++)
            {
                listNames.push_back(listNamesOrigin[i]);
            }
        }
        else
        {
            for(size_t i=0;i<listNamesOrigin.size();i++)
            {
                string stri=listNamesOrigin[i];
                int il=0;
                int ir=0;
                int ishuangyinhao=0;
                //算（-）的个数
                for(int q=0;q<stri.size();q++)
                {
                    if(stri[q]=='(')
                        il++;
                    if(stri[q]==')')
                        ir++;
                    if(stri[q]=='\"')
                        ishuangyinhao++;
                }
                int icha=il-ir;
                //        hlogone(icha);

                //如果这一项不包扩括号和引号，则就是变量
                if(icha==0&&ishuangyinhao==0)
                {
                    listNames.push_back(stri);
                    //            hlogone(stri);
                }//优先级是先处理引号
                else if(ishuangyinhao>0)
                {
                    string strjoin=stri;
                    //            hlogone(stri);
                    //从这个开始往后数
                    for(int j=i+1;j<listNamesOrigin.size();j++)
                    {
                        string strj=listNamesOrigin[j];
                        strjoin+=","+strj;
                        for(int p=0;p<strj.size();p++)
                        {
                            if(strj[p]=='"')
                            {
                                ishuangyinhao--;
                                //                        hlogone(ishuangyinhao);
                            }
                        }
                        //为了防止互相干扰
                        if(((ishuangyinhao==0)))
                        {
                            //把i指针移到这
                            i=j;
                            break;
                        }
                    }

                    //            hlogone(strjoin);
                    listNames.push_back(strjoin);
                }
                //只包含（但不包含），则要跟下一个结合，遇到（增加，遇到）减少，直到为0为止算一个
                else if(icha>0)
                {
                    string strjoin=stri;
                    //从这个开始往后数
                    for(int j=i+1;j<listNamesOrigin.size();j++)
                    {
                        string strj=listNamesOrigin[j];
                        strjoin+=","+strj;
                        for(int p=0;p<strj.size();p++)
                        {
                            if(strj[p]=='(')
                                il++;
                            if(strj[p]==')')
                                ir++;
                        }
                        //为了防止互相干扰
                        if(((il-ir==0)))
                        {
                            //把i指针移到这
                            i=j;
                            break;
                        }
                    }
                    listNames.push_back(strjoin);
                }
            }
        }
    }
    //打印
    //    hlogone(listNames.size());
    //    for(size_t i=0;i<listNames.size();i++)
    //    {
    //        hlogone(listNames[i]);
    //    }
    //    hlogone(listValues.size());
    //    for(size_t i=0;i<listValues.size();i++)
    //    {
    //        hlogone(listValues[i]);
    //    }
    //整理了一遍之后，如果发现

    string strval;
    for(size_t i=0;i<listNames.size();i++)
    {
        //在此判断是否是单独字符串,或者是pstring()<<这种的行为
        //        hlog(listNames[i]);
        //        string stri=listNames[i];
        //        hlog(stri.size());
        //        hlog(stri[0]=='"',stri[stri.size()-1]=='"');
        //        hlog(stri[stri.size()-1]=='"');
        //                hlogone(listValues[i]);
        //如果名字第一个字符和最后一个是",则说明是字符串,直接打印value,因为有可能会有这种的xfdsf",,wfew,
        if((listNames[i][0]=='"'&&listNames[i][listNames[i].size()-1]=='"')||
                (listNames[i].find("()<<")!=string::npos))
        {
            strval+="["+listValues[i]+"] ";
        }
        else
        {
            strval+="["+listNames[i]+" : "+listValues[i]+"] ";
        }
    }

    strval=strval.substr(0,strval.size()-1);
    //    hlogone(strval);
    qpHLOGFunp(file,line,strval);
}

//支持多参数不同类型的联合打印
//支持函数嵌套带逗号，已处理
//hlog(itest,funtest(funtest2(funtest3(2),2),3,funtest2(6,8)),db,funtest2(2,3),funtest3(3));
#define qlog(...) qexpand(__FILE__,__LINE__,#__VA_ARGS__,__VA_ARGS__)

class qlib : public QObject
{
    Q_OBJECT
public:
    explicit qlib(QObject *parent = 0);
    static QString getCopyInClipboard()
    {
        QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
        QString originalText = clipboard->text();         //获取剪贴板上文本信息
        return originalText;
    }
    static void setCopyInClipboard(QString info)
    {
        QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
        clipboard->setText(info);
    }
    //中文无乱码的读取写入
    static QString getConfString(QString joint,QString key,QString path="xlfd.conf")
    {
        QSettings *configIniWrite = new QSettings(path, QSettings::IniFormat);
        configIniWrite->setIniCodec(QTextCodec::codecForName("utf8")); //在此添加设置，即可读写ini文件中的中文
        //向ini文件中写入内容,setValue函数的两个参数是键值对
        //向ini文件的第一个节写入内容,ip节下的第一个参数
        QString strInfo="/"+joint+"/"+key;
        //        configIniWrite->setValue("/ip/first", "192.168.0.1");
        //        value=QString(plib::toUTF8(value.toStdString().c_str()).c_str());

        QString stread=configIniWrite->value(strInfo).toString();
        qlog(stread);

        //写入完成后删除指针
        delete configIniWrite;
        return stread;
    }
    //中文无乱码的读取写入
    static void setConfString(QString joint,QString key,QString value,QString path="xlfd.conf")
    {
        QSettings *configIniWrite = new QSettings(path, QSettings::IniFormat);
        configIniWrite->setIniCodec(QTextCodec::codecForName("GB2312")); //在此添加设置，即可读写ini文件中的中文
        //向ini文件中写入内容,setValue函数的两个参数是键值对
        //向ini文件的第一个节写入内容,ip节下的第一个参数
        QString strInfo="/"+joint+"/"+key;
        //        configIniWrite->setValue("/ip/first", "192.168.0.1");
        //        value=QString(plib::toUTF8(value.toStdString().c_str()).c_str());
        configIniWrite->setValue(strInfo,value);
        //写入完成后删除指针
        delete configIniWrite;
    }
    static void initlog(QString strLogFullPath="xlfd.log")
    {
        plib::initLog(strLogFullPath.toStdString().c_str());
        strFullPathLog=strLogFullPath;
        qInstallMessageHandler(outputMessage);
        qRegisterMetaType<pstring>("pstring");
        qRegisterMetaType<QVector<int> >("QVector<int>");
    }
    //正常":/qss/flatwhite.css"
    //名字是flatwhite.css
    static void loadQss(QApplication* pa,QString name)
    {
        QString strpath=":/qss/"+name;
        qDebug()<<strpath;
        //加载样式表
        QFile file(":/qss/"+name);

        if (file.open(QFile::ReadOnly)) {
            qDebug("open success");
            QString qss = QLatin1String(file.readAll());
            QString paletteColor = qss.mid(20, 7);
            pa->setPalette(QPalette(QColor(paletteColor)));
            pa->setStyleSheet(qss);
            file.close();
        }
        else {
            qDebug("open fail");
        }
    }
    template <class T>
    static QByteArray toByteArray(T t)
    {
        //        QPoint pos(5, 7);
        //        qDebug()<<pos;
        //        QPoint tmp;
        //        qDebug()<<tmp;
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::ReadWrite);   //使用QDataStream将Qpoint序列化进QByteArray内
        out.setVersion(QDataStream::Qt_4_8);                //设置Qt串行化版本 使用Qt5.8版本流
        out<<t;
        //        qDebug()<<datagram.length();
        return datagram;
    }
    template <class T>
    static T toClass(QByteArray datagram)
    {
        T t;
        QDataStream dsIn(&datagram,QIODevice::ReadWrite);  //使用QDataStream将QByteArray反序列化
        dsIn.setVersion(QDataStream::Qt_4_8);             //设置Qt串行化版本 使用Qt5.8版本流
        dsIn>>t;
        //        qDebug()<<tmp;
        return t;
    }
    //任意类型到字符串的转换
    template<class T>
    static QString toString(const T & t)
    {
        pstring strtype=(typeid(T).name());
//        hlog(strtype);
        //如果是c字符串char*
        if(strtype.contain("char const ["))
        {
//            hlog("this is const char array");
            return QString((char*)(&t));
        }
        //如果是c++字符串string或者pstring
        if(typeid(T)==typeid(pstring)||typeid(T)==typeid(string))
        {
//            hlog("this is c++ string");
            //            pstring strcpp=*((pstring*)(&t));
            return QString::fromStdString(*((string*)(&t)));
        }
        if(typeid(T)==typeid(QString))
        {
//            hlog("this is QString string");
            return *(QString*)(&t);
        }

        QBuffer bug;
        bug.open(QIODevice::ReadWrite);
        QDebug * deb = new QDebug(&bug);
        (*deb)<<t;
        delete deb;
        QString str(bug.data());
        //不知道为何qDebug会增加一个字符,强行去掉
        QString stres= str.mid(0,str.size()-1);
        return stres;
        //linux不需要，直接自己解决了，测试过
        //        return unicodeToUtf8(stres);
    }
    //unicode to utf8\u53d1这种,linux不需要，因为直接自己解决了
    //      static QString unicodeToUtf8(QString Str)
    //      {
    //          QString filename = Str;
    //          do {
    //              int idx = filename.indexOf("\\u");
    //              QString strHex = filename.mid(idx, 6);
    //              strHex = strHex.replace("\\u", QString());
    //              int nHex = strHex.toInt(0, 16);
    //              filename.replace(idx, 6, QChar(nHex));
    //          } while (filename.indexOf("\\u") != -1);
    //          return filename;
    //      }
    //登陆到ssh,使用putty
    static void loginSsh(QString program,QString ip,QString user,QString pwd,QString port="22");

    static QString toQStringHex(char ch);

    static void messageBox(QString strContent,QString strTitle="提示信息");
    static QString getIPLocal();
    //保存单个文件
    static QString saveDialogFile(QString qstrPathDefault="C:/Program Files/MATLAB/R2010b/bin");
    //选择单个文件
    static QString openDialogFile(QString qstrPathDefault="C:/Program Files/MATLAB/R2010b/bin");
    //选择多个文件
    static QStringList openDialogFiles(QString qstrPathDefault="C:/desktop/401/highspeedphotos/photos/SCENE5");
    //选择文件夹,已做后期处理,结果总是最后带斜杠的
    static QString openDialogSelectDir(QString qstrPathDefault="./");
    //创建文件夹,创建目录
    static bool mkdir(QString path="./");
    //弹出信息提示框

    //弹出输入框,使用方法如下
    //QString strinput;
    //if(qlib::inputbox(strinput))
    //{
    //    qlog(strinput);
    //}
    static bool inputbox(QString &strInput,QString strdefault=QString::fromLocal8Bit("您要输入的文本"));

    //弹出确认框
    static bool confirmbox(QString strcontent);


    static QVector<QMap<QString,QString> > vp2qvp(plist<pmap<pstring,pstring> > pres)
    {
        QVector<QMap<QString,QString> > qres;
        for(size_t i=0;i<pres.size();i++)
        {
            QMap<QString,QString> qmapi;
            pmap<pstring,pstring> mapi=pres[i];
            for( pmap<pstring,pstring>::iterator iter = mapi.begin(); iter != mapi.end(); iter++)
            {
                pstring pkey=iter->first;
                pstring pval=iter->second;

                qmapi.insert(QString::fromStdString(pkey),
                             QString::fromStdString(pval));
            }
            qres.append(qmapi);
        }
        return qres;
    }
    static QVector<QVector<QString> > vvs2qvvs(plist<plist<pstring> > pres)
    {
        QVector<QVector<QString> > vvs;
        for(size_t i=0;i<pres.size();i++)
        {
            QVector<QString> qi;
            for(int j=0;j<pres[i].size();j++)
            {
                qi.append(QString::fromStdString(pres[i][j]));
            }
            vvs.append(qi);
        }
        return vvs;
    }
    template <class T>
    static plist<T> toPlist(QVector<T> qlist)
    {
        plist<T> lres;
        for(size_t i=0;i<qlist.size();i++)
        {
            lres.append(qlist[i]);
        }
        return lres;
    }
    template <class T1,class T2>
    static pmap<T1,T2> toPmap(QMap<T1,T2> qmap)
    {
        pmap<T1,T2> mres;
        for(typename QMap<T1,T2>::iterator it=qmap.begin();it!=qmap.end();it++)
        {
            mres[it.key()]=it.value();
        }
        return mres;
    }
    template <class T1,class T2>
    static QMap<T1,T2> toQmap(pmap<T1,T2> map)
    {
        QMap<T1,T2> mres;
        for(size_t i=0;i<map.size();i++)
        {
            T1 t1=map.getKey(i);
            T2 t2=map[t1];
            mres.insert(t1,t2);
        }
        return mres;
    }

};





class qlibRand
{
public:
    qlibRand();
    int rand(int s,int e);//[s,e)
};

class btcp:public ptcp{
public:
    btcp();
    //下面这个构造函数不许要init
    btcp(ctcp* tcpin);//根据xtcp转换
    btcp(string ip,int port);
    btcp(QString ip,int port);
    btcp(int iSockAccept,string ip,int port);//接受时用,只会是客户端 //该方法不需要init
    int setThreadServerb(void (*fun_thread)(btcp * ptcp));
    template <class T>
    int sendb(string strtype,T t)//############send用这个
    {
        //        string strdata=libBoost::toString(t);
        //        return sendx(strtype,strdata);

        //        string strdata=libBoost::toString(t);

        QByteArray qba=toByteArray(t);
        //        qlog(qba);
        //        qlog(qba.count());
        //        qlog(qba.data());
        //        string strdata=string(qba.data());//如果这样,肯定要失败的,因为不知道长度,必须按长度来赋值
        string strdata;
        for(size_t i=0;i<qba.length();i++)
            strdata.push_back(qba.data()[i]);
        //        qlog(strdata.size());
        return sendx(strtype,strdata);
    }
    //只发包头
    int sendb(string strtype)
    {
        return sendx(strtype);
    }
    int recvb(int timeout=-1)
    {
        return recvx(timeout);
    }


    template <class T>
    T getClass()
    {
        //        return libBoost::toClass<T>(this->getData());
        //必须指定长度,不然无法使用,因为不知道char数组的具体长度
        return toClass<T>(QByteArray(this->getData(),this->length));
    }

private:
    template <class T>
    QByteArray toByteArray(T t)
    {
        //        QPoint pos(5, 7);
        //        qDebug()<<pos;
        //        QPoint tmp;
        //        qDebug()<<tmp;
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::ReadWrite);   //使用QDataStream将Qpoint序列化进QByteArray内
        out.setVersion(QDataStream::Qt_4_8);                //设置Qt串行化版本 使用Qt5.8版本流
        out<<t;
        //        qDebug()<<datagram.length();
        return datagram;
    }
    template <class T>
    T toClass(QByteArray datagram)
    {
        T t;
        QDataStream dsIn(&datagram,QIODevice::ReadWrite);  //使用QDataStream将QByteArray反序列化
        dsIn.setVersion(QDataStream::Qt_4_8);             //设置Qt串行化版本 使用Qt5.8版本流
        dsIn>>t;
        //        qDebug()<<tmp;
        return t;
    }
};


//增加对string 的支持
inline QDataStream& operator>>(QDataStream &in,string& data)
{
    in>>data;
    return in;
}
inline QDataStream& operator<<(QDataStream &out,string& data)
{
    out<<data;
    return out;
}

inline QDataStream& operator>>(QDataStream &in,unsigned long& data)
{
    in>>data;
    return in;
}
inline QDataStream& operator<<(QDataStream &out,unsigned long& data)
{
    out<<data;
    return out;
}
//要加static 不然多个项目重复使用会有重定义错误
static ostream& operator<<(ostream& os,QString str)
{
    os<<pstring(str.toStdString());
    return os;
}
static istream& operator>>(istream& is,QString &str)
{
    pstring data;
    is>>data;
    //    hlog(data);
    str=qlib::toString(data);
    return is;
}
static ostream& operator<<(ostream& os,QStringList xv)
{
    os<<"len: "<<xv.size()<<" QStringList\n{\n";
    for(size_t i=0;i<xv.size();i++)
    {
        os<<xv[i]<<"\n";
    }
    os<<"}";
    return os;
}
static istream & operator>>(istream &is,QStringList &xv)
{
    for(size_t i=0;i<5;i++)
        is.get();

    size_t len=0;
    is>>len;
    //7是纯名字的长度pvector
    common_skipCountch(is,11+4);
    for(size_t i=0;i<len;i++)
    {
        QString t;
        is>>t;

        xv.append(t);
        common_skipCountch(is,1);//跳过空格
    }
    common_skipCountch(is,1);
    return is;
}

template<class T>
static ostream& operator<<(ostream &os,QList<T> xv)
{
    os<<"len: "<<xv.size()<<" QList\n{\n";
    for(size_t i=0;i<xv.size();i++)
    {
        os<<xv[i]<<"\n";
    }
    os<<"}";
    return os;
}
template<class T>
static istream & operator>>(istream &is,QList<T> &xv)
{
    for(size_t i=0;i<5;i++)
        is.get();

    size_t len=0;
    is>>len;
    //7是纯名字的长度pvector
    common_skipCountch(is,5+4);
    for(size_t i=0;i<len;i++)
    {
        T t;
        is>>t;

        xv.append(t);
        common_skipCountch(is,1);//跳过空格
    }
    common_skipCountch(is,1);
    return is;
}

template<class T>
static ostream& operator<<(ostream &os,QVector<T> xv)
{
    os<<"len: "<<xv.size()<<" QVector\n{\n";
    for(size_t i=0;i<xv.size();i++)
    {
        os<<xv[i]<<"\n";
    }
    os<<"}";
    return os;
}
template<class T>
static istream & operator>>(istream &is,QVector<T> &xv)
{
    for(size_t i=0;i<5;i++)
        is.get();

    size_t len=0;
    is>>len;
    //7是纯名字的长度pvector
    common_skipCountch(is,7+4);
    for(size_t i=0;i<len;i++)
    {
        T t;
        is>>t;

        xv.append(t);
        common_skipCountch(is,1);//跳过空格
    }
    common_skipCountch(is,1);
    return is;
}



template<class T1,class T2>
static ostream& operator<<(ostream& os,QMap<T1,T2> qmap)
{
    os<<qlib::toPmap(qmap);
    return os;
}
template<class T1,class T2>
static istream& operator>>(istream& is,QMap<T1,T2> &qmap)
{
    pmap<T1,T2> mapcpp;
    is>>mapcpp;
    hlog(mapcpp);
    qmap =qlib::toQmap(mapcpp);
    return is;
}
#endif // QLIB_H
