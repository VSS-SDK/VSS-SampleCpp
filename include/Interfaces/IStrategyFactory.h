//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_ISTRATEGYFACTORY_H
#define VSS_SAMPLE_ISTRATEGYFACTORY_H

#include <Domain/StrategyType.h>
#include "IStrategy.h"

class IStrategyFactory {
public:
    virtual IStrategy* create(StrategyType) = 0;
};

#endif //VSS_SAMPLE_ISTRATEGYFACTORY_H
