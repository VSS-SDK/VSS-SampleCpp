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
    turn_gain = TURN_GAIN;

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
    
    if(goal_keeper_state != GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL){
        calc_cmd_to();
    }
}

void Robot::GK_projection(){
    switch(goal_keeper_state){
        case GoalKeeperState::GK_MARK_THE_BALL:{
            if(distancePoint(pose, *ball) > 10){
                cout << "GK_MARK_THE_BALL" << endl;
                path.poses.clear();
                float theta = radian(ball_in_the_future, goal[goal_defense]);

                if(goal_defense == Goal::RIGHT){
                    projection = goal[goal_defense];
                    projection.x -= 10;
                    projection.y = ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-150));
                }else{
                    projection = goal[goal_defense];
                    projection.x += 10;
                    projection.y = ball_in_the_future.y - (sin(theta)*fabs(ball_in_the_future.x-10));
                }

                if(projection.y > 90){
                    projection.y = 90;
                }

                if(projection.y < 50){
                    projection.y = 50;
                }
            }else{
                goal_keeper_state = GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL;
            }
        }break;
        case GoalKeeperState::GK_SPIN_TO_KICK_THE_BALL:{
            if(distancePoint(pose, *ball) <= 10){
                if(goal_defense == Goal::RIGHT){
                    if(pose.y < ball->y){
                        cout << "GK_SPIN_RIGHT" << endl;
                        cmd.left = 300;
                        cmd.right = -300;
                    }else if(pose.y > ball->y){
                        cout << "GK_SPIN_LEFT" << endl;
                        cmd.left = -300;
                        cmd.right = 300;
                    }
                }else{
                    if(pose.y < ball->y){
                        cout << "GK_SPIN_LEFT" << endl;
                        cmd.left = -300;
                        cmd.right = 300;
                    }else if(pose.y > ball->y){
                        cout << "GK_SPIN_RIGHT" << endl;
                        cmd.left = 300;
                        cmd.right = -300;
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