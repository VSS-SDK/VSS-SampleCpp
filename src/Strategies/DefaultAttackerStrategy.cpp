//
// Created by john on 12/10/18.
//

#include <Strategies/DefaultAttackerStrategy.h>

vss::Robot DefaultAttackerStrategy::getObjectivePose(vss::Robot selfState, std::vector<vss::Robot> teamRobots,
                                                             std::vector<vss::Robot> oponentRobots, vss::Ball ball) {

    return vss::Robot(ball.x, ball.y, 0, 0, 0, 0);
}

