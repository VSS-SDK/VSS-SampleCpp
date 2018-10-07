//
// Created by john on 07/10/18.
//

#include <Communications/CommandSender.h>
#include <Communications/CommandSenderAdapter.h>

CommandSenderAdapter::CommandSenderAdapter(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;

    this->commandSender = new vss::CommandSender();
    this->commandSender->createSocket(this->executionConfig);
}

void CommandSenderAdapter::send(vss::Command command) {
    this->commandSender->sendCommand(command);
}

