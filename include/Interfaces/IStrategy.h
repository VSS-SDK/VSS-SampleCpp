//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_ISTRATEGY_H
#define VSS_SAMPLE_ISTRATEGY_H

#include <Domain/WheelsCommand.h>
#include <Domain/Robot.h>
#include <vector>
#include <Domain/Ball.h>

class IStrategy {
public:
    virtual vss::Robot getObjectivePose(vss::Robot, std::vector<vss::Robot>, std::vector<vss::Robot>, vss::Ball) = 0;
};

#endif //VSS_SAMPLE_ISTRATEGY_H
