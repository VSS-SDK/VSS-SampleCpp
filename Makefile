#
# This file is part of the VSS-SampleStrategy project.
#
# This Source Code Form is subject to the terms of the GNU GENERAL PUBLIC LICENSE,
# v. 3.0. If a copy of the GPL was not distributed with this
# file, You can obtain one at http://www.gnu.org/licenses/gpl-3.0/.
#


CCX = g++ -std=c++11 -w



INCLUDES = -Isrc -Isrc/interface

LIBRARIES_PROTO = `pkg-config --cflags --libs protobuf`

LIBRARIES = $(LIBRARIES_PROTO) -lzmq -pthread -lm -w



SRC := $(shell find -name '*.cpp')
FILE_NAMES_SRC = $(SRC:.cpp=.o)

PROTOS := $(shell find -name '*.cc')
FILE_NAMES_PROTOS = $(PROTOS:.cc=.o)

FILE_NAMES = $(FILE_NAMES_SRC) $(FILE_NAMES_PROTOS) 



EXEC = VSS-SampleStrategy
.cpp.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

.cc.o:
	@$(CCX) $(INCLUDES) $(LIBRARIES) -Wall -Wformat -c -o $@ $< -w

all: message_compiling $(EXEC)
	@echo Done ...
	
message_compiling:
	@echo Compiling VSS-SampleStrategy ...

message_cleaning:
	@echo Cleaning VSS-SampleStrategy ...

run:
	./$(EXEC)

$(EXEC): proto $(FILE_NAMES)
	@$(CCX) -o $(EXEC) $(FILE_NAMES) $(LIBRARIES) $(INCLUDES)

clean: message_cleaning
	@rm $(EXEC) $(FILE_NAMES)

proto:
	cd src/interface/protos && make -f protos.make

teste: 
	$(SRC)
