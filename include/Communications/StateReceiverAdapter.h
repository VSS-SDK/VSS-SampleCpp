//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_STATERECEIVERADAPTER_H
#define VSS_SAMPLE_STATERECEIVERADAPTER_H

#include <Interfaces/IStateReceiverAdapter.h>
#include <Interfaces/IStateReceiver.h>
#include <Domain/ExecutionConfig.h>
#include <Domain/State.h>

class StateReceiverAdapter : public IStateReceiverAdapter {
public:
    vss::IStateReceiver *stateReceiver;
    vss::ExecutionConfig executionConfig;

    StateReceiverAdapter(vss::ExecutionConfig&);

    vss::State receive(vss::FieldTransformationType) override;
};

#endif //VSS_SAMPLE_STATERECEIVERADAPTER_H
