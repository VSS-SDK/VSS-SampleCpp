//
// Created by john on 12/10/18.
//

#include <Factories/StrategyFactory.h>
#include <Strategies/DefaultAttackerStrategy.h>
#include <Strategies/UndefinedStrategy.h>

StrategyFactory::StrategyFactory() {

}

IStrategy *StrategyFactory::create(StrategyType strategyType) {
    switch (strategyType) {
        case StrategyType::DefaultAttacker:
            return new DefaultAttackerStrategy();
        default:
            return new UndefinedStrategy();
    }
}