CPP=g++
CARGS=-std=c++17 -Wall -Werror -O0 -g3 -m64
INC=-I src
MKDIR=mkdir -p
RM=rm -rf
SRC=$(PWD)/src
BIN=$(PWD)/bin

clean:
	@echo "--> Cleaning compiled..."
	$(RM) $(BIN)
	@echo

$(BIN)/%.o: $(SRC)/%.cpp
	@echo "--> Compiling $<..."
	$(MKDIR) $(@D)
	$(CPP) $(CARGS) -c $< -o $@ $(INC)
	@echo

$(BIN)/exec/instance_parser_exec : $(BIN)/instance/instance.o \
                                   $(BIN)/exec/instance_parser_exec.o
	@echo "--> Linking objects..."
	$(CPP) -o $@ $^ $(CARGS) $(INC)
	@echo

instance_parser_exec : $(BIN)/exec/instance_parser_exec

execs : instance_parser_exec

all : execs
