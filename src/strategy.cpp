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
	if(its_real_transmition){
		// SEND INFO FOR ROBOTS
		while(true){
			if(has_new_state){
				has_new_state = false;

				// Ball state
				global_state.balls(0).y();		// X
            	global_state.balls(0).x();		// Y

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
					global_state.robots_yellow(i).k_pose().x();			// Pos X
					global_state.robots_yellow(i).k_pose().y();			// Pos Y
					global_state.robots_yellow(i).k_pose().yaw();		// Rotation in Z Axis (YAW)

					// Yellow Robots Kalman predict VELOCITYS
					global_state.robots_yellow(i).k_v_pose().x();		// Vel X
					global_state.robots_yellow(i).k_v_pose().y();		// Vel Y
					global_state.robots_yellow(i).k_v_pose().yaw();		// Vel Rotation in Z Axis (YAW)

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
					global_state.robots_blue(i).k_pose().x();			// Pos X
					global_state.robots_blue(i).k_pose().y();			// Pos Y
					global_state.robots_blue(i).k_pose().yaw();			// Rotation in Z Axis (YAW)

					// Blue Robots Kalman predict VELOCITYS
					global_state.robots_blue(i).k_v_pose().x();			// Vel X
					global_state.robots_blue(i).k_v_pose().y();			// Vel Y
					global_state.robots_blue(i).k_v_pose().yaw();		// Vel Rotation in Z Axis (YAW)

					// Blue Robots COLOR LABEL (RGB)
					global_state.robots_blue(i).color().r();			// R
					global_state.robots_blue(i).color().g();			// G
					global_state.robots_blue(i).color().b();			// B
				}
			}else{
				usleep(33333);
			}
		}
	}else{
		// SEND INFO FOR SIMULATOR

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

}