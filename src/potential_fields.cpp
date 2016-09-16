/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "potential_fields.h"

PotentialFields::PotentialFields(){
	alpha = 1.0;
	radiusBall = 4.267 / 2.0; // Diametro de uma bola de golfe 4.267 / 2.0 = raio 
	areaBall = 12.0;

	omega = 3.0;
	radiusPosition = 1.0;
	areaPosition = 5.0;

	beta = 0.9; //0.4
	INF = 1.1; //20
	radiusRobot = 11.3 / 1.6; // Diagonal de um quadrado L = L*sqrt(2). L = 8 = diagonal 11.3 / 2.0 = raio
	areaRobot = 20.0;

	max_module = 15.0;

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
	repulsive_force_our_robots();
	repulsive_force_adversary_robots();
	
	normalize();

	return result;
}

int PotentialFields::sign(float signal){
	if(signal < 0){
		return -1;
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
	double theta;
	double distances;
	int k;

	for(int j = 0 ; j < our_robots.size() ; j++){
		if(id != j){
			theta = radian(our_robots.at(id), our_robots.at(j));
			distances = distancePoint(our_robots.at(id), our_robots.at(j));

			if(distances <= radiusRobot){
				//se esta escostado no obstaculo, recebe um vetor maximo 
				result.x += tanh(cos(theta))*beta;
				result.y += tanh(sin(theta))*INF;

			}else if(distances <= (radiusRobot + areaRobot)){
				//se esta dentro da area de influencia
				result.x += beta*(areaRobot + radiusRobot - distances)*cos(theta); 
				result.y += INF*(areaRobot + radiusRobot - distances)*sin(theta);	
			}
		}
	}
}

void PotentialFields::repulsive_force_adversary_robots(){
	double theta;
	double distances;
	int k;

	for(int j = 0 ; j < adversary_robots.size() ; j++){
		theta = radian(our_robots.at(id), adversary_robots.at(j));
		distances = distancePoint(our_robots.at(id), adversary_robots.at(j));

		if(distances <= radiusRobot){
			//se esta escostado no obstaculo, recebe um vetor maximo 
			result.x += tanh(cos(theta))*beta;
			result.y += tanh(sin(theta))*INF;

		}else if(distances <= (radiusRobot + areaRobot)){
			//se esta dentro da area de influencia
			result.x += beta*(areaRobot + radiusRobot - distances)*cos(theta); 
			result.y += INF*(areaRobot + radiusRobot - distances)*sin(theta);	
		}
	}

	result.x *= 2.5;
	result.y *= 2.5;
}

void PotentialFields::normalize(){
	float size;
	float overshoot;

	size = distancePoint(btVector3(0, 0, 0), result);

	// Trata overshoot criado pelo campo repulsivo

	overshoot = size/max_module;

	if(overshoot > 1.00){
		overshoot *= 100.0;
		result.x = (result.x / overshoot)*100.0;
		result.y = (result.y / overshoot)*100.0;
	}

	//cout << distancePoint(btVector3(0, 0, 0), result) << endl;
}