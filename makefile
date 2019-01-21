# Makefile for symkit 1.0 by Raffaele Ancarola 
# C++ library for physics symulations

NAME := flatland
BIN := bin/lib$(NAME).so
CC := g++
CFLAGS := -Wall -std=c++17

DIRS := engine 
BACKUP := backup
INSTALL_DIR := /usr/lib

INCLUDES := $(patsubst %,%/include,$(DIRS))

# other required libraries
LIBS := -lSDL2 #add libraries

SRC := $(wildcard $(patsubst %,%/*.cpp,$(DIRS)))
OBJ := $(patsubst %.cpp,bin/%.o,$(SRC))

.PHONY: dirs clean install backup restore
all: $(BIN)

# builds all binaries into the shared library

$(BIN): dirs $(OBJ) 
	@printf "\nAssembling binaries\n\n"
	$(CC) $(CFLAGS) -shared -o $@ $(OBJ) -I $(INCLUDES) $(LIBS)
	@printf "\nCompilation successfully completed\n"

# compile all sources

$(OBJ): bin/%.o : %.cpp $(SRC)
	@printf "\nCompiling $<\n"
	$(CC) $(CFLAGS) -c $< -fPIC -o $@ -I $(INCLUDES) $(LIBS)

# phony commands implementation

# install the compiled library into your system
# be careful with this command because it could not work
install:
	chmod +x install.sh
	./install.sh $(BIN) $(INSTALL_DIR)

# generate all necessaries directories
dirs:
	mkdir -p bin $(patsubst %,bin/%,$(DIRS))
	mkdir -p $(INCLUDES) $(BACKUP)
	@printf "Default directories created\n"

# clean all binaries
clean:
	rm -rfv bin/*
	@printf "Binary files cleaned\n"

# backup the project in backup/symkit.zip
backup:
	mkdir -p $(BACKUP)
	rm -rfv $(BACKUP)/*
	zip -r $(BACKUP)/symkit.zip $(DIRS)
	@printf "Backup completed\n"

# restore the last backup, backup/symkit.zip must be present
restore:
	unzip $(BACKUP)/symkit.zip -d $(BACKUP)
	rm -rfv $(DIRS)
	mv $(patsubst %,$(BACKUP)/%,$(DIRS)) . 
	@printf "Backup restored\n"

