//
// Created by Administrator on 2020/12/24.
//

#ifndef PLIB_TESTSERIALIZATION_H
#define PLIB_TESTSERIALIZATION_H
#include "../plib.h"
void testserialization()
{
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
}

#endif //PLIB_TESTSERIALIZATION_H
