//
// Created by john on 07/10/18.
//

#include <Communications/DebugSenderAdapter.h>
#include <Communications/DebugSender.h>

DebugSenderAdapter::DebugSenderAdapter(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;

    this->debugSender = new vss::DebugSender();
    this->debugSender->createSocket(this->executionConfig);
}

void DebugSenderAdapter::send(vss::Debug debug) {
    this->debugSender->sendDebug(debug);
}

