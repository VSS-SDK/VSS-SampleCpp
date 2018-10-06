/*
 * This file is part of the VSS-SampleStrategy project.
 *
 * This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
 * v. 3.0. If a copy of the GPL was not distributed with this
 * file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
 */

#include <Communications/DebugSender.h>

using namespace vss;

IDebugSender *debugSender;

int main(int argc, char** argv){
    debugSender = new DebugSender();

    debugSender->createSocket(TeamType::Yellow);

    while(true){
        vss::Debug debug;

        for(unsigned int i = 0 ; i < 3 ; i++){
            debug.stepPoints.push_back(Point(85 + rand()%20, 65 + rand()%20));
        }

        for(unsigned int i = 0 ; i < 3 ; i++){
            debug.finalPoses.push_back(Pose(85 + rand()%20, 65 + rand()%20, rand()%20));
        }

        for(unsigned int i = 0 ; i < 3 ; i++){
            vss::Path path;
            path.points.push_back(Point(85, 65));
            path.points.push_back(Point(85 + rand()%20, 65 + rand()%20));
        }

        debugSender->sendDebug(debug);
    }

    return 0;
}