//
// Created by john on 07/10/18.
//

#include <Communications/StateReceiverAdapter.h>
#include <Communications/StateReceiver.h>

StateReceiverAdapter::StateReceiverAdapter(vss::ExecutionConfig &executionConfig) {
    this->executionConfig = executionConfig;

    this->stateReceiver = new vss::StateReceiver();
    this->stateReceiver->createSocket(this->executionConfig);
}

vss::State StateReceiverAdapter::receive(vss::FieldTransformationType fieldTransformationType) {
    return this->stateReceiver->receiveState(fieldTransformationType);
}

