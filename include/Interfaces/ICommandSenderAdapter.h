//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_ICOMMANDSENDERADAPTER_H
#define VSS_SAMPLE_ICOMMANDSENDERADAPTER_H

#include <Domain/Command.h>

class ICommandSenderAdapter {
public:
    virtual void send(vss::Command) = 0;
    virtual std::string getType() = 0;
};

#endif //VSS_SAMPLE_ICOMMANDSENDERADAPTER_H
