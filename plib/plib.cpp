#include "plib.h"


//默认是当前目录下的xlfd.log
string strPathLog("xlfd.log");
string strPathConf("xlfd.conf");
bool bexit = false;
int LenPackTCP = 512 * 1024;
pmutex mut;

plib::plib() {

}

//第一个是所有文件全路径列表，第二个是所有目录全路径列表，第三个是所有文件名相对路径列表，第四个是所有目录相对路径列表
plist<plist<pstring> > plib::getAllFilesAndPathsRecurision(const string path) {
    //获取全路径
    vector<string> lfall;
    vector<string> lpall;
    common_getAllFilesAndPathsRecursion(path, lfall, lpall);

    plist<pstring> plfall;
    plist<pstring> plpall;
    for (int i = 0; i < lfall.size(); i++)
        plfall.append(lfall[i]);
    for (int i = 0; i < lpall.size(); i++)
        plpall.append(lpall[i]);


    //    hlog(plpall);
    //获取相对路径等
    int lenRemove = path.size();
    if (path[path.size() - 1] != '/')
        lenRemove++;
    //        hlog(path);
    //    hlog(path.size());
    hlog(lenRemove);
    //    相对路径的文件和相对路径的目录,要加上上一目录，不然考过去全是文件夹下的所有东西

    string strLastDir = path;
    if (path[path.size() - 1] != '/')
        strLastDir += "/";
    //    hlog(strLastDir);
    plist<pstring> lspit = pstring(strLastDir).split("/");
    //        hlog(lspit);
    pstring pstrlast = lspit[lspit.size() - 2];
    //    hlog(pstrlast);
    strLastDir = pstrlast + "/";
    //    hlog(strLastDir);




    plist<pstring> plf;
    plist<pstring> plp;
    for (int i = 0; i < plfall.size(); i++) {
        //要增加上一目录
        //        plf.append(strLastDir+lfall[i].substr(lenRemove,lfall[i].size()));
        plf.append(lfall[i].substr(lenRemove, lfall[i].size()));
    }
    for (int i = 0; i < plpall.size(); i++) {
        //        plp.append(strLastDir+lpall[i].substr(lenRemove,lpall[i].size()));
        plp.append(lpall[i].substr(lenRemove, lpall[i].size()));
    }


    plist<plist<pstring> > lres;
    //第一个是所有文件全路径列表，第二个是所有目录全路径列表，第三个是所有文件名相对路径列表，第四个是所有目录相对路径列表
    lres.append(plfall);
    lres.append(plpall);
    lres.append(plf);
    lres.append(plp);
    return lres;
}


string plib::getPathFromFullPath(string strFullPath) {
    vector<string> lstr = common_split(strFullPath, "/");
    string stres = "/";
    for (int i = 0; i < lstr.size() - 1; i++)
        stres += lstr[i] + "/";
    return stres;
}

string plib::getFileNameFromFullPath(string strFullPath) {
    vector<string> lstr = common_split(strFullPath, "/");
    return lstr[lstr.size() - 1];
}

string plib::getmd5(string path) {
    string strall = getShellOutput("md5sum \"" + path + "\"");
    //    hlog(strall);
    vector<string> lstr = common_split(strall, " ");
    return lstr[0];
}


string plib::pwd() {
    char buf[1024];
    getcwd(buf, sizeof(buf));
    return string(buf);
}

//plib::setPathConf("/opt/sc/csgl/SCService/sc.conf");
int plib::setPathConf(string strnewpath) {
    if (!isExsistDir(strnewpath)) {
        hlog(pstring() << "配置文件" << strnewpath << "不存在,请检查");
        return -1;
    }
    strPathConf = strnewpath;
    return 0;
}

//plib::setPathLog("/opt/sc/csgl/SCService/log/SCService.log");
int plib::initLog(string strnewpath) {
    clib_initLog(strnewpath.c_str());
    if (strnewpath.substr(strnewpath.length() - 4, strnewpath.length()) != ".log") {
        hlog("日志路径中必须包含文件名");
        return -1;
    }
    //先获取纯路径
    string strpathonly;
    vector<string> vstr = common_split(strnewpath, "/");
    for (int i = 0; i < vstr.size() - 1; i++) {
        strpathonly += vstr[i] + "/";
    }
    strpathonly = "/" + strpathonly;
    //        hlog(strpathonly);

    strPathLog = strnewpath;
    if (!isExsistDir(strpathonly)) {
        mkdirp(strpathonly.c_str());
        hlog(pstring() << "日志路径" << strpathonly << "不存在，递归创建成功");
    } else
        hlog(pstring() << "日志路径" << strpathonly << "已存在，可正常写入日志");
    return 0;
}


bool plib::isContaiAlpha(string str) {
    for (int i = 0; i < str.size(); i++) {
        if (isalpha(str.at(i)))
            return true;
    }
    return false;
}

string plib::hostToIp(const char *url) {
    struct hostent *h;

    if ((h = gethostbyname(url)) == NULL) {
        //        printf("不能得到IP %s\n",stderr);
        hlog("获取ip错误");
        hlog(stderr);
        return "";
    }
    //    printf("HostName :%s\n",h->h_name);
    hlog("域名：");
    hlog(h->h_name);
    char *arrchip = inet_ntoa(*((struct in_addr *) h->h_addr));
    //    printf("IP Address :%s\n",arrchip);
    hlog("地址：");
    hlog(arrchip);
    return string(arrchip);
}


plist<long> plib::getDiskUseInfo(const char *arrchPath) {
    //获取结果那一行，因为有两种情况：第一种
    //Filesystem           1K-blocks     Used Available Use% Mounted on
    ///dev/mapper/vg_develop-lv_root
    //                      36661960 19709392  15083564  57% /
    //第二种
    //Filesystem     1K-blocks  Used Available Use% Mounted on
    //-                2005968   168   2005800   1% /dev
    //所以先获取结果那一行，然后获取数字

    pstring stres = plib::getShellOutput("df \"" + string(arrchPath) + "\"|grep \"%\"|grep -v Mounted");
    //    hlog(stres);
    plist<pstring> listall = stres.split(" ");
    //    hlog(listall);
    plist<pstring> listres;
    //只要纯数字
    for (int i = 0; i < listall.size(); i++) {
        if (listall[i].isNum())
            listres.append(listall[i]);
    }
    //    hlog(listres);
    plist<long> listlong;
    if (listall.size() > 0) {
        listlong.append(atol(listres[0].c_str()));
        listlong.append(atol(listres[1].c_str()));
    }
    return listlong;
}

bool plib::createFile(const char *arrchFullPath) {
    int ires = clib_createFile(arrchFullPath);
    if (ires < 0)
        return false;
    return true;
}

bool plib::remove(const char *path) {
    clib_remove(path);
}


void plib::setSignals(void (*funExit)(int id), void (*funPipe)(int id)) {
    clib_setAllSignals(funExit, funPipe);
}

bool plib::isExsistProcess(string strNameProcess) {
    if (clib_isProcessExist(strNameProcess.c_str()))
        return true;
    return false;
}


int plib::daemonize() {
    int ires= clib_daemonize();
    return ires;
}

string plib::mail(string addr, string title, string content) {
    //echo content|mail -s title xlfdlihailong@126.com
    string cmd = "echo " + content + "|mail -s " + title + " " + addr;
    //    return system(cmd.c_str());
    return plib::getShellOutput(cmd);
}


/*
 *
 *	名称:	 GetPrivateProfileString ()
 *
 *	参数1: pchSectionName		节名
 *	参数2: pchKeyName				键名
 *	参数3: pchDefStr				缺省值
 *	参数4: pchRetBuf				存放返回字符串的缓冲区
 *	参数5: uiBufSize				retbuf的大小
 *	参数6: pchFileName			配置文件名
 *
 *	返回值:	若找到相应Key的值则返回该值，否则返回NULL。
 *注意： 读取字符串时，要先memset，不然后面可能有乱码
 *	功能:	返回的字符串的大小，不包括字符串结束符；若未找到,则返回小于0的数。
 *
*/
int plib_getConfString(const char *pchSectionName, const char *pchKeyName,
                       char *pchRetBuf) {
    unsigned int uiBufSize = 256;
    int iFd;                            //文件句柄
    char arrchBuf[256];        //缓存
    int iResult;                    //返回值
    int i, j;
    int iTmp;
    int iSectionFinded;        //节名找到标志

    //初始化数组
    memset(arrchBuf, 0, 256);

    //为变量赋初值
    iSectionFinded = 0;

    //打开指定的文件。
    //    iFd	= open(gs_strConfPath,O_RDWR);
    iFd = open(strPathConf.c_str(), O_RDWR);
    if (iFd == -1) {
        //        strcpy(pchRetBuf,pchDefStr);
        //        return strlen(pchDefStr);
        //        pchRetBuf=NULL;
        return -1;
    }

    while (1) {
        //从文件中读一行。
        iResult = clib_readLine(iFd, arrchBuf, 256);
        //        hlog(arrchBuf);
        //        hlog(iResult);
        if ((iResult == -1) || (iResult == 0)) {

            //关闭文件
            close(iFd);
            //            strcpy(pchRetBuf,pchDefStr);
            //返回指定的缺省值
            //            return strlen(pchDefStr);
            //            pchRetBuf=NULL;
            return -2;
        }

        //判断节名是否已找到
        if (iSectionFinded == 0) {
            //查找section。
            for (i = 0; i < iResult; i++) {
                //忽略空格和制表符。
                if ((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) {
                    continue;
                }

                //若第i个字符不为'['，终止循环，从新读一行
                if (arrchBuf[i] != '[') {
                    break;
                }

                //若此节名与要查找的节名长度不同，终止循环，从新读一行
                if (arrchBuf[i + strlen(pchSectionName) + 1] != ']') {
                    break;
                }

                //若此节名与要查找的节名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i + 1], pchSectionName, strlen(pchSectionName)) != 0) {
                    break;
                } else {
                    //设置节名找到标志为1
                    iSectionFinded = 1;
                    break;
                }
            }
        } else {
            //查找key，并获得它的值。
            for (i = 0; i < iResult; i++) {
                //忽略空格和制表符。
                if ((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) {
                    continue;
                }

                //若第i个字符不为'['，终止循环
                if (arrchBuf[i] == '[') {
                    //关闭文件
                    close(iFd);
                    //                    strcpy(pchRetBuf,pchDefStr);
                    //                    return strlen(pchDefStr);
                    //                    pchRetBuf=NULL;
                    return -3;
                }

                //若此键名与要查找的键名长度不同，终止循环，从新读一行
                if ((arrchBuf[i + strlen(pchKeyName)] != 9) && (arrchBuf[i + strlen(pchKeyName)] != 32) &&
                    (arrchBuf[i + strlen(pchKeyName)] != '=')) {
                    break;
                }

                //若此键名与要查找的键名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i], pchKeyName, strlen(pchKeyName)) != 0) {
                    break;
                }

                //把i移到键名后
                i += strlen(pchKeyName);
                //忽略键名后的空格和制表符。
                while (((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) && (i < iResult)) {
                    i++;
                }

                if (i == iResult) {
                    break;
                }

                if (arrchBuf[i] != '=') {
                    break;
                }

                i++;
                while (((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) && (i < iResult)) {
                    i++;
                }

                if (i == iResult) {
                    break;
                }

                //获得字符串值
                iTmp = uiBufSize >= (unsigned int) (iResult - i + 1) ? iResult - i : (int) uiBufSize - 1;
                memcpy(pchRetBuf, &arrchBuf[i], iTmp);
                j = strlen(pchRetBuf);
                //消除右边的空格和\n
                while ((j >= 0) && (((*(pchRetBuf + j - 1)) == ' ') || ((*(pchRetBuf + j - 1)) == 10) ||
                                    ((*(pchRetBuf + j - 1)) == '\r'))) {
                    j--;
                }
                *(pchRetBuf + j) = 0;
                close(iFd);
                return iTmp;

            }
        }
    }
}

/*
 *
 *	名称:	WritePrivateProfileString	()
 *
 *	参数1: pchSectionName		节名
 *	参数2: pchKeyName				键名
 *	参数3: pchString				包含键名及键值的字符串
 *	参数4: pchFileName			配置文件名
 *
 *	返回值:	若成功则返回0，否则返回-1。
 * 如果有，则会改值，如果没找到的话，不做任何操作，返回-3
 *	功能:	将一个Key和它的值写入指定的配置文件。
 *
*/
int plib_setConfString(const char *pchSectionName, const char *pchKeyName, const char *pchString) {
    int iFd;                                    //文件句柄
    char arrchBuf[256];                //缓存
    int iResult;                            //返回值
    int i;
    int iSectionFinded;                //节名找到标志
    int iTmpFd;                                //临时文件句柄
    int iTmpResult;                        //临时返回值
    int iTotalLen;                        //总长度
    char arrchTmpBuf[256];        //临时缓冲区
    struct stat struFileStat;
    char arrchTmpString[256];    //临时字符串

    //初始化数组
    memset(arrchBuf, 0, 256);
    memset(arrchTmpBuf, 0, 256);
    memset(arrchTmpString, 0, 256);

    //为变量赋初值
    iTotalLen = 0;
    iSectionFinded = 0;

    //打开指定的文件。
    //    iFd	=	open(gs_strConfPath,	O_RDWR);
    iFd = open(strPathConf.c_str(), O_RDWR);
    if (iFd == -1) {
        return -1;
    }

    fstat(iFd, &struFileStat);

    while (1) {
        //从文件中读一行。
        iResult = clib_readLine(iFd, arrchBuf, 256);
        //        hlogone(iResult);
        //        hlogone(arrchBuf);
        if (iResult == -1) {
            //关闭文件
            close(iFd);
            return -2;
        } else if (iResult == 0) {
            //这里注释，表示没有找到的话就不做任何操作


            //            if (iSectionFinded == 0)
            //            {
            //                sprintf(arrchBuf,	"\n[%s]\n",	pchSectionName);
            //                write(iFd, arrchBuf, strlen(arrchBuf));
            //            }

            //            sprintf(arrchBuf,	"%s=%s\n", pchKeyName, pchString);
            //            write(iFd, arrchBuf, strlen(arrchBuf));

            close(iFd);
            errno = ENOENT;
            return -3;
        }

        if (iSectionFinded == 0) {
            //查找section。
            for (i = 0; i < iResult; i++) {
                //忽略空格和制表符。
                if ((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) {
                    continue;
                }

                //若第i个字符不为'['，终止循环，从新读一行
                if (arrchBuf[i] != '[') {
                    break;
                }

                //若此节名与要查找的节名长度不同，终止循环，从新读一行
                if (arrchBuf[i + strlen(pchSectionName) + 1] != ']') {
                    break;
                }

                //若此节名与要查找的节名不同，终止循环，从新读一行
                if (strncmp(&arrchBuf[i + 1], pchSectionName, strlen(pchSectionName)) != 0) {
                    break;
                } else {
                    iSectionFinded = 1;
                    break;
                }
            }//for (i=0; i<iResult;	i++)
        } else {
            //查找key，并替换原来的值。
            for (i = 0; i < iResult; i++) {
                //忽略空格和制表符。
                if ((arrchBuf[i] == 9) || (arrchBuf[i] == 32)) {
                    continue;
                }

                if (arrchBuf[i] != '[') {
                    if ((arrchBuf[i + strlen(pchKeyName)] != 9) && (arrchBuf[i + strlen(pchKeyName)] != 32) &&
                        (arrchBuf[i + strlen(pchKeyName)] != '=')) {
                        break;
                    }

                    if (strncmp(&arrchBuf[i], pchKeyName, strlen(pchKeyName)) != 0) {
                        break;
                    }
                }

                iTmpFd = open("~123456.tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
                if (iTmpFd == -1) {
                    close(iFd);

                    return -4;
                }

                if (arrchBuf[i] == '[') {
                    //移动文件指针
                    lseek(iFd, 0 - iResult - 1, SEEK_CUR);
                }

                iTmpResult = read(iFd, arrchTmpBuf, 256);
                while (iTmpResult != 0) {
                    iTotalLen += iTmpResult;
                    write(iTmpFd, arrchTmpBuf, iTmpResult);

                    iTmpResult = read(iFd, arrchTmpBuf, 256);
                }//while (iTmpResult !=	0)

                if (arrchBuf[i] == '[') {
                    //移动文件指针
                    lseek(iFd, 0 - iTotalLen, SEEK_END);
                } else {
                    //移动文件指针
                    lseek(iFd, 0 - iResult - iTotalLen - 1, SEEK_END);
                }

                //移动文件指针
                lseek(iTmpFd, 0 - iTotalLen, SEEK_CUR);

                sprintf(arrchTmpString, "%s=%s\n", pchKeyName, pchString);
                write(iFd, arrchTmpString, strlen(arrchTmpString));
                iTmpResult = read(iTmpFd, arrchTmpBuf, 256);
                while ((iTmpResult != -1) && (iTotalLen > 0)) {
                    write(iFd, arrchTmpBuf, iTmpResult);
                    iTotalLen -= iTmpResult;
                    iTmpResult = read(iTmpFd, arrchTmpBuf, 256);
                }//while ((iTmpResult	!= -1) &&	(iTotalLen > 0))

                if (arrchBuf[i] != '[') {
                    if (strlen(arrchTmpString) < (size_t) (iResult + 1)) {
                        ftruncate(iFd, struFileStat.st_size + strlen(arrchTmpString) - iResult - 1);
                    }
                }

                close(iFd);
                close(iTmpFd);
                unlink("~123456.tmp");
                return 0;
            }
        }//for (i=0; i<iResult;	i++)
    }//while (1)
}

string plib::getConfString(string section, string key) {
    if(!plib::isExsistFile(strPathConf))
    {
        hlog(pstring()<<"找不到配置文件"<<strPathConf);
        return "";
    }
    char config[2048];
    bzero(config, 2048);
    int ires = plib_getConfString(section.c_str(), key.c_str(), config);
    //    hlog(ires);
    if (ires < 0) {
        string info = "读取配置文件失败，请检查路径" + strPathConf + "或section:" + section + "配置项" + key + "是否正确";
        hlog(info);
        hlog(strPathConf);
        //        hlog(getShellOutput("pwd"));

        return "";
    }
    //    hlog(config);
    string readconf = strPathConf + "  [" + section + "] " + key + "=" + config;
//    hlog(readconf);
    return string(config);
}
bool plib::delConfString(string section, string key) {
    hlog(strPathConf);
    if(!plib::isExsistFile(strPathConf))
    {
        hlog(pstring()<<"找不到配置文件"<<strPathConf);
        return false;
    }
    //先找section,如果找不到section,就不用加了,不然会一直找不到一直加
    pstring strall = plib::getContentsAllFromFile(strPathConf);
    //    hlog(strContentAll);
    if (strall == "")
        return false;
    //section [basic]
    pstring strSectionDes = "[" + section + "]";
    hlog(strall.contain(strSectionDes));
    //如果找不到section,删除失败
    if (!strall.contain(strSectionDes))
    {
        hlog(pstring() << "找不到该section" << strSectionDes << "删除失败");
        return false;
    }
    else
    {
        //找到了这个section,再找key
        //要根据所有section分组
        //先找key,整个文件找如果没有key,删除失败
        if (!strall.contain(key+"="))
        {
            hlog(pstring() << "没有找到该key" << key << "删除失败");
            return false;
        }
        else
        {
            //找到所有key,还要看key是不是这个section的,这里用一个偷懒的办法,从key的
            //位置从后往前找section,找到的section匹配则说明是要找的,否则就不是这个section的
            //也是删除失败

            plist<string> lines=plib::getAllLinesFromFile(strPathConf);
            //先找到key所在的行
            int indexKey=-1;
            for(int i=0;i<lines.size();i++)
            {
                pstring li=lines[i];
                if(li.contain(key+"="))
                {
                    indexKey=i;
                    break;
                }
            }
//            hlog(indexKey,lines[indexKey]);
            //然后从后往前找【】看是否对应
            int ifind=-1;
            for(int i=indexKey;i>=0;i--)
            {
                pstring li=lines[i];
                hlog(i,li);
                if(li.contain("[")&&li.contain("]"))
                {
                    ifind=i;
                    break;
                }
            }
//            hlog(ifind);
//            hlog(lines[ifind]);
            if(lines[ifind]!=strSectionDes)
            {
                hlog(pstring()<<"要删除的section是"<<section<<",而找到的key对应的section为"<<lines[ifind]<<"，不匹配，删除失败");
                return false;
            }
            //section匹配才可以删
            lines.deleteByIndex(indexKey);
//            hlog(lines);
            return plib::saveLines(strPathConf,lines);
        }
    }
}

bool plib::addConfString(string section, string key, string value) {
    hlog (strPathConf);
    if(!plib::isExsistFile(strPathConf))
    {
        hlog(pstring()<<"找不到配置文件"<<strPathConf);
        return false;
    }
    //先找section,如果找不到section,就不用加了,不然会一直找不到一直加
    string strContentAll = plib::getContentsAllFromFile(strPathConf);
    //    hlog(strContentAll);
    if (strContentAll == "")
        return false;
    //section [basic]
    string strSectionToAdd = "[" + section + "]";
    string strConfToAdd = key + "=" + value;
    hlog(strSectionToAdd, strConfToAdd);
    hlog(pstring(strContentAll).contain(strSectionToAdd));
    //如果找不到section,最下边新家section,再新加配置
    if (!pstring(strContentAll).contain(strSectionToAdd)) {
        hlog(strSectionToAdd);
        if (!plib::saveStringAppend(strPathConf, strSectionToAdd)) {
            hlog("追加section失败");
            return false;
        }

        if (!plib::saveStringAppend(strPathConf, strConfToAdd)) {
            hlog("追加配置失败");
            return false;
        } else {
            return true;
        }
    } else {//如果找到了section,那么先看这个section下有没有,如果有,就不增加,

        if (plib::getConfString(section, key) != "") {
            hlog(pstring() << "已经有该配置" << key << ",无法增加,增加配置失败");
            return false;
        } else {// 如果没有则在section下第一行增加
            //先找section位置
//            hlog(strSectionToAdd);
            int index = strContentAll.find(strSectionToAdd);
//            hlog(index);
            return plib::insertStringToFile(strPathConf, "\n" + strConfToAdd, strSectionToAdd);
        }
    }
}

bool plib::setConfString(string section, string key, string value) {
    hlog(strPathConf);
    if(!plib::isExsistFile(strPathConf))
    {
        hlog(pstring()<<"找不到配置文件"<<strPathConf);
        return false;
    }
    int ires = plib_setConfString(section.c_str(), key.c_str(), value.c_str());
//    hlog(ires);
    if (ires < 0) {
        string info = "读取配置文件失败，请检查路径" + strPathConf + "或section:" + section + "配置项" + key + "是否正确";
        hlog(info);
        return false;
    } else {
        string info = "成功将[" + section + "]下的配置项" + key + "的值改为" + value;
        hlog(info);
        return true;
    }
}

//新的算法，根据网络名分组，可判断是否没网等，去除vmnet lo等,没网返回空
pstring plib::getIPLocal() {
    //原来的方式，过于简单，会出现多个ip，比如vmnet这种的
    //    string strIpLocal=getShellOutput(
    //                "/sbin/ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d 'addr:'");



    //新的算法，根据网络名分组，可判断是否没网等，去除vmnet lo等,没网返回空
    pstring strall = plib::getShellOutput("ifconfig");
    //    hlog(strall);
    pliststring liststr = strall.split("\n");
    //        hlog(liststr);

    pliststring listRes;
    for (int i = 0; i < liststr.size(); i++) {
        pstring stri = liststr[i];
        if (stri[0] != ' ') {
            listRes.append(stri + liststr[i + 1]);
            i++;
        }
    }
    //    hlog(listRes);
    pmap<string, string> mapall;
    //要区分centos6系列和centos7系列
    //centos6:
    //    vmnet8    Link encap:Ethernet  HWaddr 00:50:56:C0:00:08
    //              inet addr:192.168.9.1  Bcast:192.168.9.255  Mask:255.255.255.0
    //centos7:
    //ens33: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
    //        inet 192.168.211.155  netmask 255.255.255.0  broadcast 192.168.211.255

    for (int i = 0; i < listRes.size(); i++) {
        pliststring listsplit = listRes[i].split(" ");
        //        hlog(listsplit);
        pstring straddr;
        for (int j = 1; j < listsplit.size(); j++) {
            pstring stri = listsplit[j];
            //            hlog(stri);
            //先找到inet
            if (stri == "inet") {
                //看下一个是啥，如果下一个有addr，则cento6，如果没有则centos7
                pstring strinext = listsplit[j + 1];
                if (strinext.substr(0, 4) == ("addr")) {
                    //                hlog(stri);
                    straddr = strinext.substr(5, strinext.size());
                } else {
                    straddr = strinext;
                }

            }
        }
                mapall.add(listsplit[0],straddr);
//        mapall[listsplit[0]] = straddr;
    }
    //    hlog(mapall);

    plist<pstring> listres;
    for (int i = 0; i < mapall.getKeys().size(); i++) {
        pstring stri = mapall.getKeys()[i];
        //virbr也是虚拟网口，要去掉
        if (!stri.contain("lo") && !stri.contain("vmnet") && !stri.contain("virbr")) {
            //如果没网，eth0有，但是ip是空，要判断
            if (mapall[stri] != "")
                listres.append(mapall[stri]);
        }
    }
    if (listres.size() > 1) {
        hlog("找到多个本机ip,取第一个,不对的话自己设吧");
        hlog(listres);
//        return "";
        return listres[0];
    }

    if (listres.size() == 0) {
        hlog("未找到本机ip，请检查网络");
        return "";
    }
    if (listres.size() == 1) {
        hlog(pstring() << "本机ip为  " << listres[0]);
        return listres[0];
    }
}

string plib::getShellOutput(string shell) {
    FILE *pipe = popen(shell.c_str(), "r");
    if (!pipe)
        return "";
    char buffer[666666];
    bzero(buffer, sizeof(buffer));
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 666666, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    if (result.size() > 0) {
        //如果有结果会多一个换行符，在这里去掉
        if (result[result.length() - 1] == '\n')
            result = result.substr(0, result.length() - 1);
    }
    return result;
}


void plib::monitorLogOneFileOneConstrain(string path, string key) {
    int iCountOld = plib::getLineCountContains(path, key);
    while (1) {
        int iCountNew = plib::getLineCountContains(path, key);
        if (iCountNew > iCountOld) {
            hlog(plib::getLineLastContains(path, key));
        }
        iCountOld = iCountNew;
    }
}

void plib::monitorLogOneFileMultiConstrain(string path, plist<string> keys) {
    vector<int> vCountOld(keys.size());
    vector<int> vCountNew(keys.size());
    for (int i = 0; i < keys.size(); i++)
        vCountOld[i] = plib::getLineCountContains(path, keys[i]);

    while (1) {
        for (int i = 0; i < keys.size(); i++) {
            vCountNew[i] = plib::getLineCountContains(path, keys[i]);
            if (vCountNew[i] > vCountOld[i]) {
                hlog(plib::getLineLastContains(path, keys[i]));
            }
            vCountOld[i] = vCountNew[i];
        }
    }
}

void plib::monitorLogMultiFileMultiConstrain(plist<pstring> paths, plist<pstring> keys) {
    hlog(paths);
    hlog(keys);

    int vCountOld[paths.size()][keys.size()];
    int vCountNew[paths.size()][keys.size()];
    for (int j = 0; j < paths.size(); j++) {
        for (int i = 0; i < keys.size(); i++)
            vCountOld[j][i] = plib::getLineCountContains(paths[j], keys[i]);
    }

    while (1) {
        for (int j = 0; j < paths.size(); j++) {
            for (int i = 0; i < keys.size(); i++) {
                vCountNew[j][i] = plib::getLineCountContains(paths[j], keys[i]);
                if (vCountNew[j][i] > vCountOld[j][i]) {
                    //做处理
                    hlog(paths[j]);
                    hlog(keys[i]);
                    hlog(plib::getLineLastContains(paths[j], keys[i]));
                }
                vCountOld[j][i] = vCountNew[j][i];
            }
        }
        //防止cpu占用过高
        usleep(10);
    }
}

//获取path文件中含有strsub的行个数，不要带空格
int plib::getLineCountContains(string path, string strsub) {
    plist<string> vall = plib::getAllLinesFromFile(path);
    int iCount = 0;
    for (int i = 0; i < vall.size(); i++) {
        string strtmp = vall[i];
        if (strtmp.find(strsub) != string::npos) {
            iCount++;
        }
    }
    return iCount;
}

//获取path文件中含有strsub的最后一行,不要带空格
pstring plib::getLineLastContains(string path, string strsub) {
    plist<string> vall = plib::getAllLinesFromFile(path);
    for (int i = vall.size() - 1; i >= 0; i--) {
        string strtmp = vall[i];
        if (strtmp.find(strsub) != string::npos)
            return strtmp;
    }
    return "";
}

bool plib::saveStringAppend(string path, string content) {
//    pfile fappend(path,typeappend);
//    if(fappend.save(content)<0)
//        return false;
//    return true;
    ofstream os;
    os.open(path.c_str(),ios::app);
    os << content;
    os.close();
}

bool plib::saveString(string path, string content) {
    //用这个会变成二进制格式,vim后边带符号?
//    ofstream os;
//    os.open(path.c_str());
//    os << content;
//    os.close();
//用这个会导致文件减少...啥情况
//    pfile fsave(path,typesave);
//    if(fsave.save(content.c_str(),content.size())<0)
//        return false;
//    return true;
    ofstream out(path.c_str());
    if (out.is_open())
    {
        out<<content;
        out.close();
        return true;
    }
    return false;
}

bool plib::saveLines(string path, plist<string> keys) {
    string stres;
    for(int i=0;i<keys.size();i++)
    {
//        hlog(keys[i]);
        stres+=keys[i]+"\r\n";
    }
//    stres+=keys[keys.size()-1];
//    hlog(stres);
    return plib::saveString(path,stres);
}

plist<string> plib::getAllLinesFromFile(string path)
{
    string strload=plib::getContentsAllFromFile(path);

//    plist<string> lsplit=pstring(strload).split("\r\n");
    vector<string> vres=common_split(strload,"\r\n");
    plist<string> lres;
    for(int i=0;i<vres.size();i++)
        lres.append(vres[i]);
    return lres;
}


long long plib::getFileSize(string strFullPath) {
    long long llres = clib_getFileSize(strFullPath.c_str());
    //    hlog(llres);
    return llres;
}

string plib::getTimeNow() {
    char acTime[20];
    bzero(acTime, 20);
    clib_getStringTimeNow(acTime);
    return string(acTime);
}

string plib::getTimeNowNoSplit() {
    pstring strnow = plib::getTimeNow();
    pliststring listsplit = strnow.split(": -.");
    return listsplit.join("");
}

string plib::getDateNow() {
    char acnow[20];
    bzero(acnow, 20);
    clib_getStringTimeNow(acnow);
    char acdate[11];
    bzero(acdate, 11);
    memcpy(acdate, acnow, 10);
    return string(acdate);
}

string plib::getTimeHHMMSS() {
    string strTmNow = plib::getTimeNow();
    return strTmNow.substr(11, 8);
}

bool plib::isExsistFile(string strpath) {
    fstream _file;
    _file.open(strpath.c_str(), ios::in);
    if (!_file)
        return false;
    else
        return true;
}

bool plib::isExsistDir(string strpath) {
    //0就是判断存在性，无论时文件还是文件夹都能判断
    if (access(strpath.c_str(), 0) < 0)
        return false;
    else
        return true;
}

bool plib::insertStringToFile(string path, string content, string keyFind) {
    string strAll = plib::getContentsAllFromFile(path);
    int ifind = strAll.find(keyFind) + keyFind.size();
    if (ifind == string::npos) {
        hlog(pstring() << "未找到该字符串" << content << "插入失败");
        return false;
    }
    string strfront = strAll.substr(0, ifind);
    string strbehind = strAll.substr(ifind, strAll.size());
    string strAllNew = strfront + content + strbehind;
//    hlog(strAll);
//    hlog(strAllNew);
    return plib::saveString(path, strAllNew);
}




bool pstring::isNum() {
    stringstream sin(*this);
    double d;
    char c;
    if (!(sin >> d)) {
        return false;
    }
    if (sin >> c) {
        return false;
    }
    return true;

}




void *fun_thread_xpackcppjoin(void *para) {
    xparacpp *xp = (xparacpp *) para;
    xp->fun_thread(xp->para);

    xp->isFinish = true;
    return NULL;
}

void *fun_thread_xpackcpp(void *para) {
    pthread_detach(pthread_self());
    xparacpp *xp = (xparacpp *) para;
    xp->fun_thread(xp->para);

    xp->isFinish = true;
    return NULL;
}


//不等带完成
int pthread::start(void (*fun_thread)(void *), void *para) {
    xp.fun_thread = fun_thread;
    xp.para = para;
    pthread_create(&tid, NULL, fun_thread_xpackcpp, &xp);
}

//等带完成
int pthread::startjoin(void (*fun_thread)(void *), void *para) {
    xp.fun_thread = fun_thread;
    xp.para = para;
    pthread_create(&tid, NULL, fun_thread_xpackcppjoin, &xp);
    pthread_join(tid, NULL);
}

pthreadpackc::pthreadpackc() {
    this->pth = cthreadInit();
}

int pthreadpackc::start(void (*fun_thread)(void *), void *para) {
    //        hlog(*(int*)para);
    cthread_start(this->pth, fun_thread, para);
}


int pthreadpackc::startjoin(void (*fun_thread)(void *), void *para) {
    cthread_startjoin(this->pth, fun_thread, para);
}

bool pthreadpackc::operator<(const pthreadpackc &th) const {
    return this->pth->tidthread < th.pth->tidthread;
}


pmutex::pmutex() {
    this->mut = cmutexInit();
}

pmutex::~pmutex() {
    this->mut->destroy(this->mut);
}

int pmutex::lock() {
    this->mut->lock(this->mut);
    return 0;
}

int pmutex::unlock() {
    this->mut->unlock(this->mut);
    return 0;
}


pfile::pfile(string strFullPath, enum typexfile type) {
    pf = cfileInit(strFullPath.c_str(), type);
    this->strpath = strFullPath;
    this->dataAll = NULL;
    this->sizeFile = plib::getFileSize(this->strpath);

    if (type == typeload)
        this->strtype = "load";
    if (type == typeappend)
        this->strtype = "append";
    if (type == typesave)
        this->strtype = "save";
}

pfile::~pfile() {
    //不能在此加任何东西，不然影响打印，打印时复制
    //    if(this->dataAll!=NULL)
    //        free(this->dataAll);
    //    free(pf);
    //    pf=NULL;
}



int pfile::save(const char *data, int len) {
    int ires = cfileSave(pf, (void *) data, len);
    if (ires == 0)
        return -1;
    return ires;
}
//正常fread是读到0就是末尾了，这个封装成<0就是读不出来了
int pfile::load(char *data, int len) {
    int ires = cfileLoad(pf, data, len);
    if (ires == 0)
        return -1;
    return ires;
}
int pfile::loadAll() {
    this->dataAll = HMALLOC(char, this->sizeFile);
//    return cfileLoad(pf, this->dataAll, this->sizeFile);
    return load(this->dataAll,this->sizeFile);
}


int pfile::move(unsigned long long pos) {
    return cfilemove(pf, pos);
}

unsigned long long pfile::size() {
    return pf->size(pf);
}

int pfile::close() {
    int ires = fclose(pf->fp);
    if (this->dataAll != NULL)
        free(this->dataAll);
    free(pf);
    pf = NULL;
    return ires;
}


ptime::ptime() {
    init();
    this->time = clib_getTimeNow();
}

void ptime::init() {
    gettimeofday(&(this->time.struTime), NULL); //gettimeofday(&start,&tz);结果一样
}

void ptime::setNowTime() {
    this->init();
    this->time = clib_getTimeNow();
}


ptime::ptime(xctime time) {
    init();
    this->time = time;
}

ptime::ptime(int year, int mon, int day, int hour, int min, int sec,int ms) {
    init();
    //直接赋值有错，还是直接用字符串比较好
    //    this->time.ushYear=year;
    //    this->time.uchMonth=mon;
    //    this->time.uchDay=day;
    //    this->time.uchHour=hour;
    //    this->time.uchMinute=min;
    //    this->time.uchSecond=sec;

    string strtime = plib::toString(year) + "-";
    string strmon;
    if (mon < 10)
        strmon = "0" + plib::toString(mon);
    else
        strmon = plib::toString(mon);
    string strday;
    if (day < 10)
        strday = "0" + plib::toString(day);
    else
        strday = plib::toString(day);
    string strhour;
    if (hour < 10)
        strhour = "0" + plib::toString(hour);
    else
        strhour = plib::toString(hour);
    string strmin;
    if (min < 10)
        strmin = "0" + plib::toString(min);
    else
        strmin = plib::toString(min);
    string strsec;
    if (sec < 10)
        strsec = "0" + plib::toString(sec);
    else
        strsec = plib::toString(sec);
    string strms;
    if(ms<10)
        strms="00"+plib::toString(ms);
    else if(ms>=10&&ms<100)
        strms="0"+plib::toString(ms);
    else
        strms=plib::toString(ms);

    strtime=strtime+strmon+"-"+strday+" "+strhour+":"+strmin+":"+strsec+"."+strms;
    hlog(strtime);

    this->time=clib_getTimeFromString(strtime.c_str());
}

ptime::ptime(string strTime) {
    init();
    //    clib c=clibInit();
    //    this->time=c.getTimeFromString(strTime.c_str());
    this->time = clib_getTimeFromString(strTime.c_str());
}

ptime::ptime(const char *acTime) {
    init();
    //    clib c=clibInit();
    //    this->time=c.getTimeFromString(acTime);
    this->time = clib_getTimeFromString(acTime);
}
pstring ptime::getStringTimeNow() {
    pstring strTimeNow=plib::getTimeNow();
//    hlog(strTimeNow);
    return strTimeNow.substr(11,strTimeNow.size());
}

pstring ptime::getStringTimeNowNoMs() {
    return plib::getTimeHHMMSS();
}

pstring ptime::getStringTimeFullNowNoMs() {
    pstring strTimeFull= ptime::getStringTimeFullNow();
    return strTimeFull.substr(0,strTimeFull.size()-4);
}

pstring ptime::getStringTimeFullNow() {
    char acTime[22];
    bzero(acTime, 22);
    clib_getStringTimeNow(acTime);
    return string(acTime);
}

pstring ptime::getStringDateNow() {
    string time = getStringTimeFullNow();
    return time.substr(0, 10);
}

unsigned short ptime::year() {
    return this->time.ushYear;
}

unsigned char ptime::mon() {
    return this->time.uchMonth;
}

unsigned char ptime::day() {
    return this->time.uchDay;
}

unsigned char ptime::hour() {
    return this->time.uchHour;
}

unsigned char ptime::minute() {
    return this->time.uchMinute;
}

unsigned char ptime::second() {
    return this->time.uchSecond;
}

unsigned short ptime::msecond() {
    return this->time.ushMSecond;
}

double ptime::getDiff(clock_t t1, clock_t t2) {
    return ((double) (t1 - t2)) / CLOCKS_PER_SEC;
}

ptime ptime::getTimeFromSeconds(int64_t secs) {
    return ptime(clib_getTimeFromSeconds(secs));
}

int64_t ptime::getSecondsFrom1970() {
    return clib_getNowSeconds();
}

//2000-01-01 00:00:00.000
int64_t ptime::getSecondsFromString(string strTime) {
    xctime ct = clib_getTimeFromString(strTime.c_str());
    return clib_getSecondsFromXtime(ct);
}

int ptime::getSecondsFrom2000() {
    return getSecondsFromString("2000-01-01 00:00:00.000");
}

//获取2000开始的日期时间，要加上1970到2000的差才行
ptime ptime::getTimeFromSeconds2000(int secs) {
    int64_t icha = ptime::getSecondsFrom1970() - ptime::getSecondsFrom2000();
    int64_t iTotalFrom1970 = secs + icha;
    return ptime::getTimeFromSeconds(iTotalFrom1970);
}

unsigned int ptime::getJS() {
    ptime tmnow;
    string strdate = tmnow.toStringDate();
//    hlog(strdate);
    string timeres = strdate + " 00:00:00.000";
//    hlog(timeres);
    ptime tmorign(timeres);

    double dbcha = tmnow - tmorign;
    return dbcha * 10000;
    //    ptime tmorigin()
}

//从积秒获取时间HH:MM:SS
string ptime::getTimeFromJS(int js) {
    int hour = js / 36000000;
//    hlog(hour);
    int min = (js - hour * 36000000) / 600000;
//    hlog(min);
    int sec = (js - hour * 36000000 - min * 600000) / 10000;
//    hlog(sec);
    char acres[30];
    cinit(acres);
    sprintf(acres, "%02d:%02d:%02d", hour, min, sec);
//    hlog(acres);
    return string(acres);
}


//IsLeap函数判断一个年份是否为闰年，方法如下:
bool ptime::isLeap(int year) {
    return (year % 4 == 0 || year % 400 == 0) && (year % 100 != 0);
}

//上面的StringToDate函数用于取出日期中的年月日并判断日期是否合法
//从字符中最得年月日 规定日期的格式是yyyy-mm-dd
bool ptime::StringToDate(string date, int &year, int &month, int &day) {
    year = atoi((date.substr(0, 4)).c_str());
    month = atoi((date.substr(5, 2)).c_str());
    day = atoi((date.substr(8, 2)).c_str());
    int DAY[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeap(year)) {
        DAY[1] = 29;
    }
    return year >= 0 && month <= 12 && month > 0 && day <= DAY[month] && day > 0;
}

//DayInYear能根据给定的日期，求出它在该年的第几天，代码如下
int ptime::getDayInYear(int year, int month, int day) {
    //int _day = 0;
    int DAY[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeap(year))
        DAY[1] = 29;
    for (int i = 0; i < month - 1; ++i) {
        day += DAY[i];
    }
    return day;
}

int ptime::getDaysBetween2Date(string date1, string date2) {
    //取出日期中的年月日
    int year1, month1, day1;
    int year2, month2, day2;
    if (!StringToDate(date1, year1, month1, day1) || !StringToDate(date2, year2, month2, day2)) {
        cout << "输入的日期格式不正确";
        return -1;
    }
    if (year1 == year2 && month1 == month2) {
        return day1 > day2 ? day1 - day2 : day2 - day1;

        //如果年相同
    } else if (year1 == year2) {
        int d1, d2;
        d1 = getDayInYear(year1, month1, day1);
        d2 = getDayInYear(year2, month2, day2);
        return d1 > d2 ? d1 - d2 : d2 - d1;

        //年月都不相同
    } else {
        //确保year1年份比year2早
        if (year1 > year2) {
            //swap进行两个值的交换
            swap(year1, year2);
            swap(month1, month2);
            swap(day1, day2);
        }
        int d1, d2, d3;
        if (isLeap(year1))
            d1 = 366 - getDayInYear(year1, month1, day1); //取得这个日期在该年还于下多少天
        else
            d1 = 365 - getDayInYear(year1, month1, day1);
        d2 = getDayInYear(year2, month2, day2); //取得在当年中的第几天
        cout << "d1:" << d1 << ", d2:" << d2;

        d3 = 0;
        for (int year = year1 + 1; year < year2; year++) {
            if (isLeap(year))
                d3 += 366;
            else
                d3 += 365;
        }
        return d1 + d2 + d3;
    }
}

//这是积日，军队项目要求从2000开始，用2字节存储
unsigned short ptime::getJD() {
    ptime tnow;
    string date = tnow.toStringDate();
    return getDaysBetween2Date(date, "2000-01-01");
}

pstring ptime::getStringTimeFullNowNoSplit() {
    return plib::getTimeNowNoSplit();
}

pstring ptime::getStringTimeFullNowNoSplitNoMs() {
    ptime tnow;
    pstring info=plib::toString(tnow);
    pliststring lres=info.split("- :.");
    pstring stres=lres.join("");
    return stres.substr(0,stres.size()-3);
}

pstring ptime::getStringTimeNowNoSplit() {
    pstring info= ptime::getStringTimeNow();
    pliststring lres= info.split(":.");
    return lres.join("");
}

pstring ptime::getStringTimeNowNoSplitNoMs() {
    pstring info= ptime::getStringTimeNowNoMs();
    pliststring lres=info.split(":");
    return lres.join("");
}

pstring ptime::getStringDateNowNoSplit() {
    pstring info= ptime::getStringDateNow();
    pliststring lres=info.split("-");
    return lres.join("");
}


pdir::pdir(const char *path) {
    init(path);
}

int pdir::init(const char *path) {
    this->isValid = false;
    //    hlog(path);
    this->pcdir = cdirInit(path);

    if (this->pcdir == NULL) {
        hlog(pstring() << "cdirInit fail:" << path);
        return -1;
    }
    this->isValid = true;
    this->strPath = string(path);

    for (int i = 0; i < cvstring_size(pcdir->vstrNamesAll); i++) {
        cstring *pstrName = pcdir->vstrNamesAll->at(pcdir->vstrNamesAll, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listNameAll.push_back(string(acName));
    }

    for (int i = 0; i < pcdir->vstrNamesFile->size(pcdir->vstrNamesFile); i++) {
        cstring *pstrName = pcdir->vstrNamesFile->at(pcdir->vstrNamesFile, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listNameFile.push_back(string(acName));
    }

    for (int i = 0; i < pcdir->vstrNamesDir->size(pcdir->vstrNamesDir); i++) {
        cstring *pstrName = pcdir->vstrNamesDir->at(pcdir->vstrNamesDir, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listNameDir.push_back(string(acName));
    }


    for (int i = 0; i < pcdir->vstrPathsAll->size(pcdir->vstrPathsAll); i++) {
        cstring *pstrName = pcdir->vstrPathsAll->at(pcdir->vstrPathsAll, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listPathAll.push_back(string(acName));
    }

    for (int i = 0; i < pcdir->vstrPathsFile->size(pcdir->vstrPathsFile); i++) {
        cstring *pstrName = pcdir->vstrPathsFile->at(pcdir->vstrPathsFile, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listPathFile.push_back(string(acName));
    }
    for (int i = 0; i < pcdir->vstrPathsDir->size(pcdir->vstrPathsDir); i++) {
        cstring *pstrName = pcdir->vstrPathsDir->at(pcdir->vstrPathsDir, i + 1);
        char acName[256];
        bzero(acName, 256);
        pstrName->toStringFix(pstrName, acName);
        this->listPathDir.push_back(string(acName));
    }


    plist<long> listDisk = plib::getDiskUseInfo(path);


    this->lTotal = listDisk[0];
    this->lUse = listDisk[1];
    this->dbUsePercent = 1.0 * this->lUse / lTotal;
    return 0;
}

pdir::pdir(const pdir &pdircp) {
    init(pdircp.strPath.c_str());
}

pdir::~pdir() {
    cdirDestroy(this->pcdir);
    //    pcdir->destroy(pcdir);
}


void printString(pstring str) {
    hlog(str.c_str());
}

