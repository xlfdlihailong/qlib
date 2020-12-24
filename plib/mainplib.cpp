#include "pmysql.h"
#include "ptcp.h"
#include "test/testudp.h"

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
int main() {
//    testudp();
    ptime tnow("2020-12-23 11:22:33.111");
    hlog(tnow);
    ptime t2("2020-12-23 11:22:33");
    hlog(t2);
    ptime t3("20200101112233999");
    hlog(t3);
    ptime t4("20200101112233");
    hlog(t4);
//    ptcp ts(plib::getIPLocal(), 3333);
//    ts.setThreadServerDetach(thread_recv);
//    plib::sleep(1000);
//    ptcp tc(plib::getIPLocal(), 3333);
//    hlog(tc.connectx());
//    double dbsend = 3.1111;
//    hlog(tc.sendx("xlfd", dbsend));
//    while (1) {
//        plib::sleep(1000);
//    }

    return 0;

}