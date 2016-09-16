/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "path_planning.h"

vector<btVector3> robotsStatic;
int staticID;

PathPlanning::PathPlanning(){
    lowBound = 0;
    highBound = 170;   
    timeToResolve = 0.1;
    planner = PLANNER_RRTSTAR;
    step_size = 15;0;
}

Path PathPlanning::solvePath(int id_robot, btVector3 goal_pose){
    path.poses.clear();
    staticID = id_robot;
    ob::StateSpacePtr space(new ob::SE2StateSpace());

    ob::RealVectorBounds bounds(2);
    bounds.setLow(0);
    bounds.setHigh(170);

    space->as<ob::SE2StateSpace>()->setBounds(bounds);

    ob::SpaceInformationPtr si(new ob::SpaceInformation(space));

    si->setStateValidityChecker(boost::bind(&isStateValid, _1));

    ob::ScopedState<ob::SE2StateSpace> start(space);
    ob::ScopedState<ob::SE2StateSpace> goal(space);

    double x_i = robots.at(id_robot).x;
    double y_i = robots.at(id_robot).y;

    double x_f = goal_pose.x;
    double y_f = goal_pose.y;
    
    start->setX(x_i);
    start->setY(y_i);

    goal->setX(x_f);
    goal->setY(y_f);

    ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));

    pdef->setStartAndGoalStates(start, goal);
    
    //ob::PlannerPtr planner(new og::LazyRRT(si));      // GOOD
    //ob::PlannerPtr planner(new og::pRRT(si));

    ob::PlannerPtr planner(new og::RRT(si));        // VERY GOOD

    planner->setProblemDefinition(pdef);

    planner->setup();
    //si->printSettings(std::cout);
    //pdef->print(std::cout);

    ob::PlannerStatus solved = planner->solve(timeToResolve);

    if(solved){
        ob::PathPtr path_ptr;
        path_ptr = pdef->getSolutionPath();
        path = PathPtr2Path(path_ptr);
    }

    return path;
}

bool PathPlanning::isStateValid(const ob::State *state){
    bool ok = true;
    const ob::SE2StateSpace::StateType* state2D = state->as<ob::SE2StateSpace::StateType>();
    double x = state2D->getX();
    double y = state2D->getY();

    if(x < 20 || x > 140 || y < 20 || y > 100){
        ok = false;
    }

    for(int i = 0 ; i < robotsStatic.size() ; i++){
        if(i != staticID){
            double dis = sqrt((x-robotsStatic.at(i).x)*(x-robotsStatic.at(i).x) + (y-robotsStatic.at(i).y)*(y-robotsStatic.at(i).y));
            cout << "dist:" << dis << endl;
            if(dis < RADIUS_ROBOT){
                ok = false;
                cout << "FALSE" << endl;
            }
            cout << endl;
        }
    }
    
    return ok;
}


bool PathPlanning::isStateValidEasy(const ob::SpaceInformation *si, const ob::State *state)
{
    const ob::SE2StateSpace::StateType *s = state->as<ob::SE2StateSpace::StateType>();
    double x=s->getX(), y=s->getY();
    return si->satisfiesBounds(s) && (x<5 || x>13 || (y>8.5 && y<9.5));
}

bool PathPlanning::isStateValidHard(const ob::SpaceInformation *si, const ob::State *state)
{
    return si->satisfiesBounds(state);
}


void PathPlanning::setBounds(float lowBound, float highBound){
    this->lowBound = lowBound;
    this->highBound = highBound;
}

void PathPlanning::setPlanner(typePlanner planner){
    this->planner = planner;
}

void PathPlanning::setTimeToResolve(float timeToResolve){
    this->timeToResolve = timeToResolve;
}

void PathPlanning::setRobots(vector<btVector3> our_robots, vector<btVector3> adversary_robots){
    vector<btVector3> robots_s;
    robots_s = our_robots;

    for(int i = 0 ; i < adversary_robots.size() ; i++){
        robots_s.push_back(adversary_robots.at(i));
    }

    this->robots = robots_s;
    robotsStatic = this->robots;
}