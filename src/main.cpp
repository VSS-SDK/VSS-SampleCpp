/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/CommandSender.h>
#include <zconf.h>

using namespace vss;

ICommandSender *commandSender;

int main(int argc, char** argv){
    commandSender = new CommandSender();

    commandSender->createSocket(TeamType::Yellow);

    while(true){
        usleep(33000);
        Command command;

        for(int i = 0 ; i < 3 ; i++){
            command.commands.push_back(WheelsCommand(10, -10));
        }

        commandSender->sendCommand(command);
    }

    return 0;
}