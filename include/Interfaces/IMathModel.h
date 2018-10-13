//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_IMATHMODEL_H
#define VSS_SAMPLE_IMATHMODEL_H

#include <Domain/WheelsCommand.h>
#include <Domain/Robot.h>

class IMathModel {
public:
    virtual vss::WheelsCommand getCommand(vss::Robot actual, vss::Robot objective) = 0;
};

#endif //VSS_SAMPLE_IMATHMODEL_H
