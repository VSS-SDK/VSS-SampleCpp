/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "thread"		// C++11
#include "interface.h"

using namespace std;

class Strategy{
protected:
	Interface interface_receive, interface_send;
	vss_state::Global_State global_state;
	vss_command::Global_Commands global_commands;
	
	thread *thread_receive;
    thread *thread_send;

    int port;
    bool has_new_state, has_new_command;
	bool its_real_transmition;
public:
	Strategy();	
	void init(int port);

	void receive_thread();
	void send_thread();
	void calc_strategy();
};

#endif // _STRATEGY_H_