/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "strategy.h"

Strategy::Strategy(){
	its_real_transmition = false;
	has_new_state = has_new_command = false;
}

void Strategy::init(int port){
	this->port = port;

	thread_receive = new thread(bind(&Strategy::receive_thread, this));
	thread_send = new thread(bind(&Strategy::send_thread, this));

	thread_receive->join();
	thread_send->join();
}

void Strategy::receive_thread(){
	interface_receive.createSocketReceiveState(&global_state);

	while(true){
		// Only loop if has a new state
		interface_receive.receiveState();
		has_new_state = true;
	}
}

void Strategy::send_thread(){
	if(its_real_transmition){
		// SEND INFO FOR ROBOTS
		while(true){
			if(has_new_state){
				has_new_state = false;
			}else{
				usleep(33333);
			}
		}
	}else{
		interface_send.createSendCommandsTeam1(&global_commands);
		global_commands.set_id(0);
		global_commands.set_is_team_yellow(true);

		while(true){
			if(has_new_state){
				calc_strategy();
				has_new_state = false;

				interface_send.sendCommandTeam1();
			}else{
				usleep(33333);
			}

		}
	}

}

void Strategy::calc_strategy(){
	float robot_x, robot_y, robot_yaw;
	float ball_x, ball_y;
	float distance_robot_ball;
	float angulation_robot_ball;
	float angulation_robot_robot_ball;

	ball_x = global_state.balls(0).pose().x();
	ball_y = global_state.balls(0).pose().y();

	robot_x = global_state.robots_yellow(0).pose().x();
	robot_y = global_state.robots_yellow(0).pose().y();
	robot_yaw = global_state.robots_yellow(0).pose().yaw();		// RADIANS

	robot_yaw = robot_yaw * (180.0/M_PI);	// CONVERT TO DEGREES
	                                     	
	robot_yaw -= 180; // 180 if comes from VSS-Simulator

	if(robot_yaw < 0){
		robot_yaw += 360;
	}

	distance_robot_ball = distance(robot_x, robot_y, ball_x, ball_y);
	angulation_robot_ball = angulation(robot_x, robot_y, ball_x, ball_y);

	angulation_robot_ball = angulation_robot_ball * (180.0/M_PI);	// CONVERT TO DEGREES
	                                     	
	//angulation_robot_ball -= 45;

	if(angulation_robot_ball < 0){
		angulation_robot_ball += 360;
	}

	angulation_robot_robot_ball = robot_yaw - angulation_robot_ball;

	cout << angulation_robot_robot_ball << endl;

	float left_vel, right_vel;
	float eixoRobo = 8.0;

	left_vel = distance_robot_ball - (angulation_robot_robot_ball * eixoRobo / 2.00);
	right_vel = distance_robot_ball + (angulation_robot_robot_ball * eixoRobo / 2.00);
	
	left_vel *= 0.3;
	right_vel *= 0.3;

	if(fabs(angulation_robot_robot_ball) < 8 && distance_robot_ball < 10){
		left_vel = 0;
		right_vel = 0;
	}

	cout << left_vel << " - " << right_vel << endl;
	
	global_commands = vss_command::Global_Commands();

	global_commands.set_is_team_yellow(true);
	vss_command::Robot_Command *robot = global_commands.add_robot_commands();
	robot->set_id(0);
	robot->set_left_vel(left_vel);
	robot->set_right_vel(right_vel);

	for(int i = 1 ; i < 3 ; i++){
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(0);
		robot->set_right_vel(0);
	}
}

float Strategy::distance(float r_x, float r_y, float b_x, float b_y){
	return sqrt(((r_x - b_x)*(r_x - b_x)) + ((r_y - b_y)*(r_y - b_y)));
}

float Strategy::angulation(float r_x, float r_y, float b_x, float b_y){
	return atan2(r_y - b_y, r_x - b_x);
}

// HOW GET VALUES FROM PROTOBUF GLOBAL_STATE
// Ball state
/*global_state.balls(0).pose().x();			// Pos X
global_state.balls(0).pose().y();			// Pos Y

global_state.balls(0).v_pose().x();			// Vel X
global_state.balls(0).v_pose().y();			// Vel Y

global_state.balls(0).k_pose().x();			// Kalman Pos X
global_state.balls(0).k_pose().y();			// kalman Pos Y

global_state.balls(0).k_v_pose().x();		// Kalman Vel X
global_state.balls(0).k_v_pose().y();		// Kalman Vel Y

// Robots state
for(int i = 0 ; i < 3 ; i++){
	// Yellow Robots POSE
	global_state.robots_yellow(i).pose().x();			// Pos X
	global_state.robots_yellow(i).pose().y();			// Pos Y
	global_state.robots_yellow(i).pose().yaw();			// Rotation in Z Axis (YAW)

	// Yellow Robots VELOCITYS
	global_state.robots_yellow(i).v_pose().x();			// Vel X
	global_state.robots_yellow(i).v_pose().y();			// Vel Y
	global_state.robots_yellow(i).v_pose().yaw();		// Vel Rotation in Z Axis (YAW)

	// Yellow Robots Kalman predict POSE
	global_state.robots_yellow(i).k_pose().x();			// Kalman Pos X
	global_state.robots_yellow(i).k_pose().y();			// Kalman Pos Y
	global_state.robots_yellow(i).k_pose().yaw();		// Kalman Rotation in Z Axis (YAW)

	// Yellow Robots Kalman predict VELOCITYS
	global_state.robots_yellow(i).k_v_pose().x();		// Kalman Vel X
	global_state.robots_yellow(i).k_v_pose().y();		// Kalman Vel Y
	global_state.robots_yellow(i).k_v_pose().yaw();		// Kalman Vel Rotation in Z Axis (YAW)

	// Yellow Robots COLOR LABEL (RGB)
	global_state.robots_yellow(i).color().r();			// R
	global_state.robots_yellow(i).color().g();			// G
	global_state.robots_yellow(i).color().b();			// B
	


	// Blue Robots POSE
	global_state.robots_blue(i).pose().x();				// Pos X
	global_state.robots_blue(i).pose().y();				// Pos Y
	global_state.robots_blue(i).pose().yaw();			// Rotation in Z Axis (YAW)

	// Blue Robots VELOCITYS
	global_state.robots_blue(i).v_pose().x();			// Vel X
	global_state.robots_blue(i).v_pose().y();			// Vel Y
	global_state.robots_blue(i).v_pose().yaw();			// Vel Rotation in Z Axis (YAW)

	// Blue Robots Kalman predict POSE
	global_state.robots_blue(i).k_pose().x();			// Kalman Pos X
	global_state.robots_blue(i).k_pose().y();			// Kalman Pos Y
	global_state.robots_blue(i).k_pose().yaw();			// Kalman Rotation in Z Axis (YAW)

	// Blue Robots Kalman predict VELOCITYS
	global_state.robots_blue(i).k_v_pose().x();			// Kalman Vel X
	global_state.robots_blue(i).k_v_pose().y();			// Kalman Vel Y
	global_state.robots_blue(i).k_v_pose().yaw();		// Kalman Vel Rotation in Z Axis (YAW)

	// Blue Robots COLOR LABEL (RGB)
	global_state.robots_blue(i).color().r();			// R
	global_state.robots_blue(i).color().g();			// G
	global_state.robots_blue(i).color().b();			// B
}*/