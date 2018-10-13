//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_UNDEFINEDSTRATEGY_H
#define VSS_SAMPLE_UNDEFINEDSTRATEGY_H

#include <Interfaces/IStrategy.h>

class UndefinedStrategy : public IStrategy {
public:
    vss::Robot getObjectivePose(vss::Robot, std::vector<vss::Robot>, std::vector<vss::Robot>, vss::Ball) override;
};

#endif //VSS_SAMPLE_UNDEFINEDSTRATEGY_H
