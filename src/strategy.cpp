/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "strategy.h"

Strategy::Strategy(){
    main_color = "yellow";
    is_debug = false;
    real_environment = false;

	srand(time(NULL));

	for(int i = 0 ; i < 3 ; i++){
		Robot r;
		r.set_id(i);
		r.alloc_ball(&ball, &v_ball);
		our_team.push_back(r);
		adversary_team.push_back(r);
	}

	our_team.at(0).set_task(Task::ATTACKER);
	our_team.at(1).set_task(Task::DEFENDER);
	our_team.at(2).set_task(Task::DEFENDER);

	for(int i = 0 ; i < 3 ; i++){
		our_team.at(i).alloc_our_team(&our_team);
		our_team.at(i).alloc_adversary_team(&adversary_team);
	}
}

void Strategy::init(string main_color, bool is_debug, bool real_environment, Goal goal){
	init_sample(main_color, is_debug, real_environment);

	for(int i = 0 ; i < our_team.size(); i++){
		our_team.at(i).set_goal(goal);
	}

	loop();
}

void Strategy::loop(){
	while(true){
		// DON'T REMOVE receive_data();
		receive_state();
		// DON'T REMOVE receive_Data();
		
		// DON'T REMOVE update_state_on_robots();
		update_state_on_robots();
		// DON'T REMOVE update_state_on_robots();

		calc_strategy();

		// DON'T REMOVE update_commands_from_robots();
		update_commands_from_robots();
		// DON'T REMOVE update_commands_from_robots();
		
		
		if(!real_environment){
			// DON'T REMOVE send_data();
			send_commands();
			// DON'T REMOVE send_data();
		}else{
			for(int i = 0 ; i < 3 ; i++){
				commands[i] = our_team.at(i).get_command();
				commands[i].left = commands[i].left * 3.5;
				commands[i].right = commands[i].right * 3.5;

				if(commands[i].left < 0){
					commands[i].left = 255 + fabs(commands[i].left);
				}
				if(commands[i].right < 0){
					commands[i].right = 255 + fabs(commands[i].right);
				}

				commands[i].left = (int)commands[i].left;
				commands[i].right = (int)commands[i].right;
			}

			comm.sendSerialData(commands);
		}

		// DON'T REMOVE debug_mode
		if(is_debug){
			update_debug_from_robots();
			send_debug();
		}
		// DON'T REMOVE debug_mode
	}
}

void Strategy::calc_strategy(){
	for(int i = 0 ; i < 3 ; i++){
		our_team.at(i).calc_action();
	}
}

void Strategy::update_state_on_robots(){
	ball = state.ball;
	v_ball = state.v_ball;

	for(int i = 0 ; i < 3 ; i++){
		our_team.at(i).set_pose(state.robots[i].pose);
		our_team.at(i).set_v_pose(state.robots[i].v_pose);
		adversary_team.at(i).set_pose(state.robots[i+3].pose);
		adversary_team.at(i).set_v_pose(state.robots[i+3].pose);
	}
}

void Strategy::update_commands_from_robots(){
	for(int i = 0 ; i < 3 ; i++){
		commands[i] = our_team.at(i).get_command();
	}
}

void Strategy::update_debug_from_robots(){
	for(int i = 0 ; i < 3 ; i++){
		debug.robots_step_pose[i] = our_team.at(i).get_step_pose();
        debug.robots_final_pose[i] = our_team.at(i).get_final_pose();
        debug.robots_path[i] = our_team.at(i).get_path();
	}
}