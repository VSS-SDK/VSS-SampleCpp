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

    apf.set_robots(our_poses, adversary_poses);
    pp.setRobots(our_poses, adversary_poses);

    AT_projection();

    //btVector3 potential = apf.calc_result(id, final_pose, true, GOTO::BALL);
    if( distancePoint(pose, path.poses.at(act_pose_of_path)) < RADIUS_ROBOT*2.0 ){
        if(act_pose_of_path < path.poses.size()-1){
            act_pose_of_path++;
        }
    }

    btVector3 potential = apf.calc_result(id, path.poses.at(act_pose_of_path), true, GOTO::POSITION);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
    
    calc_cmd_to();
}

void Robot::AT_path_planning(){
    btVector3 sub_projection, sup_projection;
    
    sub_projection.x = projection.x - (cos(projection.z)*15.0);
    sub_projection.y = projection.y - (sin(projection.z)*15.0); 
    sub_projection.z = sub_projection.z;

    path = pp.solvePath(id, sub_projection);
    //path = pp.solvePath(id, projection);
    path.poses.push_back(projection);
}

void Robot::AT_projection(){
    if(distancePoint(pose, projection) < 10.0 || status == 0){
        resolve_iterator++;
        status = 1;
        act_pose_of_path = 0;

        final_pose = generate_free_pose();

        projection = final_pose;

        AT_path_planning();
    }
}

btVector3 Robot::generate_free_pose(){
    bool pose_ok = false;
    btVector3 new_pose;

    while(!pose_ok){
        pose_ok = true;

        new_pose.x = (rand() % 120) + 20;
        new_pose.y = (rand() % 110) + 10;
        new_pose.z = rand() % 360;
        
        for(int i = 0 ; i < our_team->size() ; i++){
            if( distancePoint(new_pose, our_team->at(i).pose ) < RADIUS_ROBOT*4.0 ){
                pose_ok = false;
                break;
            }
        }

        if(pose_ok){
            for(int i = 0 ; i < adversary_team->size() ; i++){
                if( distancePoint(new_pose, adversary_team->at(i).pose ) < RADIUS_ROBOT*4.0 ){
                    pose_ok = false;
                    break;
                }
            }
        }
    }

    return new_pose;
}