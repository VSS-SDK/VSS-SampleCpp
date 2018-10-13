//
// Created by john on 12/10/18.
//

#include <RobotStrategy.h>
#include <Domain/ExecutionConfig.h>
#include <Factories/StrategyFactory.h>
#include <MathModels/Rodetas2017Model.h>

RobotStrategy::RobotStrategy(unsigned int index, StrategyType strategyType, vss::ExecutionConfig& executionConfig) {
    this->strategyFactory = new StrategyFactory();
    this->executionConfig = executionConfig;

    this->index = index;
    this->strategy = strategyFactory->create(strategyType);
    this->mathModel = new Rodetas2017Model();
}

vss::WheelsCommand RobotStrategy::getCommand(vss::State state) {
    this->state = state;

    if(executionConfig.teamType == vss::TeamType::Yellow){
        auto objective = this->strategy->getObjectivePose(state.teamYellow[index], getOtherTeamRobots(state.teamYellow), state.teamBlue, state.ball);
        return this->mathModel->getCommand(state.teamYellow[index], objective);
    } else {
        auto objective = this->strategy->getObjectivePose(state.teamBlue[index], getOtherTeamRobots(state.teamBlue), state.teamYellow, state.ball);
        return this->mathModel->getCommand(state.teamBlue[index], objective);
    }
}

std::vector<vss::Robot> RobotStrategy::getOtherTeamRobots(std::vector<vss::Robot> teamRobots) {
    std::vector<vss::Robot> filteredRobots;

    for(unsigned int i = 0 ; i < teamRobots.size() ; i++) {
        if(this->index != i)
            filteredRobots.push_back(teamRobots[i]);
    }

    return filteredRobots;
}
