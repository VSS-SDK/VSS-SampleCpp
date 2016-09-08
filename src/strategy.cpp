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
	
	for(int i = 3 ; i < 3 ; i++){
		our_team.at(i).alloc_our_team(&our_team);
		our_team.at(i).alloc_adversary_team(&adversary_team);
	}
}

void Strategy::init(string main_color, bool is_debug, bool real_environment){
	init_sample(main_color, is_debug, real_environment);
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
			// Put your transmission code here
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
	//ball.show();
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