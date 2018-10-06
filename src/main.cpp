/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/StateReceiver.h>
#include <Communications/CommandSender.h>
#include <Communications/DebugSender.h>
#include <Builders/StdinInterpreterBuilder.h>
#include "cstdlib"

using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSender;
IDebugSender *debugSender;

State state;

void send_commands();
void send_debug();

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

    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSender = new CommandSender();
    debugSender = new DebugSender();

    stateReceiver->createSocket(executionConfig);
    commandSender->createSocket(executionConfig);
    debugSender->createSocket(executionConfig);

    while(true){
        state = stateReceiver->receiveState(FieldTransformationType::None);
        std::cout << state << std::endl;

        send_commands();
        send_debug();
    }

    return 0;
}

void send_commands(){
    Command command;

    for(int i = 0 ; i < 3 ; i++){
        command.commands.push_back(WheelsCommand(10, -10));
    }

    commandSender->sendCommand(command);
}


void send_debug(){
    vss::Debug debug;

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.stepPoints.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        debug.finalPoses.push_back(Pose(85 + rand()%20, 65 + rand()%20, rand()%20));
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Path path;
        path.points.push_back(Point(85, 65));
        path.points.push_back(Point(85 + rand()%20, 65 + rand()%20));
    }

    debugSender->sendDebug(debug);
}
