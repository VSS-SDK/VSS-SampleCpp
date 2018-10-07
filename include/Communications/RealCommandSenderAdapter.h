//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_REALCOMMANDSENDERADAPTER_H
#define VSS_SAMPLE_REALCOMMANDSENDERADAPTER_H

#include <Interfaces/ICommandSenderAdapter.h>
#include <Domain/ExecutionConfig.h>
#include <Interfaces/ICommandSender.h>

class RealCommandSenderAdapter : public ICommandSenderAdapter {
public:
    RealCommandSenderAdapter(vss::ExecutionConfig&);

    void send(vss::Command) override;
    std::string getType() override;

private:
    const std::string type = "RealCommandSenderAdapter";

    vss::ExecutionConfig executionConfig;
};

#endif //VSS_SAMPLE_REALCOMMANDSENDERADAPTER_H
