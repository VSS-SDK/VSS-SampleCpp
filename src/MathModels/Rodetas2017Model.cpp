//
// Created by john on 12/10/18.
//

#include <cmath>
#include <MathModels/Rodetas2017Model.h>
#include <Domain/BlazicParameters.h>
#include <Helpers/Math.h>

vss::WheelsCommand Rodetas2017Model::getCommand(vss::Robot actual, vss::Robot objective) {
    if(actual == objective)
        return vss::WheelsCommand(0,0);

    auto wheelsCommand = vss::WheelsCommand(10, -10);

    float standardPower = 20;
    auto angleToObjective = vss::Math::angleBetween(actual, objective);
    float correctionPower = (standardPower) * sin( angleToObjective - actual.angle );

    wheelsCommand.leftVel = (standardPower + correctionPower);
    wheelsCommand.rightVel = (standardPower - correctionPower);

    return wheelsCommand;
}

