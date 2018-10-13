//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_BLAZICMODEL_H
#define VSS_SAMPLE_BLAZICMODEL_H

#include <Interfaces/IMathModel.h>

class Rodetas2017Model : public IMathModel {
public:
    vss::WheelsCommand getCommand(vss::Robot actual, vss::Robot objective) override;
};

#endif //VSS_SAMPLE_BLAZICMODEL_H
