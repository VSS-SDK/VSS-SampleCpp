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
	robot_radius = 8.0;
	distance_stop = 5.0;
	situation = 0;
	srand(time(NULL));
	goal_glob.x = (rand() % 110) + 20;
	goal_glob.y = (rand() % 100) + 20;
}

void Strategy::init(string main_color){
	this->main_color = main_color;

	thread_receive = new thread(bind(&Strategy::receive_thread, this));
	//thread_send = new thread(bind(&Strategy::send_thread, this));

	thread_receive->join();
	//thread_send->join();
}

void Strategy::receive_thread(){
	interface_receive.createSocketReceiveState(&global_state);

	if(main_color == "yellow"){
		interface_send.createSendCommandsTeam1(&global_commands);
	}else{
		interface_send.createSendCommandsTeam2(&global_commands);
	}

	while(true){
		// Only loop if has a new state
		interface_receive.receiveState();
		state = common::Global_State2State(global_state, main_color);
		situation = global_state.situation();
		has_new_state = true;

		calc_strategy();
		has_new_state = false;
		if(main_color == "yellow"){
			interface_send.sendCommandTeam1();
		}else{
			interface_send.sendCommandTeam2();
		}	
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
		if(main_color == "yellow"){
			interface_send.createSendCommandsTeam1(&global_commands);
		}else{
			interface_send.createSendCommandsTeam2(&global_commands);
		}

		while(true){
			if(has_new_state){
				calc_strategy();
				has_new_state = false;
				if(main_color == "yellow"){
					interface_send.sendCommandTeam1();
				}else{
					interface_send.sendCommandTeam2();
				}
			}else{
				usleep(33333);
			}

		}
	}

}

void Strategy::calc_strategy(){
	bool all_robots_ok = false;

	// PACK COMMAND
	global_commands = vss_command::Global_Commands();

	if(situation == 0){
		play();
		global_commands.set_situation(NONE); 
	}else{
		all_robots_ok = position_our_goal();

		if(all_robots_ok){
			global_commands.set_situation(NONE); 
		}else{
			global_commands.set_situation(GOAL_TEAM1); 
		}
	}

	if(main_color == "yellow"){
		global_commands.set_is_team_yellow(true);	// IF IS YELLOW: TRUE
	}else{
		global_commands.set_is_team_yellow(false);
	}

	//commands[0].show();

	for(int i = 0 ; i < 3 ; i++){
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(commands[i].left);
		robot->set_right_vel(commands[i].right);
	}
}

void Strategy::play(){
	float act_distance_to_proj;
	btVector3 projection;

	/*if(main_color == "yellow"){
		projection = project_bt_to(state.ball, btVector3(150.0, 75, 0), distance_stop);
	}else{
		projection = project_bt_to(state.ball, btVector3(0.0, 55, 0), distance_stop);
	}*/

	act_distance_to_proj = fabs(distancePoint(state.robots[0].pose, goal_glob));
	//cout << "distance: " << act_distance_to_proj << endl;

	if(act_distance_to_proj > distance_stop + robot_radius){
		commands[0] = calc_cmd_to(state.robots[0].pose, goal_glob, distance_stop);
	}else{
		goal_glob.x = (rand() % 110) + 20;
		goal_glob.y = (rand() % 100) + 20;
	}
}

bool Strategy::position_our_goal(){
	bool all_robots_ok = false;

	commands[0] = calc_cmd_to(state.robots[0].pose, btVector3(50, 65, 0), distance_stop);
	
	if(commands[0].left == 0 && commands[0].right == 0){
		all_robots_ok = true;
	}
	
	return all_robots_ok;
}


common::btVector3 Strategy::project_bt_to(btVector3 ball, btVector3 goal, float proj_distance){
	btVector3 projection;
	float theta, distance;

	distance = distancePoint(ball, goal);
	theta = radian(goal, ball);

	projection.x = ball.x - cos(theta)*proj_distance;
	projection.y = ball.y - sin(theta)*proj_distance;

	return projection;
}

common::Command Strategy::calc_cmd_to(btVector3 act, btVector3 goal, float distance_to_stop){
	Command cmd;
	float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	distance_robot_goal = distancePoint(goal, act);
	angulation_robot_goal = angulation(goal, act);

	//cout << "act: " << act.z << endl;
	//cout << "ang_robot_goal: " << angulation_robot_goal << endl;

	angulation_robot_goal -= 180; // 180 if comes from VSS-Simulator

    if(angulation_robot_goal < 0){
    	angulation_robot_goal += 360;
    }

	//cout << "ang_robot_goal: " << angulation_robot_goal << endl;

	int angles = (int)fabs(act.z - angulation_robot_goal) % 360;
	int dist = angles > 180 ? 360 - angles : angles*-1;

	//cout << dist << endl;
	angulation_robot_robot_goal = dist;

	cout << angulation_robot_robot_goal << endl;
	//cout << "diff: " << angulation_robot_robot_goal << endl;

	if(fabs(angulation_robot_robot_goal) <= 135){
		cmd.left = distance_robot_goal + 0.2*(angulation_robot_robot_goal * robot_radius / 2.00);
		cmd.right = distance_robot_goal - 0.2*(angulation_robot_robot_goal * robot_radius / 2.00);
		
		cmd.left *= 0.3;
		cmd.right *= 0.3;
	}else{
		if(angulation_robot_robot_goal >= 0){
			cmd.left = 50;
			cmd.right = -50;
		}else{
			cmd.left = -50;
			cmd.right = 50;
		}
	}

	if(distance_robot_goal < distance_to_stop){
		cmd.left = 0;
		cmd.right = 0;
	}

	return cmd;
}