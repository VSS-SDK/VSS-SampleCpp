//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_ICOMMANDSENDERFACTORY_H
#define VSS_SAMPLE_ICOMMANDSENDERFACTORY_H

#include <Domain/ExecutionConfig.h>
#include "ICommandSenderAdapter.h"

class ICommandSenderFactory {
public:
    virtual ICommandSenderAdapter* create(vss::ExecutionConfig&) = 0;
};

#endif //VSS_SAMPLE_ICOMMANDSENDERFACTORY_H
