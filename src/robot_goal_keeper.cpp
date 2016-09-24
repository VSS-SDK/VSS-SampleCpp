/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */
 
#include "robot.h"

void Robot::GK_calc_action(){
    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses, *ball);

    GK_may_reach_the_ball_in_time();
    GK_projection();
    
    btVector3 potential = apf.calc_result(id, projection, true, GOTO::POSITION, goal_keeper_state);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
    
    if(goal_keeper_state != GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL && goal_keeper_state != GoalKeeperState::GK_ADJUST_ANGLE){
        //cout << fabs(pose.x - projection.x) << endl;
        if(fabs(pose.x - projection.x) > 5){
            cout << "AT" << endl;
            AT_calc_cmd();
        }else{
            cout << "GK" << endl;
            GK_calc_cmd();
        }
    }
}

void Robot::GK_projection(){
    float y_min = 45;
    float y_max = 85;
    path.poses.clear();
    float theta = radian(ball_in_the_future, goal[goal_defense]);

    switch(goal_keeper_state){
        case GoalKeeperState::GK_MARK_THE_BALL:{
            cout << "GK_MARK_THE_BALL" << endl;
            if(distancePoint(pose, *ball) > 10){
                path.poses.clear();
                float theta = radian(ball_in_the_future, goal[goal_defense]);

                if(goal_defense == Goal::RIGHT){
                    projection = goal[goal_defense];
                    projection.x -= 12;
                    projection.y = ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-150));

                    path.poses.push_back(btVector3(160, y_min, 0));
                    path.poses.push_back(btVector3(147, y_min, 0));
                    path.poses.push_back(btVector3(147, y_max, 0));
                    path.poses.push_back(btVector3(160, y_max, 0));
                    
                }else{
                    projection = goal[goal_defense];
                    projection.x += 12;
                    projection.y = ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-10));


                    path.poses.push_back(btVector3(10, y_min, 0));
                    path.poses.push_back(btVector3(23, y_min, 0));
                    path.poses.push_back(btVector3(23, y_max, 0));
                    path.poses.push_back(btVector3(10, y_max, 0));
                }

                if(projection.y > y_max){
                    projection.y = y_max;
                }

                if(projection.y < y_min){
                    projection.y = y_min;
                }
            }else{
                goal_keeper_state = GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL;  
            }
        }break;
        case GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL:{
            cout << "GK_SPIN_TO_KICK_THE_BALL" << endl;
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
                goal_keeper_state = GoalKeeperState::GK_MARK_THE_BALL;
            }
        }break;
    }

    final_pose = projection;
}

void Robot::GK_may_reach_the_ball_in_time(){
    ball_in_the_future.x = ball->x + (v_ball->x*0.25);
    ball_in_the_future.y = ball->y + (v_ball->y*0.25);
}

// MODEL BLAZIC
void Robot::GK_calc_cmd(){
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

    float soft_gain;
    float cmd_adjust;

    if(!real_environment){
        soft_gain = 0.1;
        cmd_adjust = 10;
    }else{
        soft_gain = 0.1;
        cmd_adjust = 5;
    }

    //velocity_gain += 1.0;

    if(pose.z > 70 && pose.z < 100){
        if(step_pose.y > pose.y){
            //cmd.left = distance_robot_goal;
            //cmd.right = distance_robot_goal;
            float soft_gain;
			float PI = turn_gain*angulation_robot_robot_goal;// + 0.001*sumError;

            cmd.left = distance_robot_goal + (PI * robot_side_size)*soft_gain;
            cmd.right = distance_robot_goal - (PI * robot_side_size)*soft_gain;
            
            cmd.left *= -velocity_gain;
            cmd.right *= -velocity_gain;
        }else{
            float PI = turn_gain*angulation_robot_robot_goal;// + 0.001*sumError;

            cmd.left = distance_robot_goal - (PI * robot_side_size)*soft_gain;
            cmd.right = distance_robot_goal + (PI * robot_side_size)*soft_gain;
            
            cmd.left *= velocity_gain;
            cmd.right *= velocity_gain;
        }
    }else{
        if(pose.z > 90){
            cmd.right = cmd_adjust;
            cmd.left = 1;
        }else{
            cmd.right = -cmd_adjust;
            cmd.left = 1;
        }
    }
}
