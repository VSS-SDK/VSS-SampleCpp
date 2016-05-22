#include "strategy.h"

Strategy::Strategy(){
	
}

void Strategy::init(){
	thread_receive = new thread(bind(&Strategy::receive_thread, this));
	thread_send = new thread(bind(&Strategy::send_thread, this));

	thread_receive->join();
	thread_send->join();
}

void Strategy::receive_thread(){

}

void Strategy::send_thread(){

}