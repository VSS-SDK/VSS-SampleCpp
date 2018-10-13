//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_TEAM_H
#define VSS_SAMPLE_TEAM_H

#include <Domain/State.h>
#include <Domain/Command.h>
#include <Domain/TeamType.h>
#include <Domain/ExecutionConfig.h>
#include "RobotStrategy.h"

class Team {
public:
    Team(vss::ExecutionConfig&);

    vss::Command& getCommandFromState(vss::State&);

    void setTeamType(vss::TeamType);
    vss::TeamType getTeamType();

private:
    vss::ExecutionConfig executionConfig;

    vss::State state;
    vss::Command command;

    std::vector<RobotStrategy> robots;
};

#endif //VSS_SAMPLE_TEAM_H
