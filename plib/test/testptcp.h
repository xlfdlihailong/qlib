//
// Created by Administrator on 2020/12/24.
//

#ifndef PLIB_TESTPTCP_H
#define PLIB_TESTPTCP_H

#include "../ptcp.h"

void thread_recv(ptcp *p) {
    while (1) {
        presult res = p->recvz();
        if (res.res < 0)
            break;
        hlog(p->type);
        if (p->type == "xlfd") {
            double dbrecv = p->getClassFromPresult<double>(res);
            hlog(dbrecv);
        }
    }
    hlog("exit");
}

void testptcp() {
    ptcp ts(plib::getIPLocal(), 3333);
    ts.setThreadServerDetach(thread_recv);
    plib::sleep(1000);
    ptcp tc(plib::getIPLocal(), 3333);
    hlog(tc.connectx());
    double dbsend = 3.1111;
    hlog(tc.sendx("xlfd", dbsend));
    while (1) {
        plib::sleep(1000);
    }
}

#endif //PLIB_TESTPTCP_H
