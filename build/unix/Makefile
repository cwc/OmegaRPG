BUILD_DIR := obj

CPP := g++
LD := g++

CFLAGS := -std=c++11
LDFLAGS := -lcurses

DATA_DIR := ../../data

OMEGA_SRC_DIR := ../../Omega/src

OMEGA_CPP_SRC := $(wildcard $(OMEGA_SRC_DIR)/*.cpp)
OMEGA_CPP_OBJS := $(patsubst $(OMEGA_SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(OMEGA_CPP_SRC))

all : omega

omega : build_dir $(OMEGA_CPP_OBJS) copy_data
	$(LD) -o $@ $(OMEGA_CPP_OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o : $(OMEGA_SRC_DIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

build_dir : 
	mkdir -p $(BUILD_DIR)

copy_data :
	cp -r $(DATA_DIR) ./

clean : 
	rm -rf $(BUILD_DIR)
	rm -rf data
	rm -f omega*

rebuild : clean all

test : omega

