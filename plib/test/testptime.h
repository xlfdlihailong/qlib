//
// Created by Administrator on 2020/12/24.
//

#ifndef PLIB_TESTPTIME_H
#define PLIB_TESTPTIME_H
#include "../plib.h"
void testptime()
{
    ptime tnow("2020-12-23 11:22:33.111");
    hlog(tnow);
    ptime t2("2020-12-23 11:22:33");
    hlog(t2);
    ptime t3("20200101112233999");
    hlog(t3);
    ptime t4("20200101112233");
    hlog(t4);
    hlog(ptime::getStringTimeNow());
    hlog(ptime::getStringTimeNowNoSplit());
    hlog(ptime::getStringTimeNowNoSplitNoMs());
    hlog(ptime::getStringDateNowNoSplit());

    hlog(t3.toStringTimeFull());
    hlog(t3.toStringDate());
    hlog(t3.toStringDateNoSplit());
    hlog(t3.toStringTime());
    hlog(t3.toStringTimeNoMs());
    hlog(t3.toStringTimeNoSplit());
    hlog(t3.toStringTimeNoSplitNoMs());
    hlog(t3.toStringTimeFullNoMs());
    hlog(t3.toStringTimeFullNoSplit());
    hlog(t3.toStringTimeFullNoSplitNoMs());
}
#endif //PLIB_TESTPTIME_H
