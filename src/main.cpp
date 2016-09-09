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

bool argParse(int argc, char** argv, string *color, bool *debug, bool *real);

int main(int argc, char** argv){
    srand(time(NULL));
	string color;
    bool debug = false;
    bool real_environment = false;

	if(argParse(argc, argv, &color, &debug, &real_environment)){
        /*if(debug){
            cout << "debug ON";
        }*/
        if(color == "yellow" || color == "blue"){
		    Strategy strategy;
		    strategy.init(color, debug, real_environment);
        }else{
            cerr << "ERROR: Your main color must be yellow or blue." << endl;
        }
        
	}else{
		cerr << "ERROR: You must enter a main color." << endl;
	}

	return 0;
}

bool argParse(int argc, char** argv, string *color, bool *debug, bool *real){
    namespace bpo = boost::program_options;

    // Declare the supported options.
    bpo::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "(Optional) produce help message")
        ("debug,d", "(Optional) open the debug rotine")
        ("real,r", "(Optional) open the real transmition")
        ("color,c", bpo::value<std::string>()->default_value(" "), "(Required) Specify the main color of your team, may be yellow or blue.");
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

    *color = vm["color"].as<string>();

    if(*color == " "){
        return false;
    }

    return true;
}