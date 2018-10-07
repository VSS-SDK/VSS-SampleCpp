//
// Created by john on 07/10/18.
//

#include <Factories/CommandSenderFactory.h>
#include <Communications/SimCommandSenderAdapter.h>
#include <Communications/RealCommandSenderAdapter.h>

ICommandSenderAdapter* CommandSenderFactory::create(vss::ExecutionConfig &executionConfig) {
    switch (executionConfig.environmentType) {
        case vss::EnvironmentType::Simulation:
            return new SimCommandSenderAdapter(executionConfig);
        case vss::EnvironmentType::Real:
            return new RealCommandSenderAdapter(executionConfig);
        default:
            return new SimCommandSenderAdapter(executionConfig);
    }
}

