//
// Created by john on 07/10/18.
//

#include <Communications/CommandSender.h>
#include <Communications/SimCommandSenderAdapter.h>

SimCommandSenderAdapter::SimCommandSenderAdapter(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;

    this->commandSender = new vss::CommandSender();
    this->commandSender->createSocket(this->executionConfig);
}

void SimCommandSenderAdapter::send(vss::Command command) {
    this->commandSender->sendCommand(command);
}

std::string SimCommandSenderAdapter::getType() {
    return type;
}

