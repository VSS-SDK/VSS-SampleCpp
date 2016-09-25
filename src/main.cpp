/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include "sstream"
#include "strategy.h"
#include "boost.h"
#include "common.h"

bool argParse(int argc, char** argv, string *color, bool *debug, bool *real, Goal *goal, string *ip_receive_state, string *ip_send_debug, string *ip_send_command);

int main(int argc, char** argv){
    srand(time(NULL));
	string color;
    bool debug = false;
    bool real_environment = false;
    Goal goal = Goal::UNDEFINED;
    string ip_receive_state;
    string ip_send_debug;
    string ip_send_command;

	if(argParse(argc, argv, &color, &debug, &real_environment, &goal, &ip_receive_state, &ip_send_debug, &ip_send_command)){
        if(color == "yellow" || color == "blue"){
		    Strategy strategy;
		    strategy.init(color, debug, real_environment, goal, ip_receive_state, ip_send_debug, ip_send_command);
        }else{
            cerr << "ERROR: Your main color must be yellow or blue." << endl;
        }
	}else{
		cerr << "ERROR: You must enter a main color and a Side of Attack." << endl;
	}

	return 0;
}

bool argParse(int argc, char** argv, string *color, bool *debug, bool *real, Goal *goal, string *ip_receive_state, string *ip_send_debug, string *ip_send_command){
    namespace bpo = boost::program_options;

    // Declare the supported options.
    bpo::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "(Optional) produce help message")
        ("debug,d", "(Optional) open the debug rotine")
        ("real,r", "(Optional) open the real transmition")
        ("ip_receive_state,i", bpo::value<std::string>()->default_value("localhost"), "(Optional) Specify the IP from pc it's running VSS-Vision.")
        ("ip_send_debug,I", bpo::value<std::string>()->default_value("localhost"), "(Optional) Specify the IP from pc it's running VSS-Viewer.")
        ("ip_send_command,s", bpo::value<std::string>()->default_value("localhost"), "(Optional) Specify the IP from pc it's running VSS-Simulator.")
        ("goal,g", bpo::value<std::string>()->default_value(" "), "(Required) specify the goal that your team attacks, may be left or right.")
        ("color,c", bpo::value<std::string>()->default_value(" "), "(Required) specify the main color of your team, may be yellow or blue.");
    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
    bpo::notify(vm);

    if (vm.count("help")){
        std::cout << desc << std::endl;
        return false;
    }

    if (vm.count("debug")){
        *debug = true;
    }

     if (vm.count("real")){
        *real = true;
    }

    if(vm["goal"].as<string>() == " "){
        return false;
    }else{
        if(vm["goal"].as<string>() == "left"){
            *goal = Goal::LEFT;
        }else{
            *goal = Goal::RIGHT;
        }
    }

    *ip_receive_state = vm["ip_receive_state"].as<string>();    

    *ip_send_debug = vm["ip_send_debug"].as<string>();

    *ip_send_command = vm["ip_send_command"].as<string>();

    *color = vm["color"].as<string>();

    if(*color == " "){
        return false;
    }

    return true;
}