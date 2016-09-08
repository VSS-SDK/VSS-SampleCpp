#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "common.h"
#include "potential_fields.h"

using namespace common;

class Robot{
protected:
    int id;
    float robot_side_size;
    float max_velocity_wheel;
    float max_aceleration_wheel;
    float angle_to_spin;
    float distance_to_stop;

    bool need_brutal_mode;           // Potential Fields OFF
    bool need_to_replan_path;    
    bool front;
    Task task;

    btVector3 pose;
    btVector3 v_pose;
    btVector3 final_pose;
    btVector3 step_pose;
    
    Path path;
    Command cmd;

    vector<Robot> *our_team, *adversary_team;
    btVector3 *ball, *v_ball;

    PotentialFields apf;
    // PID pid;

    void calc_cmd_to();
    void check_opportunities();
    void check_need_to_replan_path();
    void check_need_to_brutal_mode();

    void GK_calc_action();
    void DF_calc_action();
    void AT_calc_action();

    bool debug_pos;
public:
    Robot();

    void alloc_our_team(vector<Robot>*);
    void alloc_adversary_team(vector<Robot>*);
    void alloc_ball(btVector3 *ball, btVector3 *v_ball);

    void set_id(int);
    void set_task(Task);
    void set_pose(btVector3);
    void set_v_pose(btVector3);

    btVector3 get_pose();
    btVector3 get_v_pose();
    btVector3 get_final_pose();
    btVector3 get_step_pose();
    Path get_path();
    Command get_command();

    void calc_action();
};

#endif // _ROBOT_H_