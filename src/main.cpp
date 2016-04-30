#include "interface.h"

int main(){
	Interface interface;
	vss_state::Global_State global_state;
	interface.createLoopReceiveState(&global_state);
}