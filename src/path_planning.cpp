/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "path_planning.h"

vector<btVector3> robotsStatic;

PathPlanning::PathPlanning(){
    lowBound = 0;
    highBound = 170;   
    timeToResolve = 1;
    planner = PLANNER_RRTSTAR;
}

Path PathPlanning::solvePath(int id_robot, btVector3 goal_pose){
    /*ob::StateSpacePtr space(new ob::SE2StateSpace());

    ob::RealVectorBounds bounds(2);
    bounds.setLow(lowBound);
    bounds.setHigh(highBound);

    space->as<ob::SE2StateSpace>()->setBounds(bounds);

    ob::SpaceInformationPtr si(new ob::SpaceInformation(space));

    si->setStateValidityChecker(boost::bind(&isStateValid, _1));

    ob::ScopedState<ob::SE2StateSpace> start(space);
    ob::ScopedState<ob::SE2StateSpace> goal(space);


    start->setX((int)robots.at(id_robot).x);
    start->setY((int)robots.at(id_robot).y);
    //start->setYaw(0);

    goal->setX(goal_pose.x);
    goal->setY(goal_pose.y);
    //goal->setYaw(0);


    ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));

    pdef->setStartAndGoalStates(start, goal);

    ob::PlannerPtr planner(new og::RRTConnect(si));

    planner->setProblemDefinition(pdef);

    planner->setup();
    //si->printSettings(std::cout);
    //pdef->print(std::cout);

    ob::PlannerStatus solved = planner->solve(timeToResolve);

    if(solved){
        ob::PathPtr path_ptr;
        path_ptr = pdef->getSolutionPath();
        path = PathPtr2Path(path_ptr);
    }else{
        std::cout << "No solution found" << std::endl;
    }*/

    // construct the state space we are planning in
        ob::StateSpacePtr space(new ob::SE3StateSpace());
    
        // set the bounds for the R^3 part of SE(3)
        ob::RealVectorBounds bounds(3);
        bounds.setLow(0);
        bounds.setHigh(170);
    
        space->as<ob::SE3StateSpace>()->setBounds(bounds);
    
        // construct an instance of  space information from this state space
        ob::SpaceInformationPtr si(new ob::SpaceInformation(space));
    
        // set state validity checking for this space
        si->setStateValidityChecker(std::bind(&isStateValid, std::placeholders::_1));
    
        /* create a random start state
        ob::ScopedState<> start(space);
        start.random();
    
        // create a random goal state
        ob::ScopedState<> goal(space);
        goal.random();*/

        ob::ScopedState<ob::SE3StateSpace> start(space);
        ob::ScopedState<ob::SE3StateSpace> goal(space);


        start->setX((int)robots.at(id_robot).x);
        start->setY((int)robots.at(id_robot).y);
        //start->setYaw(0);

        goal->setX(goal_pose.x);
        goal->setY(goal_pose.y);
        //goal->setYaw(0);
    
        // create a problem instance
        ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));
    
        // set the start and goal states
        pdef->setStartAndGoalStates(start, goal);
    
        // create a planner for the defined space
        ob::PlannerPtr planner(new og::RRTConnect(si));
   
       // set the problem we are trying to solve for the planner
       planner->setProblemDefinition(pdef);
   
       // perform setup steps for the planner
       planner->setup();
   
   
       // print the settings for this space
       //si->printSettings(std::cout);
   
       // print the problem settings
       //pdef->print(std::cout);
   
       // attempt to solve the problem within one second of planning time
       ob::PlannerStatus solved = planner->solve(1.0);
        cout << "teste" << endl;
       if(solved){
           cout << "teste" << endl;
           ob::PathPtr path_ptr = pdef->getSolutionPath();
           path = PathPtr2Path(path_ptr);
       }

    return path;
}

bool PathPlanning::isStateValid(const ob::State *state){
    bool ok = true;
    const ob::SE2StateSpace::StateType* state2D = state->as<ob::SE2StateSpace::StateType>();
    double x = state2D->getX();
    double y = state2D->getY();

    for(int i = 0 ; i < robotsStatic.size() ; i++){
        //robotsStatic.at(i).show();
        double dis = sqrt((x-robotsStatic.at(i).x)*(x-robotsStatic.at(i).x) + (y-robotsStatic.at(i).y)*(y-robotsStatic.at(i).y)) - 1;/*robotsStatic.at(i).radius*/;
        if(dis < 0.0){
            ok = false;
        }
    }
    
    return ok;
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