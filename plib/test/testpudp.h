//
// Created by Administrator on 2020/12/24.
//

#ifndef PLIB_TESTPUDP_H
#define PLIB_TESTPUDP_H
#include "../pudp.h"
void thread_recv(ptcp* p)
{
    while(1)
    {
        presult res=p->recvz();
        if(res.res<0)
            break;
        hlog(p->type);
        if(p->type=="xlfd")
        {
            double dbrecv=p->getClassFromPresult<double>(res);
            hlog(dbrecv);
        }
    }
    hlog("exit");
}

void testpudp()
{
    pudp udp;
    udp.init(3333);
    hlog(udp.bind("224.0.0.4"));
    udp.setHostDES("224.0.0.4");
    hlog(udp.sendx("test",1.2345));
    hlog(udp.recvx());


    hlog(udp.sendx("type",pliststring()<<"xlfd"<<"zero"));
    presult res=(udp.recvx());
    hlog(udp.getClassFromPresult<pliststring>(res));

    double db=3.333;
    hlog(udp.send(db));
    pstring strdata;
    hlog(udp.recv(strdata));
    hlog(strdata.size());
    double dbrecv=*(double*)(strdata.data());
    hlog(dbrecv);

    pstring datatest="3";
    hlog(datatest);
    datatest.resize(4);
    strcpy((char*)datatest.data(),"xlfd");
    hlog(datatest);


    while(1)
    {
        plib::sleep(1000);
    }
}
#endif //PLIB_TESTPUDP_H
