//
// Created by john on 12/10/18.
//

#include <MathModels/BlazicModel.h>
#include <cmath>
#include <Domain/BlazicParameters.h>


vss::WheelsCommand BlazicModel::getCommand(vss::Robot actual, vss::Robot objective) {
    if(actual == objective)
        return vss::WheelsCommand(0,0);

    auto wheelsCommand = vss::WheelsCommand(10, -10);

    /*for(double v = -20.0 ; v < 20.0 ; v = v + 0.5){
        for(double w = -20.0 ; w < 20.0 ; w = w + 0.5){
            for(double theta = -M_PI ; theta < M_PI ; theta = theta + 0.1){
                auto pose = vss::Pose(cos(theta) * v, sin(theta) * v, w);
                std::cout << pose << std::endl;
            }

        }
    }*/

    return wheelsCommand;
}

double BlazicModel::getFit(vss::Pose difference, vss::Pose act) {
    
    return 0;
}

