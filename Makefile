GLAD_INCLUDE=include/
KHR_INCLUDE=include/KHR
GLM_INCLUDE=include/glm
ASSIMP_INCLUDE=include/assimp
STB_INCLUDE=include/stb
CXXFLAGS=-std=gnu++17 -pedantic -I$(GLAD_INCLUDE) -I$(KHR_INCLUDE) -I$(GLM_INCLUDE) -I$(ASSIMP_INCLUDE) -I$(STB_INCLUDE)
LDFLAGS =-ldl -lGL `pkg-config --static --libs glfw3` `pkg-config --static --libs assimp`
OBJS=$(addprefix objs/, $(addsuffix .o, Shader Mesh Model objects glad))
all: objs GL01
GL01: $(OBJS) src/main.cpp
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
objs/glad.o: src/glad.c
	g++ -c $(CXXFLAGS) $< -o $@
objs/%.o: src/%.cpp src/%.h
	g++ -c $(CXXFLAGS) $< -o $@
objs:
	mkdir objs
.PHONY: clean
clean:
	rm -rf GL01 objs
