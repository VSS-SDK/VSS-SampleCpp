/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <StateReceiver.h>
#include <CommandSender.h>
#include <DebugSender.h>
#include "cstdlib"
#include "interface.h"

using namespace vss;

IStateReceiver *stateReceiver;
ICommandSender *commandSender;
IDebugSender *debugSender;

State state;

void send_commands();
void send_debug();

int main(int argc, char** argv){
    srand(time(NULL));

    stateReceiver = new StateReceiver();
    commandSender = new CommandSender();
    debugSender = new DebugSender();

    stateReceiver->createSocket();
    commandSender->createSocket(TeamType::Yellow);
    debugSender->createSocket(TeamType::Yellow);

    while(true){
        state = stateReceiver->receiveState(FieldTransformationType::None);
        send_commands();
        send_debug();
    }

    return 0;
}

void send_commands(){
    Command command;

    command.id = 0;

    for(int i = 0 ; i < 3 ; i++){
        WheelsCommand wheelsCommand;

        wheelsCommand.id = i;
        wheelsCommand.leftVel = 10;
        wheelsCommand.rightVel = -10;

        command.commands.push_back(wheelsCommand);
    }

    commandSender->sendCommand(command);
}

void send_debug(){
    vss::Debug debug;

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Point point;

        point.x = state.teamYellow[i].x - 10 + rand()%20;
        point.y = state.teamYellow[i].y - 10 + rand()%20;

        debug.stepPoints.push_back(point);
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Pose pose;

        pose.x = state.teamYellow[i].x - 10 + rand()%20;
        pose.y = state.teamYellow[i].y - 10 + rand()%20;
        pose.angle = state.teamYellow[i].y - 10 + rand()%20;

        debug.finalPoses.push_back(pose);
    }

    for(unsigned int i = 0 ; i < 3 ; i++){
        vss::Path path;
        vss::Point point_1;
        vss::Point point_2;

        point_1.x = state.teamYellow[i].x;
        point_1.y = state.teamYellow[i].y;

        point_2.x = state.ball.x - 10 + rand()%20;
        point_2.y = state.ball.y - 10 + rand()%20;

        path.points.push_back(point_1);
        path.points.push_back(point_2);
    }

    debugSender->sendDebug(debug);
}
