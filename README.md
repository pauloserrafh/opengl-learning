# OpenGL learning

This is a learning repository for dealing with OPENGL and working with shaders,
specifically with [ShaderToy](www.shadertoy.com) compatible ones

Tested under Ubuntu 18.

## Dependencies
- glew
- glfw3

## Install
### Ubuntu

```bash
$ sudo apt install libglfw3-dev libglew-dev libsdl2-dev
```
## Build
### Ubuntu
```bash
$ g++ main.cpp -lGL -lglfw -lGLEW -lSDL2 -o demo
```

## Run
```bash
$ ./demo
```
