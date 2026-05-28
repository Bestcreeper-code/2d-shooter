CXX := g++

ELF := Game
OBJ_DIR := build

BOX2D_LIB := libs/box2D/build/src/libbox2d.a
RAYLIB_LIB := libs/raylib/src/libraylib.a

LIBS := $(BOX2D_LIB) $(RAYLIB_LIB) -lGL -lm -lpthread -ldl -lrt -lX11



CXX_SRCS := $(shell find src -name "*.cpp")
CXX_OBJS := $(CXX_SRCS:src/%.cpp=$(OBJ_DIR)/src/%.o)

INCLUDES := -Ilibs/box2D/include -Ilibs/raylib/src -Isrc

 

CXXFLAGS_DEBUG := -O0 -g -Wall -Werror
CXXFLAGS_RELEASE := -Os -s -fdata-sections -ffunction-sections -Wall -Werror -g



.PHONY: all debug release clean

all: debug

debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
debug: $(ELF)

release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: $(ELF)




$(BOX2D_LIB):
	@echo "Building Box2D"
	@(cd libs/box2D && ./build.sh)

$(RAYLIB_LIB):
	@echo "Building Raylib"
	@(cd libs/raylib/src && make -j4)



$(ELF): $(BOX2D_LIB) $(RAYLIB_LIB) $(CXX_OBJS)
	$(CXX) $(CXX_OBJS) -o $@ $(LIBS)




$(OBJ_DIR)/src/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@



clean:
	rm -rf $(OBJ_DIR) $(ELF)