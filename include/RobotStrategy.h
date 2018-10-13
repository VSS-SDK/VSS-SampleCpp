//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_ROBOTSTRATEGY_H
#define VSS_SAMPLE_ROBOTSTRATEGY_H


#include <Domain/WheelsCommand.h>
#include <Domain/State.h>
#include <Domain/StrategyType.h>
#include <Interfaces/IStrategy.h>
#include <Interfaces/IStrategyFactory.h>
#include <Domain/ExecutionConfig.h>
#include <Interfaces/IMathModel.h>

class RobotStrategy {
public:
    RobotStrategy(unsigned int, StrategyType, vss::ExecutionConfig&);

    vss::WheelsCommand getCommand(vss::State);

private:
    unsigned int index;
    IStrategy *strategy;
    IMathModel *mathModel;

    IStrategyFactory *strategyFactory;

    vss::State state;
    vss::ExecutionConfig executionConfig;

    std::vector<vss::Robot> getOtherTeamRobots(std::vector<vss::Robot> teamRobots);
};

#endif //VSS_SAMPLE_ROBOTSTRATEGY_H
