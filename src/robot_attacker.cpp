/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "robot.h"

void Robot::AT_calc_action(){
    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses, *ball);

    AT_may_reach_the_ball_in_time();
    AT_projection();
    
    btVector3 potential = apf.calc_result(id, projection, true, GOTO::POSITION, attacker_state);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;

    if(count_pose >= 30 && status != -1){
		float distance = distancePoint(history_pose, pose);
		if(distance < RADIUS_ROBOT/2.0){
			rear_count = 20;
		}
		history_pose = pose;
		count_pose = 0;
	}

    AT_calc_cmd();
}

void Robot::AT_projection(){
    switch(attacker_state){
        case AttackerState::AT_GET_BEHIND_THE_BALL:{
            //cout << "GET_BEHIND_THE_BALL" << endl;

            btVector3 test_var;
            float theta = radian(goal[goal_attack], *ball);

            test_var.x = ball->x - (cos(theta)*25.0);
            test_var.y = ball->y - (sin(theta)*25.0);

            if(pose.y >= 65){
                test_var.y += 5;
            }else{
                test_var.y -= 5;
            }

            if(test_var.x > 160.0 - RADIUS_ROBOT){
                test_var.x = 160.0 - RADIUS_ROBOT;
            }

            if(test_var.x < 10 + RADIUS_ROBOT){
                test_var.x = 10 + RADIUS_ROBOT;
            }

            if(test_var.y > 130.0 - RADIUS_ROBOT){
                test_var.y = 130.0 - RADIUS_ROBOT;
            }

            if(test_var.y < RADIUS_ROBOT){
                test_var.y = RADIUS_ROBOT;
            }

            if(distancePoint(pose, test_var) >= 15.0){
                projection = test_var;
            }else{
                attacker_state = AttackerState::AT_ADJUST_TO_GET_THE_BALL;
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::AT_ADJUST_TO_GET_THE_BALL:{
            //cout << "ADJUST_TO_GET_THE_BALL" << endl;

            btVector3 test_var;
            float theta = radian(goal[goal_attack], *ball);

            test_var.x = ball->x - (cos(theta)*35.0);
            test_var.y = ball->y - (sin(theta)*35.0);

            if(pose.y >= 65){
                test_var.y -= 5;
            }else{
                test_var.y += 5;
            }


            if(test_var.x > 160.0 - RADIUS_ROBOT){
                test_var.x = 160.0 - RADIUS_ROBOT;
            }

            if(test_var.x < 10 + RADIUS_ROBOT){
                test_var.x = 10 + RADIUS_ROBOT;
            }

            if(test_var.y > 130.0 - RADIUS_ROBOT){
                test_var.y = 130.0 - RADIUS_ROBOT;
            }

            if(test_var.y < RADIUS_ROBOT){
                test_var.y = RADIUS_ROBOT;
            }

            if(distancePoint(pose, test_var) >= 15.0){
                if(!real_environment)
                    turn_gain += 0.015;
                else
                    turn_gain += 0.005;
                projection = test_var;
            }else{
                attacker_state = AttackerState::AT_APPROACH_OF_THE_BALL;
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::AT_APPROACH_OF_THE_BALL:{
            //cout << "APPROACH_OF_THE_BALL" << endl;
            if(goal_attack == Goal::LEFT){
                if(distancePoint(pose, *ball) >= 10.0 && pose.x > ball->x){
                    projection = *ball;
                }else{
                    attacker_state = AttackerState::AT_KICK_THE_BALL;
                }
            }else{
                if(distancePoint(pose, *ball) >= 10.0 && pose.x < ball->x){
                    projection = *ball;
                }else{
                    attacker_state = AttackerState::AT_KICK_THE_BALL;
                }
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::AT_KICK_THE_BALL:{
            //cout << "KICK_THE_BALL" << endl;
            if(goal_attack == Goal::LEFT){
                if(distancePoint(pose, goal[goal_attack]) >= 10.0 && pose.x > ball->x && distancePoint(pose, *ball) <= 15.0) {
                    projection = goal[goal_attack];
                }else{
                    attacker_state = AttackerState::AT_GET_BEHIND_THE_BALL;
                }
            }else{
                if(distancePoint(pose, goal[goal_attack]) >= 10.0 && pose.x < ball->x && distancePoint(pose, *ball) <= 15.0) {
                    projection = goal[goal_attack];
                }else{
                    attacker_state = AttackerState::AT_GET_BEHIND_THE_BALL;
                } 
            }
            if(!real_environment)
                    turn_gain += 0.015;
                else
                    turn_gain += 0.005;
            iterator_aceleration += 0.0035;
            velocity_gain += iterator_aceleration;
            if(velocity_gain > 4.0){
                velocity_gain = 4.0;
            }
        }break;
    }

    final_pose = projection;
}

void Robot::AT_may_reach_the_ball_in_time(){
    // this must not be calculated any time
    time_to_reach_the_ball = (distancePoint(pose, *ball) + RADIUS_BALL + RADIUS_ROBOT) / distancePoint(v_pose, *v_ball);
    ball_in_the_future.x = ball->x + (v_ball->x*time_to_reach_the_ball)/10.0;
    ball_in_the_future.y = ball->y + (v_ball->y*time_to_reach_the_ball)/10.0;
    //cout << "ball: " << endl;
    //ball->show();
    //cout << "future: " << endl;
    //ball_in_the_future.show();
}

// MODEL BLAZIC
void Robot::AT_calc_cmd(){
    float distance_robot_goal;
	float angulation_robot_goal;
	float angulation_robot_robot_goal;

	// Diferença entre angulação do robô e do objetivo
	distance_robot_goal = distancePoint(step_pose, pose);
	angulation_robot_goal = angulation(step_pose, pose);

	angulation_robot_goal -= 180; 
    if(angulation_robot_goal < 0){
    	angulation_robot_goal += 360;
    }

	angulation_robot_robot_goal = pose.z - angulation_robot_goal;

	if(angulation_robot_robot_goal > 180){
		angulation_robot_robot_goal -= 360;
	}

	if(angulation_robot_robot_goal < -180){
		angulation_robot_robot_goal += 360;
	}

	if(fabs(angulation_robot_robot_goal) >= angle_to_spin){
		front = false;
	}else{
		front = true;
	}
	
	/*if(contIntegrative < deltaTimeIntegrative){
		errorsIntegrative.push_back(angulation_robot_robot_goal);
		contIntegrative++;
	}else{
		vector<float> tempIntegrative(errorsIntegrative.size()-1);
		std::copy(errorsIntegrative.begin(), errorsIntegrative.end()-1, tempIntegrative.begin());
		tempIntegrative.push_back(angulation_robot_robot_goal);
		errorsIntegrative = tempIntegrative;
	}

	float sumError = 0;
	for(int i = 0; i < errorsIntegrative.size();i++){
		sumError += errorsIntegrative.at(i);
	}*/

	//turn_gain = TURN_GAIN;// + (1.0 - distancePoint(pose, final_pose)/50.0)/200.0;
	//cout << (1.0 - distancePoint(pose, final_pose)/50.0)/200.0 << endl;
	//cout << turn_gain << endl;
	if(rear_count <= 0){
		if(front){
			float PI = turn_gain*angulation_robot_robot_goal;// + 0.001*sumError;

			if(fabs(angulation_robot_robot_goal) < angle_to_spin){
				cmd.left = distance_robot_goal - (PI * robot_side_size);
				cmd.right = distance_robot_goal + (PI * robot_side_size);
				
				cmd.left *= velocity_gain;
				cmd.right *= velocity_gain;
			}else{
				// SPIN
				if(angulation_robot_robot_goal >= 0){
					cmd.left = -50;
					cmd.right = 50;
				}else{
					cmd.left = 50;
					cmd.right = -50;
				}
			}
		}else{
			if(angulation_robot_robot_goal < 0){
				angulation_robot_robot_goal += 180;
			}else{
				angulation_robot_robot_goal -= 180;		
			}

			float PI = turn_gain*angulation_robot_robot_goal;// + 0.001*sumError;

			if(fabs(angulation_robot_robot_goal) < angle_to_spin){
				cmd.left = distance_robot_goal + (PI * robot_side_size);
				cmd.right = distance_robot_goal - (PI * robot_side_size);
				
				cmd.left *= -velocity_gain;
				cmd.right *= -velocity_gain;
			}else{
				// SPIN
				if(angulation_robot_robot_goal >= 0){
					cmd.left = 50;
					cmd.right = -50;
				}else{
					cmd.left = -50;
					cmd.right = 50;
				}
			}
		}

		/*if(distancePoint(pose, final_pose) < distance_to_stop){
			cmd.left = 0;
			cmd.right = 0;
		}*/

		count_pose++;
	}else{
		if(front){
			cmd.left = -25;
			cmd.right = -25;
		}else{
			cmd.left = 25;
			cmd.right = 25;
		}
		rear_count--;
	}
}
