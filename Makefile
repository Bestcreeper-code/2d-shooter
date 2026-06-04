CXX := g++

ELF := Game
OBJ_DIR := build

BOX2D_LIB := libs/box2D/build/src/libbox2d.a
RAYLIB_LIB := libs/raylib/src/libraylib.a

LIBS := $(BOX2D_LIB) $(RAYLIB_LIB) -lGL -lm -lpthread -ldl -lrt -lX11



CXX_SRCS := $(shell find src -name "*.cpp") \
	libs/imgui/imgui.cpp \
	libs/imgui/imgui_draw.cpp \
	libs/imgui/imgui_tables.cpp \
	libs/imgui/imgui_widgets.cpp \
	libs/rlImGui/rlImGui.cpp \
	OS_Dep/Linux.cpp
CXX_OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(CXX_SRCS))

INCLUDES := -Ilibs/box2D/include \
			-Ilibs/raylib/src \
			-Ilibs/imgui \
			-Ilibs/rlImGui \
			-Isrc -IOS_Dep

 

CXXFLAGS_DEBUG := -O0 -g -Wall -Werror -fsanitize=address -g -fno-omit-frame-pointer
CXXFLAGS_RELEASE := -Os -s -fdata-sections -ffunction-sections -Wall -Werror -g

LDFLAGS_DEBUG := -fsanitize=address
LDFLAGS_RELEASE := 




.PHONY: all debug release clean

all: debug

debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: $(ELF)

release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: LDFLAGS := $(LDFLAGS_RELEASE)
release: $(ELF)




$(BOX2D_LIB):
	@echo "Building Box2D"
	@(cd libs/box2D && ./build.sh)

$(RAYLIB_LIB):
	@echo "Building Raylib"
	@(cd libs/raylib/src && make -j4)



$(ELF): $(BOX2D_LIB) $(RAYLIB_LIB) $(CXX_OBJS)
	$(CXX) $(LDFLAGS) $(CXX_OBJS) -o $@ $(LIBS) 




$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@



clean:
	rm -rf $(OBJ_DIR) $(ELF)