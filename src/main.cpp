/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */


#include "interface.h"

int main(){
	Interface interface;
	vss_state::Global_State global_state;
	interface.createLoopReceiveState(&global_state);
}