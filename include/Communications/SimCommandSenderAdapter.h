//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_COMMANDSENDERADAPTER_H
#define VSS_SAMPLE_COMMANDSENDERADAPTER_H

#include <Interfaces/ICommandSenderAdapter.h>
#include <Interfaces/ICommandSender.h>
#include <Domain/ExecutionConfig.h>
#include <Domain/Command.h>

class SimCommandSenderAdapter : public ICommandSenderAdapter {
public:
    SimCommandSenderAdapter(vss::ExecutionConfig&);

    void send(vss::Command) override;

private:
    vss::ICommandSender *commandSender;
    vss::ExecutionConfig executionConfig;
};

#endif //VSS_SAMPLE_COMMANDSENDERADAPTER_H
