#include "robot.h"

void Robot::AT_calc_action(){
    step_pose = *ball;
    final_pose = step_pose;
    calc_cmd_to();
}