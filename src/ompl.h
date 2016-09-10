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
#include <ompl/geometric/planners/rrt/RRT.h>
#include <ompl/geometric/planners/rrt/pRRT.h>
#include <ompl/geometric/planners/rrt/TRRT.h>
#include <ompl/geometric/planners/rrt/RRTstar.h>
#include <ompl/geometric/planners/rrt/LBTRRT.h>
#include <ompl/geometric/planners/rrt/LazyRRT.h>
#include <ompl/geometric/planners/rrt/RRTConnect.h>

#include <ompl/geometric/planners/prm/PRMstar.h>
#include <ompl/geometric/planners/prm/LazyPRM.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/planners/prm/SPARS.h>
#include <ompl/geometric/planners/prm/SPARStwo.h>

#include <ompl/geometric/planners/est/EST.h>

#include <ompl/geometric/planners/pdst/PDST.h>

#include <ompl/geometric/planners/sbl/SBL.h>
#include <ompl/geometric/planners/sbl/pSBL.h>

#include <ompl/geometric/planners/kpiece/BKPIECE1.h>
#include <ompl/geometric/planners/kpiece/KPIECE1.h>
#include <ompl/geometric/planners/kpiece/LBKPIECE1.h>

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