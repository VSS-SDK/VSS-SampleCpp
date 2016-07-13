/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "strategy.h"

Strategy::Strategy(){
	has_new_state = has_new_command = false;
}

void Strategy::init(){
	thread_receive = new thread(bind(&Strategy::receive_thread, this));
	//thread_send = new thread(bind(&Strategy::send_thread, this));

	thread_receive->join();
	//thread_send->join();
}

void Strategy::receive_thread(){
	interface_receive.createSocketReceiveState(&global_state);

	while(true){
		interface_receive.receiveState();
		has_new_state = true;
	}
}

void Strategy::send_thread(){
	/*global_commands.set_id(0);
	global_commands.set_is_team_yellow(true);

	while(true){
		for(int i = 0 ; i < 3 ; i++){
			vss_command::Robot_Command *robot = global_commands.add_robot_commands();
			robot->set_id(i);
			robot->set_left_vel(100);
			robot->set_right_vel(100);
		}	
	}

	interface.createLoopSendCommandsYellow(&global_commands);*/
}