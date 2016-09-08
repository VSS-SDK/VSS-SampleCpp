#include "potential_fields.h"

PotentialFields::PotentialFields(){
	alpha = 1.0;
	radiusBall = 4.267 / 2.0; // Diametro de uma bola de golfe 4.267 / 2.0 = raio 
	areaBall = 12.0;

	omega = 3.0;
	radiusPosition = 1.0;
	areaPosition = 5.0;

	beta = 0.4; //0.4
	INF = 0.4; //20
	radiusRobot = 11.3 / 2.0; // Diagonal de um quadrado L = L*sqrt(2). L = 8 = diagonal 11.3 / 2.0 = raio
	areaRobot = 20.0;


	id = 0;
	is_last = true;
	result = goal = btVector3(0, 0, 0);
}

void PotentialFields::set_robots(vector<btVector3> our_robots, vector<btVector3> adversary_robots){
	this->our_robots = our_robots;
    this->adversary_robots = adversary_robots;
}

btVector3 PotentialFields::calc_result(int id, btVector3 goal, bool is_last, GOTO go_to){
	result = btVector3(0, 0, 0);
	this->id = id;
	this->is_last = is_last;
	this->goal = goal;
	this->go_to = go_to;

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

	switch(go_to){
		case GOTO::POSITION:{
			if(is_last){
				if(distances < radiusPosition){
					x += 0;
					y += 0;
				}
				else if(distances <= (radiusPosition + areaPosition)){
					x += -omega*(distances - radiusPosition)*cos(theta); 
					y += -omega*(distances - radiusPosition)*sin(theta);
				}else{
					x += -omega*areaPosition*cos(theta); 
					y += -omega*areaPosition*sin(theta);
				}
			}else{
				x += -omega*areaPosition*cos(theta); 
				y += -omega*areaPosition*sin(theta);
			}
		}break;
		case GOTO::BALL:{
			if(is_last){
				if(distances < radiusBall){
					x += 0;
					y += 0;
				}
				else if(distances <= (radiusBall + areaBall)){
					x += -alpha*(distances - radiusBall)*cos(theta); 
					y += -alpha*(distances - radiusBall)*sin(theta);
				}else{
					x += -alpha*areaBall*cos(theta); 
					y += -alpha*areaBall*sin(theta);
				}
			}else{
				x += -alpha*areaBall*cos(theta); 
				y += -alpha*areaBall*sin(theta);
			}
		}break;
		case GOTO::ROBOT:{

		}break;
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