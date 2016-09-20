/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "robot.h"

void Robot::DF_calc_action(){
    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;
    turn_gain = TURN_GAIN;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses, *ball);

    DF_projection();
    
    btVector3 potential = apf.calc_result(id, projection, true, GOTO::POSITION, defender_state);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
    
    calc_cmd_to();
}

void Robot::DF_projection(){ 
    switch(defender_state){
        case DF_MARK_THE_BALL:{
            path.poses.clear();
            float theta = radian(*ball, goal[goal_defense]);

            //if(){
                if(goal_defense == Goal::RIGHT){
                    float distance_of_mark = ball->x + 40;

                    if(distance_of_mark < 95){
                        distance_of_mark = 95;
                    }
                    if(distance_of_mark > 130){
                        distance_of_mark = 130;
                    }

                    projection = btVector3(distance_of_mark, ball->y - (sin(theta)*fabs(ball->x-distance_of_mark)), 0);

                    // DEBUG
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 130, 0));
                    path.poses.push_back(btVector3(130, 130, 0));
                    path.poses.push_back(btVector3(130, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                }else{
                    if(ball->x > 40){
                        float distance_of_mark = ball->x - 40;

                        if(distance_of_mark > 75){
                            distance_of_mark = 75;
                        }

                        if(distance_of_mark < 40){
                            distance_of_mark = 40;
                        }

                        projection = btVector3(distance_of_mark, ball->y - (sin(theta)*fabs(ball->x-distance_of_mark)), 0);

                        // DEBUG
                        path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                        path.poses.push_back(btVector3(distance_of_mark, 130, 0));
                        path.poses.push_back(btVector3(40, 130, 0));
                        path.poses.push_back(btVector3(40, 0, 0));
                        path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                    }
                }

        }break;
        case DF_INSULATES_THE_BALL:{
            float theta = radian(*ball, goal[goal_defense]);

            if(goal_defense == Goal::RIGHT){
                projection = btVector3(130, ball->y - (sin(theta)*fabs(ball->x-130)), 0);
            }else{
                projection = btVector3(40, ball->y - (sin(theta)*fabs(ball->x-40)), 0);
            }
        }break;
    }


    final_pose = projection;
    /*switch(attacker_state){
        case AttackerState::GET_BEHIND_THE_BALL:{
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
                attacker_state = AttackerState::ADJUST_TO_GET_THE_BALL;
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::ADJUST_TO_GET_THE_BALL:{
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
                attacker_state = AttackerState::APPROACH_OF_THE_BALL;
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::APPROACH_OF_THE_BALL:{
            cout << "APPROACH_OF_THE_BALL" << endl;
            if(goal_attack == Goal::LEFT){
                if(distancePoint(pose, *ball) >= 10.0 && pose.x > ball->x){
                    projection = *ball;
                }else{
                    attacker_state = AttackerState::KICK_THE_BALL;
                }
            }else{
                if(distancePoint(pose, *ball) >= 10.0 && pose.x < ball->x){
                    projection = *ball;
                }else{
                    attacker_state = AttackerState::KICK_THE_BALL;
                }
            }

            iterator_aceleration = 0.0;
            velocity_gain = 2.5;
        }break;
        case AttackerState::KICK_THE_BALL:{
            cout << "KICK_THE_BALL" << endl;
            if(goal_attack == Goal::LEFT){
                if(distancePoint(pose, goal[goal_attack]) >= 10.0 && pose.x > ball->x && distancePoint(pose, *ball) <= 15.0) {
                    projection = goal[goal_attack];
                }else{
                    attacker_state = AttackerState::GET_BEHIND_THE_BALL;
                }
            }else{
                if(distancePoint(pose, goal[goal_attack]) >= 10.0 && pose.x < ball->x && distancePoint(pose, *ball) <= 15.0) {
                    projection = goal[goal_attack];
                }else{
                    attacker_state = AttackerState::GET_BEHIND_THE_BALL;
                } 
            }
            turn_gain += 0.015;
            iterator_aceleration += 0.01;
            velocity_gain += iterator_aceleration;
        }break;
    }

    final_pose = projection;*/
}
