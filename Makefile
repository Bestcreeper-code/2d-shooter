CXX := g++

ELF := Game
OBJ_DIR := build

BOX2D_LIB := libs/box2D/build/src/libbox2d.a

RAYLIB_LIB := build/libs/raylib/raylib/libraylib.a

JSON_LIB := build/libs/cJSON/libcjson.a

LIBS := $(BOX2D_LIB) $(RAYLIB_LIB) $(JSON_LIB) -lGL -lm -lpthread -ldl -lrt -lX11



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
			-Ilibs/cJSON \
			-Isrc -IOS_Dep

 

CXXFLAGS_DEBUG := -O0 -g -Wall -Werror -g -fno-omit-frame-pointer
CXXFLAGS_RELEASE := -O2 -DNDEBUG \
	-flto \
	-fdata-sections -ffunction-sections \
	-fomit-frame-pointer \
	-fno-exceptions -fno-rtti \
	-march=native



LDFLAGS_DEBUG := 
LDFLAGS_RELEASE := -flto \
	-Wl,--gc-sections \
	-Wl,--strip-all




.PHONY: all debug release clean

all: debug

debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
debug: LDFLAGS := $(LDFLAGS_DEBUG)
debug: $(ELF)

sanitize: CXXFLAGS := $(CXXFLAGS_DEBUG) -fsanitize=address -fsanitize=undefined -fPIC -g
sanitize: LDFLAGS := $(LDFLAGS_DEBUG) -fsanitize=address -fsanitize=undefined
sanitize: $(ELF)


release: CXXFLAGS := $(CXXFLAGS_RELEASE) 
release: LDFLAGS := $(LDFLAGS_RELEASE) 
release: $(ELF)




$(BOX2D_LIB):
	@echo "Building Box2D"
	@(cd libs/box2D && ./build.sh)

$(RAYLIB_LIB):
	@echo "Building Raylib"
	@mkdir -p build/libs
	@mkdir -p build/libs/raylib
	
	@(cd build/libs/raylib && cmake ../../../libs/raylib/)
	@(cd build/libs/raylib && make -j4)

$(JSON_LIB):
	@echo "Building JSON"
	@mkdir -p build/libs
	@mkdir -p build/libs/cJSON
	@(cd libs/cJSON && make && mv libcjson.a ../../build/libs/cJSON/ )



$(ELF): $(BOX2D_LIB) $(RAYLIB_LIB) $(JSON_LIB) $(CXX_OBJS)
	$(CXX) $(LDFLAGS) $(CXX_OBJS) -o $@ $(LIBS) 




$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@



clean:
	rm -rf $(OBJ_DIR)/src/ $(ELF)


clean-all: clean
	rm -rf libs/box2D/build/*
	rm -rf $(OBJ_DIR)/* $(ELF)
	$(MAKE) -C build/raylib clean