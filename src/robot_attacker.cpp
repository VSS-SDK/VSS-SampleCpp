/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "robot.h"

void Robot::AT_calc_action(){
    path.poses.clear();
    
    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses);
    pp.setRobots(our_poses, adversary_poses);

    AT_projection();
    AT_path_planning();

    //btVector3 potential = apf.calc_result(id, final_pose, true, GOTO::BALL);
    btVector3 potential = apf.calc_result(id, final_pose, true, GOTO::POSITION);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
        
    //path.poses.push_back(pose);
    //path.poses.push_back(final_pose);

    calc_cmd_to();
}

void Robot::AT_path_planning(){
        //path = pp.solvePath(id, projection);
        //final_pose = path.poses.at(path.poses.size()-1);
        //final_pose.z = final_pose.z * (180/M_PI);
        //path.show();
        //distance_between_projections = 0;
    //}
}

void Robot::AT_projection(){
    if(debug_pos){
        debug_pos = false;
        final_pose.x = (rand() % 130) + 20;
        final_pose.y = (rand() % 110) + 10;
        final_pose.z = rand() % 360;
    }

    final_pose = *ball;
    //projection = final_pose;
    //projection = *ball;
}