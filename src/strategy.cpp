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
	force_kick = 2.5;
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
		state = common::Global_State2State(global_state);
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
	float distance_stop = 5.0;
	float act_distance_to_proj;

	btVector3 projection;
	projection = project_bt_to(state.ball, btVector3(150.0, 65, 0), distance_stop);

	act_distance_to_proj = distancePoint(state.robots[0].pose, projection);
	cout << act_distance_to_proj << endl;

	if(act_distance_to_proj > distance_stop + robot_radius){
		commands[0] = calc_cmd_to(state.robots[0].pose, projection, distance_stop);
	}else{
		commands[0] = kick_to(state.robots[0].pose, btVector3(150, 75, 0));
		cout << "kick" << endl;
	}
	
	pack_command();
}


common::btVector3 Strategy::project_bt_to(btVector3 ball, btVector3 goal, float proj_distance){
	btVector3 projection;
	float theta, distance;

	distance = distancePoint(ball, goal);
	theta = radian(goal, ball);

	projection.x = ball.x + cos(theta)*proj_distance;
	projection.y = ball.y + sin(theta)*proj_distance;

	//ball.show();
	//projection.show();

	return projection;
}

common::Command Strategy::calc_cmd_to(btVector3 act, btVector3 goal, float distance_to_stop){
	Command cmd;
	float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	distance_robot_goal = distancePoint(act, goal);
	angulation_robot_goal = angulation(act, goal);
											
	if(angulation_robot_goal < 0){
		angulation_robot_goal += 360;
	}

	angulation_robot_robot_goal = act.z - angulation_robot_goal;

	cmd.left = distance_robot_goal - 1.1*(angulation_robot_robot_goal * robot_radius / 2.00);
	cmd.right = distance_robot_goal + 1.1*(angulation_robot_robot_goal * robot_radius / 2.00);
	
	cmd.left *= 0.25;
	cmd.right *= 0.25;

	if(distance_robot_goal < distance_to_stop){
		cmd.left = 0;
		cmd.right = 0;
	}

	force_kick = 0.25;

	return cmd;
}

common::Command Strategy::kick_to(btVector3 act, btVector3 goal){
	Command cmd;
	float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	distance_robot_goal = distancePoint(act, goal);
	angulation_robot_goal = angulation(act, goal);
											
	if(angulation_robot_goal < 0){
		angulation_robot_goal += 360;
	}

	angulation_robot_robot_goal = act.z - angulation_robot_goal;

	cmd.left = distance_robot_goal - 1.1*(angulation_robot_robot_goal * robot_radius / 2.00);
	cmd.right = distance_robot_goal + 1.1*(angulation_robot_robot_goal * robot_radius / 2.00);
	
	cmd.left *= force_kick;
	cmd.right *= force_kick;

	force_kick += 0.1;

	return cmd;
}


void Strategy::pack_command(){
	global_commands = vss_command::Global_Commands();

	global_commands.set_situation(0); 			// ESTAMOS JOGANDO NORMALMENTE
	global_commands.set_is_team_yellow(true);	// IF IS YELLOW: TRUE

	for(int i = 0 ; i < 3 ; i++){
		vss_command::Robot_Command *robot = global_commands.add_robot_commands();
		robot->set_id(i);
		robot->set_left_vel(commands[i].left);
		robot->set_right_vel(commands[i].right);
	}
}