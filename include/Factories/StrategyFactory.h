//
// Created by john on 12/10/18.
//

#ifndef VSS_SAMPLE_STRATEGYFACTORY_H
#define VSS_SAMPLE_STRATEGYFACTORY_H

#include <Interfaces/IStrategyFactory.h>

class StrategyFactory : public IStrategyFactory {
public:
    StrategyFactory();

    IStrategy* create(StrategyType) override;
};

#endif //VSS_SAMPLE_STRATEGYFACTORY_H
