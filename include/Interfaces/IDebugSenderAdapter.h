//
// Created by john on 07/10/18.
//

#ifndef VSS_SAMPLE_IDEBUGSENDERADAPTER_H
#define VSS_SAMPLE_IDEBUGSENDERADAPTER_H

#include <Domain/Debug.h>

class IDebugSenderAdapter {
public:
    virtual void send(vss::Debug) = 0;
};

#endif //VSS_SAMPLE_IDEBUGSENDERADAPTER_H
