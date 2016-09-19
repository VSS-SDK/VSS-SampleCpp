/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "sample.h"
#include "robot.h"
#include "SirSoccer-COM/Communication.h"

using namespace std;
using namespace common;

class Strategy : public Sample{
	/*
		Sample :: string main_color;
		Sample :: bool is_debug;
		Sample :: bool real_environment;
		Sample :: int situation;

		Sample :: common::State state;
		Sample :: common::Command commands[3];
		Sample :: common::Debug debug;
	*/
protected:
	Communication comm;
	vector<Robot> our_team;
	vector<Robot> adversary_team;
	btVector3 ball, v_ball;

public:
	Strategy();	

	void init(string main_color, bool is_debug, bool real_environment, Goal goal);
	void loop();
	void calc_strategy();

	void update_state_on_robots();
	void update_commands_from_robots();
	void update_debug_from_robots();
};

#endif // _STRATEGY_H_