BUILD_DIR := obj

CPP := g++
CC  := gcc
LD := g++

CFLAGS :=
LDFLAGS := -lcurses

DATA_DIR := ../../data
DATA_LINK_TARGET := data

OMEGA_SRC_DIR := ../../Omega/src

OMEGA_CPP_SRC := $(wildcard $(OMEGA_SRC_DIR)/*.cpp)
OMEGA_CPP_OBJS := $(patsubst $(OMEGA_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.opp, $(OMEGA_CPP_SRC))

OMEGA_C_SRC := $(wildcard $(OMEGA_SRC_DIR)/*.c)
OMEGA_C_OBJS := $(patsubst $(OMEGA_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(OMEGA_C_SRC))

all : omega

omega : build_dir $(OMEGA_CPP_OBJS) $(OMEGA_C_OBJS) link_data
	$(LD) -o $@ $(OMEGA_CPP_OBJS) $(OMEGA_C_OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o : $(OMEGA_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.opp : $(OMEGA_SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

build_dir : 
	mkdir -p $(BUILD_DIR)

link_data :
	rm -f $(DATA_LINK_TARGET)
	ln -sf $(DATA_DIR) $(DATA_LINK_TARGET)

clean : 
	rm -rf $(BUILD_DIR)
	rm -f $(DATA_LINK_TARGET)
	rm -f omega*

rebuild : clean all

test : omega

