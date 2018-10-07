//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_COMMANDSENDERFACTORY_H
#define VSS_SAMPLE_COMMANDSENDERFACTORY_H

#include <Interfaces/ICommandSenderFactory.h>

class CommandSenderFactory : public ICommandSenderFactory {
public:
    ICommandSenderAdapter* create(vss::ExecutionConfig&) override;
};

#endif //VSS_SAMPLE_COMMANDSENDERFACTORY_H
