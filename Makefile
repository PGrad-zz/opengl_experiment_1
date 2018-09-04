GLAD_INCLUDE=include/
KHR_INCLUDE=include/KHR
GLM_INCLUDE=include/glm
ASSIMP_INCLUDE=include/assimp
STB_INCLUDE=include/stb
CXXFLAGS=-std=c++14 -pedantic -ggdb -I$(GLAD_INCLUDE) -I$(KHR_INCLUDE) -I$(GLM_INCLUDE) -I$(ASSIMP_INCLUDE) -I$(STB_INCLUDE) -fPIC
CC=g++
LDFLAGS =-ldl -lGL `pkg-config --static --libs glfw3` `pkg-config --static --libs assimp`
OBJS=$(addprefix objs/, $(addsuffix .o, Shader Mesh Model FrameBuffer objects glad))
all: objs GL01
GL01: $(OBJS) src/main.cpp
	$(CC) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
objs/glad.o: src/glad.c
	$(CC) -c $(CXXFLAGS) $< -o $@
objs/%.o: src/%.cpp src/%.h
	$(CC) -c $(CXXFLAGS) $< -o $@
objs:
	mkdir objs
.PHONY: clean
clean:
	rm -rf GL01 objs
