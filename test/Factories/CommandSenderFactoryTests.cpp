//
// Created by john on 07/10/18.
//

#include <googletest/googletest/include/gtest/gtest.h>
#include <Factories/CommandSenderFactory.h>
#include <Domain/ExecutionConfig.h>
#include <Communications/RealCommandSenderAdapter.h>

TEST(Create, WhenEnvironmentTypeIsReal_ShouldReturnRealCommandSenderAdapter){
    // Arrange
    auto executionConfig = vss::ExecutionConfig();
    auto commandSenderFactory = CommandSenderFactory();
    executionConfig.environmentType = vss::EnvironmentType::Real;
    auto realCommandSender = new RealCommandSenderAdapter(executionConfig);

    // Act
    auto commandSender = commandSenderFactory.create(executionConfig);

    // Assert
    EXPECT_EQ(commandSender->getType(), realCommandSender->getType());
}

TEST(Create, WhenEnvironmentTypeIsSimulation_ShouldReturnSimCommandSenderAdapter){
    // Arrange
    auto executionConfig = vss::ExecutionConfig();
    auto commandSenderFactory = CommandSenderFactory();
    executionConfig.environmentType = vss::EnvironmentType::Simulation;
    auto simCommandSender = new RealCommandSenderAdapter(executionConfig);

    // Act
    auto commandSender = commandSenderFactory.create(executionConfig);

    // Assert
    EXPECT_EQ(commandSender->getType(), simCommandSender->getType());
}