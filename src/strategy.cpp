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
	robot_radius = 8.0;
	distance_to_stop = 5.0;
	changePose = true;
	srand(time(NULL));
}

void Strategy::init(string main_color, bool is_debug, bool real_environment){
	init_sample(main_color, is_debug, real_environment);
	loop();
}

void Strategy::loop(){
	while(true){
		// DON'T REMOVE receive_data();
		receive_state();
		// DON'T REMOVE receive_Data();'
		
		calc_strategy();
		
		if(!real_environment){
			// DON'T REMOVE send_data();
			send_commands();
			// DON'T REMOVE send_data();
		}else{
			// Put your transmission code here
		}

		// DON'T REMOVE
		if(is_debug)
			send_debug();
		// DON'T REMOVE'
	}
}

void Strategy::calc_strategy(){
	if(changePose){
		changePose = false;
		final.x = (rand() % 100) + 30;
		final.y = (rand() % 80) + 30;
		final.z = rand() % 360;
	}

	commands[0] = calc_cmd_to(state.robots[0].pose, final, distance_to_stop);
	//state.robots[0].pose.show();
	// commands[1]
	// commands[2]
	debug.robots_final_pose[0] = final;

	for(int i = 0 ; i < 3 ; i++){
		debug.robots_path[i].poses.clear();
	}
	
	debug.robots_path[0].poses.push_back(state.robots[0].pose);
	debug.robots_path[0].poses.push_back(final);
}

common::Command Strategy::calc_cmd_to(btVector3 act, btVector3 goal, float distance_to_stop){
	Command cmd;
	float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	// Diferença entre angulação do robô e do objetivo
	distance_robot_goal = distancePoint(goal, act);
	angulation_robot_goal = angulation(goal, act);


	angulation_robot_goal -= 180; // 180 if comes from VSS-Simulator
    if(angulation_robot_goal < 0){
    	angulation_robot_goal += 360;
    }

	angulation_robot_robot_goal = act.z - angulation_robot_goal;

	if(angulation_robot_robot_goal > 180){
		angulation_robot_robot_goal -= 360;
	}

	if(angulation_robot_robot_goal < -180){
		angulation_robot_robot_goal += 360;
	}
	
	//cout << angulation_robot_robot_goal << endl;

	// Regras de movimentação
	if(fabs(angulation_robot_robot_goal) <= 135){
		cmd.left = distance_robot_goal - 0.2*(angulation_robot_robot_goal * robot_radius / 2.00);
		cmd.right = distance_robot_goal + 0.2*(angulation_robot_robot_goal * robot_radius / 2.00);
		
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

	//cmd.left = 1;
	//cmd.right = -1;
	//cmd.left e cmd.right são PWM (0 a 255 para frente) (256 á 252 para trás)

	if(distance_robot_goal < 15.0){
		cmd.left = 0;
		cmd.right = 0;
		changePose = true;
	}

	return cmd;
}