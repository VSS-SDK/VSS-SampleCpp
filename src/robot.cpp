/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "robot.h"

Robot::Robot(){
	id = -1;
    robot_side_size = 8.0;
    max_velocity_wheel = 1000.0;
    max_aceleration_wheel = 1000.0;
    angle_to_spin = 90.0;
	distance_to_stop = 10.0;

    need_brutal_mode = false;   
    front = true;

	task = Task::GOAL_KEEPER;
	debug_pos = true;

	need_to_replan_path = true; 
	distance_between_projections = 0.0;
	deltaTimeIntegrative = 30;
	act_pose_of_path = 1;
	status = 0;
	count_pose = 0;
	rear_count = 0;

	turn_gain = TURN_GAIN_REAL;
	velocity_gain = 2.5;
	iterator_aceleration = 0.0;

	goal[Goal::LEFT] = btVector3(5, 65, 0);
	goal[Goal::RIGHT] = btVector3(165, 65, 0);
    goal_attack = Goal::LEFT;
	goal_defense = Goal::RIGHT;

	attacker_state = AttackerState::AT_GET_BEHIND_THE_BALL;
	defender_state = DefenderState::DF_MARK_THE_BALL;
	goal_keeper_state = GoalKeeperState::GK_MARK_THE_BALL;
}

void Robot::calc_action(){
	//project_ball();
	 if(real_environment){
        turn_gain = TURN_GAIN_REAL;
    }else{
        turn_gain = TURN_GAIN_SIM;
    }
	
	switch(task){
		case Task::GOAL_KEEPER:{
			GK_calc_action();
		}break;
		case Task::DEFENDER:{
			DF_calc_action();
		}break;
		case Task::ATTACKER:{
			AT_calc_action();
		}break;
	}
}

bool Robot::ball_is_on_the_wall(){
	bool on_the_wall = false;

	// Triangulo
	if(ball->x < 15+10 && ball->y < 15){
		on_the_wall = true;
	}

	// Triangulo
	if(ball->x < 15+10 && ball->y > 120){
		on_the_wall = true;
	}

	// Triangulo
	if(ball->x > 150 && ball->y < 15){
		on_the_wall = true;
	}

	// Triangulo
	if(ball->x > 150 && ball->y > 120){
		on_the_wall = true;
	}

	return on_the_wall;
}

btVector3 Robot::generate_free_pose(){
    bool pose_ok = false;
    btVector3 new_pose;

    while(!pose_ok){
        pose_ok = true;

        new_pose.x = (rand() % 120) + 20;
        new_pose.y = (rand() % 90) + 20;
        new_pose.z = rand() % 360;
        
        for(int i = 0 ; i < our_team->size() ; i++){
            if( distancePoint(new_pose, our_team->at(i).pose ) < RADIUS_ROBOT*4.0 ){
                pose_ok = false;
                break;
            }
        }

        if(pose_ok){
            for(int i = 0 ; i < adversary_team->size() ; i++){
                if( distancePoint(new_pose, adversary_team->at(i).pose ) < RADIUS_ROBOT*4.0 ){
                    pose_ok = false;
                    break;
                }
            }
        }
    }

    return new_pose;
}

void Robot::alloc_our_team(vector<Robot> *our_team){
    this->our_team = our_team;
}

void Robot::alloc_adversary_team(vector<Robot> *adversary_team){
    this->adversary_team = adversary_team;
}

void Robot::alloc_ball(btVector3 *ball, btVector3 *v_ball){
    this->ball = ball;
    this->v_ball = v_ball;
}

void Robot::set_id(int id){
	this->id = id;
	switch(id){
		case 0:{
			task = Task::GOAL_KEEPER;
		}break;
		case 1:{
			task = Task::DEFENDER;
		}break;
		case 2:{
			task = Task::ATTACKER;
		}break;
	}
}

void Robot::set_task(Task task){
	this->task = task;
}

void Robot::set_pose(btVector3 pose){
    this->pose = pose;
}

void Robot::set_v_pose(btVector3 v_pose){
    this->v_pose = v_pose;
}

void Robot::set_goal(Goal goal_attack){
	this->goal_attack = goal_attack;
	if(goal_attack == Goal::LEFT){
		goal_defense = Goal::RIGHT;
	}else{
		goal_defense = Goal::LEFT;
	}
}

void Robot::set_real_environment(bool real_environment){
	this->real_environment = real_environment;
	if(real_environment){
		turn_gain = TURN_GAIN_REAL;
	}else{
		turn_gain = TURN_GAIN_SIM;
	}
}

Task Robot::get_task(){
	return task;
}

btVector3 Robot::get_pose(){
	return pose;
}

btVector3 Robot::get_v_pose(){
	return pose;
}

btVector3 Robot::get_final_pose(){
    return final_pose;
}

btVector3 Robot::get_step_pose(){
    return step_pose;
}

Path Robot::get_path(){
    return path;
}

Command Robot::get_command(){
    return cmd;
}