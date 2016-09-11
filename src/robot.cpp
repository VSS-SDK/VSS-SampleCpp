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
}

void Robot::calc_action(){
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

/*void Robot::check_need_to_replan_path(){
	switch(task){
		case Task::GOAL_KEEPER:{
			GK_check_need_to_replan_path();
		}break;
		case Task::DEFENDER:{
			DF_check_need_to_replan_path();
		}break;
		case Task::ATTACKER:{
			AT_check_need_to_replan_path();
		}break;
	}
}*/

// MODEL BLAZIC
void Robot::calc_cmd_to(){
    float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	// Diferença entre angulação do robô e do objetivo
	distance_robot_goal = distancePoint(step_pose, pose);
	angulation_robot_goal = angulation(step_pose, pose);

	angulation_robot_goal -= 180; 
    if(angulation_robot_goal < 0){
    	angulation_robot_goal += 360;
    }

	angulation_robot_robot_goal = pose.z - angulation_robot_goal;

	if(angulation_robot_robot_goal > 180){
		angulation_robot_robot_goal -= 360;
	}

	if(angulation_robot_robot_goal < -180){
		angulation_robot_robot_goal += 360;
	}

	if(fabs(angulation_robot_robot_goal) >= angle_to_spin){
		front = false;
	}else{
		front = true;
	}
	
	/*if(contIntegrative < deltaTimeIntegrative){
		errorsIntegrative.push_back(angulation_robot_robot_goal);
		contIntegrative++;
	}else{
		vector<float> tempIntegrative(errorsIntegrative.size()-1);
		std::copy(errorsIntegrative.begin(), errorsIntegrative.end()-1, tempIntegrative.begin());
		tempIntegrative.push_back(angulation_robot_robot_goal);
		errorsIntegrative = tempIntegrative;
	}

	float sumError = 0;
	for(int i = 0; i < errorsIntegrative.size();i++){
		sumError += errorsIntegrative.at(i);
	}*/

	if(front){
		float PI = 0.008*angulation_robot_robot_goal;// + 0.001*sumError;

		if(fabs(angulation_robot_robot_goal) < angle_to_spin){
			cmd.left = distance_robot_goal - (PI * robot_side_size);
			cmd.right = distance_robot_goal + (PI * robot_side_size);
			
			cmd.left *= 3.0;
			cmd.right *= 3.0;
		}else{
			// SPIN
			if(angulation_robot_robot_goal >= 0){
				cmd.left = -50;
				cmd.right = 50;
			}else{
				cmd.left = 50;
				cmd.right = -50;
			}
		}
	}else{
		if(angulation_robot_robot_goal < 0){
			angulation_robot_robot_goal += 180;
		}else{
			angulation_robot_robot_goal -= 180;		
		}

		float PI = 0.008*angulation_robot_robot_goal;// + 0.001*sumError;

		if(fabs(angulation_robot_robot_goal) < angle_to_spin){
			cmd.left = distance_robot_goal + (PI * robot_side_size);
			cmd.right = distance_robot_goal - (PI * robot_side_size);
			
			cmd.left *= -3.0;
			cmd.right *= -3.0;
		}else{
			// SPIN
			if(angulation_robot_robot_goal >= 0){
				cmd.left = 50;
				cmd.right = -50;
			}else{
				cmd.left = -50;
				cmd.right = 50;
			}
		}
	}

	if(distancePoint(pose, final_pose) < distance_to_stop){
		cmd.left = 0;
		cmd.right = 0;
		debug_pos = true;
	}
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