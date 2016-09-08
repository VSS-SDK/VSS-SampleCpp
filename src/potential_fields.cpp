#include "potential_fields.h"

PotentialFields::PotentialFields(){
	alpha = 1.0;
	beta = 0.4; //0.4
	INF = 0.4; //20
	radiusRobot = 11.3; // Diagonal de um quadrado L = L*sqrt(2). L = 8 = diagonal 11.3
	areaRobot = 20.0;

	id = 0;
	is_last = true;
	result = goal = btVector3(0, 0, 0);
}

void PotentialFields::set_robots(vector<btVector3> our_robots, vector<btVector3> adversary_robots){
	this->our_robots = our_robots;
    this->adversary_robots = adversary_robots;
}

btVector3 PotentialFields::calc_result(int id, btVector3 goal, bool is_last){
	result = btVector3(0, 0, 0);
	this->id = id;
	this->is_last = is_last;
	this->goal = goal;

	attractive_force();
	//repulsiveForceOurRobots();
	//repulsiveForceAdvRobots();

	return result;
}

int PotentialFields::sign(float signal){
	if(signal < 0){
		return -1;
	}else if(signal == 0){
		return 0;
	}else{
		return 1;
	}
}

void PotentialFields::attractive_force(){
	double theta, distances;
	float x, y;
	x = y = 0;

	theta = radian(our_robots.at(id), goal);
	distances = distancePoint(our_robots.at(id), goal);

	if(is_last){
		if(distances < radiusRobot){
			x += 0;
			y += 0;
		}
		else if(distances <= (radiusRobot + areaRobot)){
			x += -alpha*(distances - radiusRobot)*cos(theta); 
			y += -alpha*(distances - radiusRobot)*sin(theta);
		}else{
			x += -alpha*areaRobot*cos(theta); 
			y += -alpha*areaRobot*sin(theta);
		}
	}else{
		x += -alpha*areaRobot*cos(theta); 
		y += -alpha*areaRobot*sin(theta);
	}

	result.x += x;
	result.y += y;
}

void PotentialFields::repulsive_force_our_robots(){
	// TODO
}

void PotentialFields::repulsive_force_adversary_robots(){
	// TODO
}