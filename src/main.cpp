/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/StateReceiver.h>

using namespace vss;

IStateReceiver *stateReceiver;

int main(int argc, char** argv){
    stateReceiver = new StateReceiver();

    stateReceiver->createSocket();

    while(true){
        auto state = stateReceiver->receiveState(FieldTransformationType::None);
        std::cout << state << std::endl;
    }

    return 0;
}