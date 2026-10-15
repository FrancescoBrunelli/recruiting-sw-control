#ifndef __CTRL_H__
#define __CTRL_H__
#include "../can/eagle_task.h"

typedef enum {IDLE, ACCELERATE, BRAKE, FINISH, END} fsmState;

struct VehicleStatus {
    float throttle;
    float brake;
    bool mission_finished;
    double vehicle_speed;

    VehicleStatus() {
        throttle = 0;
        brake = 0;
        mission_finished = false;
        vehicle_speed = 0;
    }

    VehicleStatus(float t, float b, bool m, double v) {
        throttle = t;
        brake = b;
        mission_finished = m;
        vehicle_speed = v;
    }
};

#endif
