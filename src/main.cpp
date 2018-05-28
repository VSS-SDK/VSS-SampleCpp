/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <StateReceiver.h>
#include <CommandSender.h>
#include "cstdlib"
#include "interface.h"

using namespace vss;

StateReceiver stateReceiver;
State state;

CommandSender commandSender;
Command command;

Interface interface_debug;                      //! Interface de envio de informações de debug visual
vss_debug::Global_Debug global_debug;           //! Pacote que define o debug visual

void send_commands();
void send_debug();

int main(int argc, char** argv){
    srand(time(NULL));

    stateReceiver.createSocket(); //! VSS-Vision e VSS-Simulator abrem um socket na porta 5555
	commandSender.createSocket(TeamType::Yellow);

    interface_debug.createSendDebugTeam1(&global_debug, "tcp://localhost:5558");        //! Team2 abre o socket na porta 5559

    while(true){
    	state = stateReceiver.receiveState(FieldTransformationType::None);
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

	commandSender.sendCommand(command);
}

void send_debug(){
	global_debug = vss_debug::Global_Debug();                       // Devemos limpar, pois funciona como um vector

	for(int i = 0 ; i < 3 ; i++){                                   // Todos os robôs terão o vetor de movimentação variado perto deles mesmos
		vss_debug::Pose *steps = global_debug.add_step_poses();
		steps->set_id(i);
		steps->set_x(state.teamYellow[i].x - 10 + rand()%20);
		steps->set_y(state.teamYellow[i].y - 10 + rand()%20);
		steps->set_yaw(state.teamYellow[i].angle);
	}

	for(int i = 0 ; i < 3 ; i++){                                   // Todos os robôs terão a pose final variada perto da bola
		vss_debug::Pose *finals = global_debug.add_final_poses();
		finals->set_id(i);
		finals->set_x(state.teamYellow[i].x - 10 + rand()%20);
		finals->set_y(state.teamYellow[i].y - 10 + rand()%20);
		finals->set_yaw(0);
	}

	for(int i = 0 ; i < 3 ; i++){                                    // Todos os robôs teram um caminho que leva para a bola
		vss_debug::Path *paths = global_debug.add_paths();
		paths->set_id(i);

        // Pose do robô
        vss_debug::Pose *poses1 = paths->add_poses();
        poses1->set_id(i);
        poses1->set_x(state.teamYellow[i].x);
        poses1->set_y(state.teamYellow[i].y);
        poses1->set_yaw(0);

        // Pose da bola
        vss_debug::Pose *poses2 = paths->add_poses();
        poses2->set_id(i);
        poses2->set_x(state.ball.x - 10 + rand()%20);
        poses2->set_y(state.ball.y - 10 + rand()%20);
        poses2->set_yaw(0);
	}

	interface_debug.sendDebugTeam1();
}
