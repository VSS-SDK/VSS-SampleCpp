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

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses, *ball);

    DF_may_reach_the_ball_in_time();
    DF_projection();
    
    btVector3 potential = apf.calc_result(id, projection, true, GOTO::POSITION, defender_state);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
    
    DF_calc_cmd();
}

void Robot::DF_projection(){ 
    switch(defender_state){
        case DF_MARK_THE_BALL:{
            path.poses.clear();
            float theta = radian(ball_in_the_future, goal[goal_defense]);
            
            //if(distancePoint(pose, *ball) > 10){
                if(goal_defense == Goal::RIGHT){
                    float distance_of_mark = ball_in_the_future.x + 40;

                    if(distance_of_mark < 95){
                        distance_of_mark = 95;
                    }
                    if(distance_of_mark > 130){
                        distance_of_mark = 130;
                    }

                    projection = btVector3(distance_of_mark, ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-distance_of_mark)), 0);

                    // DEBUG
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 130, 0));
                    path.poses.push_back(btVector3(130, 130, 0));
                    path.poses.push_back(btVector3(130, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                }else{
                    float distance_of_mark = ball_in_the_future.x - 40;

                    if(distance_of_mark > 75){
                        distance_of_mark = 75;
                    }

                    if(distance_of_mark < 40){
                        distance_of_mark = 40;
                    }

                    projection = btVector3(distance_of_mark, ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-distance_of_mark)), 0);

                    // DEBUG
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 130, 0));
                    path.poses.push_back(btVector3(40, 130, 0));
                    path.poses.push_back(btVector3(40, 0, 0));
                    path.poses.push_back(btVector3(distance_of_mark, 0, 0));
                }
            //}else{
              //  defender_state = DefenderState::DF_SPIN_TO_KICK_THE_BALL;
            //}
        }break;
        /*case DefenderState::DF_SPIN_TO_KICK_THE_BALL:{
            if(distancePoint(pose, *ball) <= 10){
                if(goal_defense == Goal::RIGHT){
                    if(pose.y < ball->y){
                        //cout << "GK_SPIN_RIGHT" << endl;
                        cmd.left = 245;
                        cmd.right = -245;
                    }else if(pose.y > ball->y){
                        //cout << "GK_SPIN_LEFT" << endl;
                        cmd.left = -245;
                        cmd.right = 245;
                    }
                }else{
                    if(pose.y < ball->y){
                        //cout << "GK_SPIN_LEFT" << endl;
                        cmd.left = -245;
                        cmd.right = 245;
                    }else if(pose.y > ball->y){
                        //cout << "GK_SPIN_RIGHT" << endl;
                        cmd.left = 245;
                        cmd.right = -245;
                    }
                }
            }else{
                defender_state = DefenderState::DF_MARK_THE_BALL;
            }
        }break;*/
        /*case DF_INSULATES_THE_BALL:{
            float theta = radian(*ball, goal[goal_defense]);

            if(goal_defense == Goal::RIGHT){
                projection = btVector3(130, ball->y - (sin(theta)*fabs(ball->x-130)), 0);
            }else{
                projection = btVector3(40, ball->y - (sin(theta)*fabs(ball->x-40)), 0);
            }
        }break;*/
    }

    final_pose = projection;
}

void Robot::DF_may_reach_the_ball_in_time(){
    // this must not be calculated any time
    //time_to_reach_the_ball = (distancePoint(pose, *ball) + RADIUS_BALL + RADIUS_ROBOT) / distancePoint(v_pose, *v_ball);
    ball_in_the_future.x = ball->x + (v_ball->x*0.25);
    ball_in_the_future.y = ball->y + (v_ball->y*0.25);
    //cout << "ball: " << endl;
    //ball->show();
    //cout << "future: " << endl;
    //ball_in_the_future.show();
}

void Robot::DF_calc_cmd(){
	AT_calc_cmd();
}