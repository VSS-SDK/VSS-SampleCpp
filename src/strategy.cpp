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
	collisions = 0;
	count_collision = 0;
	object_collisions = 0;

	srand(time(NULL));

	for(int i = 0 ; i < 3 ; i++){
		Robot r;
		r.set_id(i);
		r.alloc_ball(&ball, &v_ball);
		our_team.push_back(r);
		adversary_team.push_back(r);
	}

	our_team.at(0).set_task(Task::ATTACKER);
	our_team.at(1).set_task(Task::ATTACKER);
	our_team.at(2).set_task(Task::ATTACKER);

	for(int i = 0 ; i < 3 ; i++){
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
			//cout << "teste" << endl;
			for(int i = 0 ; i < 3 ; i++){
				commands[i] = our_team.at(i).get_command();
				commands[i].left = commands[i].left * 3.0;
				commands[i].right = commands[i].right * 3.0;
				commands[i].show();
				if(commands[i].left < 0){
					commands[i].left = 255 + fabs(commands[i].left);
				}
				if(commands[i].right < 0){
					commands[i].right = 255 + fabs(commands[i].right);
				}
				cout << "plus" << endl;
				commands[i].show();
				cout << endl << endl;
				commands[i].left = (int)commands[i].left;
				commands[i].right = (int)commands[i].right;
			}

			//commands[0].show();

			comm.sendSerialData(commands);
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
	count_collision++;

	if(count_collision > 30){
		count_collision = 0;
		for(int i = 0 ; i < 3 ; i++){
			our_team.at(i).set_collision(false);
			adversary_team.at(i).set_collision(false);
		}
	}

	for(int i = 0 ; i < 3 ; i++){
		our_team.at(i).calc_action();
	}

	for(int i = 0 ; i < 3 ; i++){
		for(int j = i+1 ; j < 3 ; j++){
			if( distancePoint(our_team.at(i).get_pose(), our_team.at(j).get_pose()) < RADIUS_ROBOT*1.8 && !our_team.at(i).get_collision() && !our_team.at(j).get_collision() ){
				collisions++;
				our_team.at(i).set_collision(true);
				our_team.at(j).set_collision(true); 
			}
		}
	}

	for(int i = 0 ; i < 3 ; i++){
		for(int j = 0 ; j < 3 ; j++){
			if( distancePoint(our_team.at(i).get_pose(), adversary_team.at(j).get_pose()) < RADIUS_ROBOT*1.8 && !our_team.at(i).get_collision() && !adversary_team.at(j).get_collision() ){
				object_collisions++;
				our_team.at(i).set_collision(true);
				adversary_team.at(j).set_collision(true); 
			}
		}
	}

	cout << "resolve: " << our_team.at(0).get_resolve_iterator() + our_team.at(1).get_resolve_iterator() + our_team.at(2).get_resolve_iterator() - 3 << endl;
	cout << "robot_collisions: " << collisions << endl;
	cout << "object_collisions: " << object_collisions << endl << endl;
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