//
// Created by Administrator on 2020/12/21.
//

#ifndef PLIB_TESTUDP_H
#define PLIB_TESTUDP_H
#include "../pudp.h"
#include "../ptcp.h"
void testudp()
{
    plib::killProcessAllByNameSelfExceptSelf();
    ROAD_STATUS_STRUCT road;
    bzero(&road, sizeof(road));
    strcpy(road.arrchRoadNumber, "JOB1111111111");
    strcpy(road.arrchTaskSerialNumber, "DT1111111111111");
    road.shLink = 1;
    road.ulTransferedData = 11111111;
    hlog(road);
    ROAD_STATUS_STRUCT ress = (plib::toClass<ROAD_STATUS_STRUCT>(plib::toString(road)));
    hlog(ress);

    ROAD_STATUS_STRUCT road2;
    bzero(&road2, sizeof(road2));
    strcpy(road2.arrchRoadNumber, "JOB22222222222");
    strcpy(road2.arrchTaskSerialNumber, "DT222222222222");
    road2.shLink = 2;
    road2.ulTransferedData = 222222222222;
    hlog(road2);
    ROAD_STATUS_STRUCT ress2 = (plib::toClass<ROAD_STATUS_STRUCT>(plib::toString(road2)));
    hlog(ress2);
//
//
    TASK_STATUS_STRUCT task;
    bzero(&task, sizeof(task));
    strcpy(task.arrchTaskSerialNumber, "33333 434343 fwf23e");
    task.iRoadNumber = 2;
    task.arrstruRoadStatus[0] = road;
    task.arrstruRoadStatus[1]=road2;
    hlog(task.iRoadNumber);
    hlog(task.arrstruRoadStatus[0]);
    hlog(task.arrstruRoadStatus[1]);
    hlog(task);
    TASK_STATUS_STRUCT tres = plib::toClass<TASK_STATUS_STRUCT>(plib::toString(task));
    hlog(tres);
    hlog(tres.iRoadNumber);
    hlog(tres.arrstruRoadStatus[0]);
    hlog(tres.arrstruRoadStatus[1]);

    pudp udp;
    udp.init(3333);
    hlog(udp.bind("224.0.0.4"));
    udp.setHostDES("224.0.0.4");




    double db=3.333;
    hlog(udp.send(db));
    pstring strdata;
    hlog(udp.recv(strdata));
    hlog(strdata.size());
    double dbrecv=*(double*)(strdata.data());
    hlog(dbrecv);
    //序列化
    pstring datatest="3";
    hlog(datatest);
    datatest.resize(4);
    strcpy((char*)datatest.data(),"xlfd");
    hlog(datatest);

    hlog(udp.send(db));
    presult res=udp.recv();
    hlog(res.res);
    double dbres=*(double*)res.data.data();
    hlog(dbres);

    hlog(udp.sendx("test",pliststring()<<"xlfd"<<"zero"));
    presult res2= udp.recvx();
    hlog(res2.res);
    pliststring lres=udp.getClassFromPresult<pliststring>(res2);
    hlog(lres);
//    hlog(udp.sendx("xxx",32.2323));
//    hlog(udp.recvx(3));
}

#endif //PLIB_TESTUDP_H
