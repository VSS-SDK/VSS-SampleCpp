#ifndef _POTENTIAL_FIELDS_H_
#define _POTENTIAL_FIELDS_H_

#include "common.h"

using namespace common;

class PotentialFields{
protected:
    int id;
    bool is_last;
    float beta, INF, radiusRobot, areaRobot;
    float alpha, radiusBall, areaBall;
    float omega, radiusPosition, areaPosition;
    
    vector<btVector3> our_robots;
    vector<btVector3> adversary_robots; 
	btVector3 result, goal;
    GOTO go_to;

    int sign(float signal);
	void attractive_force();
	void repulsive_force_our_robots();
	void repulsive_force_adversary_robots();
public:

    PotentialFields();

    void set_robots(vector<btVector3> our_robots, vector<btVector3> adversary_robots);
    
	btVector3 calc_result(int, btVector3, bool, GOTO);
};

#endif // _POTENTIAL_FIELD_H_