//
// Created by john on 12/10/18.
//

#include <Team.h>
#include <Domain/ExecutionConfig.h>

Team::Team(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;

    robots.push_back(RobotStrategy(0, StrategyType::DefaultAttacker, executionConfig));
    robots.push_back(RobotStrategy(1, StrategyType::Undefined, executionConfig));
    robots.push_back(RobotStrategy(2, StrategyType::Undefined, executionConfig));
}

vss::Command& Team::getCommandFromState(vss::State& state) {
    this->state = state;
    this->command = vss::Command();

    for(auto robot : this->robots){
        auto wheelsCommand = robot.getCommand(state);
        command.commands.push_back(wheelsCommand);
    }

    return this->command;
}

void Team::setTeamType(vss::TeamType teamType) {
    this->executionConfig.teamType = teamType;
}

vss::TeamType Team::getTeamType() {
    return this->executionConfig.teamType;
}

