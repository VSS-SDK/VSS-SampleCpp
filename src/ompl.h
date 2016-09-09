#ifndef _PLANNERS_H_
#define _PLANNERS_H_

#include <ompl/base/SpaceInformation.h>
#include <ompl/base/spaces/SE2StateSpace.h>
#include <ompl/base/spaces/SE3StateSpace.h>
#include <ompl/base/spaces/DubinsStateSpace.h>
#include <ompl/base/spaces/ReedsSheppStateSpace.h>
#include <ompl/geometric/SimpleSetup.h>
#include <ompl/base/Path.h>
#include <ompl/base/State.h>
#include <ompl/base/ScopedState.h>

// apt-get install on debian
#include <ompl/geometric/planners/rrt/RRTConnect.h>
#include <ompl/geometric/planners/prm/PRMstar.h>

// download from site include this path planners
//#include <ompl/geometric/planners/bitstar/BITstar.h>
//#include <ompl/geometric/planners/cforest/CForest.h>
//#include <ompl/geometric/planners/fmt/FMT.h>
//#include <ompl/geometric/planners/rrt/InformedRRTstar.h>


#include <ompl/config.h>

#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/objectives/StateCostIntegralObjective.h>
#include <ompl/base/objectives/MaximizeMinClearanceObjective.h>
#include <ompl/base/spaces/RealVectorStateSpace.h>
#include "ompl/util/Console.h"

namespace ob = ompl::base;
namespace og = ompl::geometric;

enum typePlanner
{
    PLANNER_BITSTAR,
    PLANNER_CFOREST,
    PLANNER_FMTSTAR,
    PLANNER_INF_RRTSTAR,
    PLANNER_PRMSTAR,
    PLANNER_RRTSTAR
};

enum planningObjective
{
    OBJECTIVE_PATHCLEARANCE,
    OBJECTIVE_PATHLENGTH,
    OBJECTIVE_THRESHOLDPATHLENGTH,
    OBJECTIVE_WEIGHTEDCOMBO
};

#endif