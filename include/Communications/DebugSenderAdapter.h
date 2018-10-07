//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_DEBUGSENDERADAPTER_H
#define VSS_SAMPLE_DEBUGSENDERADAPTER_H

#include <Interfaces/IDebugSenderAdapter.h>
#include <Interfaces/IDebugSender.h>
#include <Domain/ExecutionConfig.h>
#include <Domain/Debug.h>

class DebugSenderAdapter : public IDebugSenderAdapter {
public:
    DebugSenderAdapter(vss::ExecutionConfig&);

    void send(vss::Debug) override;

private:
    vss::IDebugSender *debugSender;
    vss::ExecutionConfig executionConfig;
};

#endif //VSS_SAMPLE_DEBUGSENDERADAPTER_H
