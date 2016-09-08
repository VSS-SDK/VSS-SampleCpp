#include "robot.h"

void Robot::AT_calc_action(){
    path.poses.clear();
    if(debug_pos){
        debug_pos = false;
        final_pose.x = (rand() % 80) + 30;
        final_pose.y = (rand() % 60) + 30;
        final_pose.z = rand() % 360;
    }

    vector<btVector3> our_poses;
    vector<btVector3> adversary_poses;

	for(int i = 0 ; i < our_team->size() ; i++){
		our_poses.push_back(our_team->at(i).get_pose());
	    adversary_poses.push_back(adversary_team->at(i).get_pose());
    }

    apf.set_robots(our_poses, adversary_poses);
    //btVector3 potential = apf.calc_result(id, final_pose, true, GOTO::BALL);
    btVector3 potential = apf.calc_result(id, final_pose, true, GOTO::POSITION);
    
    step_pose.x = pose.x + potential.x;
    step_pose.y = pose.y + potential.y;
    step_pose.z = pose.z + potential.z;
        
    path.poses.push_back(pose);
    path.poses.push_back(final_pose);

    calc_cmd_to();
}