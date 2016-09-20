/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */
 
#include "robot.h"

void Robot::GK_calc_action(){
    //cmd = Command(-0, 0);
    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;
    for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }
    apf.set_robots(our_poses, adversary_poses, *ball);

    GK_projection();
    
}

void Robot::GK_projection(){

}