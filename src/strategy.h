/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "math.h"
#include "thread"		// C++11
#include "common.h"
#include "interface.h"
#include "vector"

using namespace std;
using namespace common;

class Strategy{
protected:
	string main_color;
	Interface interface_receive, interface_send;
	vss_state::Global_State global_state;
	vss_command::Global_Commands global_commands;
	
	thread *thread_receive;
    thread *thread_send;
    common::State state;
	common::Command commands[3];

    int port;
    int situation;
	float robot_radius, force_kick, distance_stop;
    bool has_new_state, has_new_command;
	bool its_real_transmition;
public:
	Strategy();	
	void init(string main_color);

	void receive_thread();
	void send_thread();

	void calc_strategy();
	common::btVector3 project_bt_to(btVector3 ball, btVector3 goal, float proj_dist);
	common::Command calc_cmd_to(btVector3 init, btVector3 final, float distance_to_stop = 10.0);
	common::Command kick_to(btVector3 robot, btVector3 kick_goal);
	
	void play();
	bool position_our_goal();
	float handleAngle(float, float);
};

#endif // _STRATEGY_H_