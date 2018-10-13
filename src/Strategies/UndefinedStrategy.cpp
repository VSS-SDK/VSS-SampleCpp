//
// Created by john on 12/10/18.
//

#include <Strategies/UndefinedStrategy.h>

vss::Robot UndefinedStrategy::getObjectivePose(vss::Robot selfState, std::vector<vss::Robot> teamRobots,
                                                       std::vector<vss::Robot> oponentRobots, vss::Ball ball) {

    return selfState;
}
