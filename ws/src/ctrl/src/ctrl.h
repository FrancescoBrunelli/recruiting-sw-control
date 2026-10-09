#ifndef __CTRL_H__
#define __CTRL_H__
#include "../can/eagle_task.h"

struct VehicleStatus {
    // false = 0; true = 1
    bool throttle;
    bool brake;
    bool mission_finished;
    double vehicle_speed;

    VehicleStatus() {
        throttle = false;
        brake = false;
        mission_finished = false;
        vehicle_speed = 0;
    }

    VehicleStatus(bool t, bool b, bool m, double v) {
        throttle = t;
        brake = b;
        mission_finished = m;
        vehicle_speed = v;
    }
};

#endif
