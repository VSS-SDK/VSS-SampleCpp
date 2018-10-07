/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Builders/StdinInterpreterBuilder.h>
#include <Kernel.h>

using namespace vss;

vss::ExecutionConfig loadExecutionConfig(int argc, char** argv){
    auto stdinInterpreterBuilder = new vss::StdinInterpreterBuilder();

    stdinInterpreterBuilder
            ->onStateRecvAddr()
            ->onStatePort()
            ->onYellowDebugSendAddr()
            ->onYellowDebugPort()
            ->onBlueDebugSendAddr()
            ->onBlueDebugPort()
            ->onBlueCmdSendAddr()
            ->onBlueCmdPort()
            ->onYellowCmdSendAddr()
            ->onYellowCmdPort()
            ->onEnvironmentType()
            ->onSideAttackType()
            ->onTeamType();

    auto stdinInterpreter = stdinInterpreterBuilder->buildInterpreter();

    return stdinInterpreter->extractExecutionConfig(argc, argv);
}


int main(int argc, char** argv){
    auto executionConfig = loadExecutionConfig(argc, argv);

    if(!executionConfig.isValidConfiguration)
        return 0;

    auto kernel = new Kernel(executionConfig);
    kernel->run();

    return 0;
}