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
    turn_gain = TURN_GAIN;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses, *ball);

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

    calc_cmd_to();
}

void Robot::AT_projection(){
    /*if(ball_is_on_the_wall()){
        cout << "WALL" << endl;
    }else{
        cout << "!WALL" << endl;
    }*/
    switch(attacker_state){
        case AttackerState::AT_GET_BEHIND_THE_BALL:{
            cout << "GET_BEHIND_THE_BALL" << endl;

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
            cout << "ADJUST_TO_GET_THE_BALL" << endl;

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
                turn_gain += 0.015;
                projection = test_var;
            }else{
                attacker_state = AttackerState::AT_APPROACH_OF_THE_BALL;
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::AT_APPROACH_OF_THE_BALL:{
            cout << "APPROACH_OF_THE_BALL" << endl;
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
            cout << "KICK_THE_BALL" << endl;
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
            turn_gain += 0.015;
            iterator_aceleration += 0.01;
            velocity_gain += iterator_aceleration;
        }break;
    }

    final_pose = projection;
}
