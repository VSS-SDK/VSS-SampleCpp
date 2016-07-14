/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "sstream"
#include "strategy.h"

int main(int argc, char** argv){
	if(argc > 1){
		stringstream ss; 
		int port;

		ss << argv[1];
		ss >> port;

		Strategy strategy;
		strategy.init(port);
	}else{
		cerr << "Please enter the port" << endl;
	}

	//Interface interface;
	//vss_state::Global_State global_state;
	/*interface_receive.createSocketReceiveState(&global_state);

	while(true){
		interface_receive.receiveState();
	}*/

	/*if(argc <= 1){
		Interface interface;
		vss_command::Global_Commands global_commands;

		global_commands.set_id(0);
		global_commands.set_is_team_yellow(true);

		for(int i = 0 ; i < 3 ; i++){
			vss_command::Robot_Command *robot = global_commands.add_robot_commands();
			robot->set_id(i);
			robot->set_left_vel(100);
			robot->set_right_vel(100);
		}

		interface.createLoopSendCommandsYellow(&global_commands);
	}else{
		Interface interface;
		vss_command::Global_Commands global_commands;

		global_commands.set_id(0);
		global_commands.set_is_team_yellow(false);

		for(int i = 0 ; i < 3 ; i++){
			vss_command::Robot_Command *robot = global_commands.add_robot_commands();
			robot->set_id(i);
			robot->set_left_vel(55);
			robot->set_right_vel(55);
		}

		interface.createLoopSendCommandsBlue(&global_commands);
	}*/

	return 0;
}