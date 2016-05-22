#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include "thread"		// C++11
#include "interface.h"

using namespace std;

class Strategy{
private:
	vss_state::Global_State global_state;
	vss_command::Global_Commands global_commands;
	
	thread *thread_receive;
    thread *thread_send;
public:
	Strategy();	
	void init();

	void receive_thread();
	void send_thread();
};

#endif // _STRATEGY_H_