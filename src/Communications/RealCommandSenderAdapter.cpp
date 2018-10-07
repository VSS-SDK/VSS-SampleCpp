//
// Created by john on 07/10/18.
//

#include <Communications/RealCommandSenderAdapter.h>

RealCommandSenderAdapter::RealCommandSenderAdapter(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;
}

void RealCommandSenderAdapter::send(vss::Command command) {
    // Envia o comando para os robôs reais
}

std::string RealCommandSenderAdapter::getType() {
    return type;
}
