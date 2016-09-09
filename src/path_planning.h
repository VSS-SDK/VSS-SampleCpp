/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef _PATH_PLANNING_H_
#define _PATH_PLANNING_H_

#include "ompl.h"
#include "vector"
#include "common.h"

using namespace std;
using namespace common;

class PathPlanning{
protected:
    int id_robot;
    float timeToResolve;                                    // Tempo para resolver o problema     
    float lowBound, highBound;                              // Limites do espaço
    typePlanner planner;                                    // Algoritmo de planejamento de trajetória
    planningObjective objectiveType;

    Path path;
    vector<btVector3> robots;
    btVector3 goal_pose;
    
    static bool isStateValid(const ob::State *state);
    static bool isStateValidEasy(const ob::SpaceInformation *si, const ob::State *state);
    static bool isStateValidHard(const ob::SpaceInformation *si, const ob::State *state);

public:
    PathPlanning();  
    
    Path solvePath(int id_robot, btVector3 goal_pose);
    
    void setBounds(float, float);
    void setPlanner(typePlanner);
    void setTimeToResolve(float);

    void setRobots(vector<btVector3>, vector<btVector3>);
};

#endif // _PATH_PLANNING_H_