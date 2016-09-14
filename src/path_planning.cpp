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
    step_size = 15;0;
}

Path PathPlanning::solvePath(int id_robot, btVector3 goal_pose){
    /*this->id_robot = id_robot;
    bool easy = true;
    ob::StateSpacePtr space(new ob::ReedsSheppStateSpace);
    //ob::StateSpacePtr space(new ob::DubinsStateSpace);
    
    ob::ScopedState<> start(space), goal(space);
    ob::RealVectorBounds bounds(2);
    bounds.setLow(0);
    if (easy)
        bounds.setHigh(170);
    else
    {
        bounds.high[0] = 170;
        bounds.high[1] = 0.5;
    }
    space->as<ob::SE2StateSpace>()->setBounds(bounds);

    // define a simple setup class
    og::SimpleSetup ss(space);

    // set state validity checking for this space
    const ob::SpaceInformation *si = ss.getSpaceInformation().get();
    auto isStateValid = easy ? isStateValidEasy : isStateValidHard;
    ss.setStateValidityChecker([isStateValid, si](const ob::State *state)
        {
            return isStateValid(si, state);
        });

    // set the start and goal states
    if (easy)
    {   
        
        //start[0] = (int)robots.at(id_robot).x/10.0; 
        //start[1] = (int)robots.at(id_robot).y/10.0;
        //start[2] = 0;
        //goal[0] = (int)goal_pose.x/10.0;
        //goal[1] = (int)goal_pose.y/10.0;
        //goal[2] = -.99*boost::math::constants::pi<double>();
        
        int x_i = robots.at(id_robot).x;
        int y_i = robots.at(id_robot).y;

        int x_f = goal_pose.x;
        int y_f = goal_pose.y;
    
        start[0] = x_i;
        start[1] = y_i;
        start[2] = 0.0;
        
        goal[0] = x_f;
        goal[1] = y_f;
        goal[2] = 0.0;//-.99*boost::math::constants::pi<double>();

        /*cout << "TESTE" << endl;
        cout << "start: " << start[0] << ", " << start[1] << ", " << start[2] << endl;
        cout << "goal: " << goal[0] << ", " << goal[1] << ", " << goal[2] << endl;
        cout << "FINAL DO TESTE" << endl;
    }
    else
    {
        start[0] = 0.5;
        start[1] = 0.5;
        start[2] = 0.5*boost::math::constants::pi<double>();;
        goal[0] = 5.5;
        goal[1] = 0.5;
        goal[2] = 0.5*boost::math::constants::pi<double>();
    }
    ss.setStartAndGoalStates(start, goal, 0.5);

    // this call is optional, but we put it in to get more output information
    ss.getSpaceInformation()->setStateValidityCheckingResolution(0.005);
    //ss.setup();
    //ss.print();

    // attempt to solve the problem within 30 seconds of planning time
    ob::PlannerStatus solved = ss.solve(30.0);

    if(solved){
        std::vector<double> reals;

        //ss.simplifySolution();
        og::PathGeometric path_geo = ss.getSolutionPath();
        //path_geo.interpolate(1000);
        path = PathGeometric2Path(path_geo);
        /*for(int i = 0 ; i < path.poses.size() ; i++){
            path.poses.at(i).x += 20;
            path.poses.at(i).y += 20;
        }
        //path_geo.printAsMatrix(std::cout);
    }*/
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
    
    //cout << x_i << ", " << y_i << endl;
    //start->setX(88);
    //start->setY(59);
    //start->setYaw(0);
    start->setX(x_i);
    start->setY(y_i);
    //start->setYaw(0);

    //goal->setX(100);
    //goal->setY(101);
    goal->setX(x_f);
    goal->setY(y_f);
    //goal->setYaw(0);


    ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));

    pdef->setStartAndGoalStates(start, goal, 0.1);

    //ob::PlannerPtr planner(new og::RRTConnect(si));
    ob::PlannerPtr planner(new og::RRT(si));          // GOOD
    //ob::PlannerPtr planner(new og::LazyRRT(si));      // GOOD
    //ob::PlannerPtr planner(new og::pRRT(si));

    //ob::PlannerPtr planner(new og::PRMstar(si));        // VERY GOOD

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

    /*ob::StateSpacePtr space(new ob::ReedsSheppStateSpace);

    ob::RealVectorBounds bounds(2);
    bounds.setLow(0);
    bounds.setHigh(170);

    space->as<ob::ReedsSheppStateSpace>()->setBounds(bounds);

    ob::SpaceInformationPtr si(new ob::SpaceInformation(space));

    si->setStateValidityChecker(std::bind(&isStateValid, std::placeholders::_1));

    ob::ScopedState<ob::ReedsSheppStateSpace> start(space);
    ob::ScopedState<ob::ReedsSheppStateSpace> goal(space);

    start->setX((int)robots.at(id_robot).x);
    start->setY((int)robots.at(id_robot).y);
    start->setYaw(0);

    goal->setX(goal_pose.x);
    goal->setY(goal_pose.y);
    goal->setYaw(0);

    ob::ProblemDefinitionPtr pdef(new ob::ProblemDefinition(si));

    pdef->setStartAndGoalStates(start, goal);

    ob::PlannerPtr planner(new og::RRTConnect(si));

    planner->setProblemDefinition(pdef);

    planner->setup();

    //si->printSettings(std::cout);

    //pdef->print(std::cout);

    ob::PlannerStatus solved = planner->solve(1.0);

    if(solved){
        ob::PathPtr path_ptr = pdef->getSolutionPath();
        path = PathPtr2Path(path_ptr);
    }*/

    /* construct the state space we are planning in
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
        goal.random();

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
       }*/

    return path;
}

bool PathPlanning::isStateValid(const ob::State *state){
    bool ok = true;
    const ob::SE2StateSpace::StateType* state2D = state->as<ob::SE2StateSpace::StateType>();
    double x = state2D->getX();
    double y = state2D->getY();

    if(x < 10 || x > 150 || y < 10 || y > 110){
        ok = false;
    }

    /*for(int i = 0 ; i < robotsStatic.size() ; i++){
        //robotsStatic.at(i).show();
        double dis = sqrt((x-robotsStatic.at(i).x)*(x-robotsStatic.at(i).x) + (y-robotsStatic.at(i).y)*(y-robotsStatic.at(i).y)) - 1.0;
        if(dis < 0.0){
            ok = false;
        }
    }*/
    
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