#!/bin/bash

g++ -c controls.cpp
g++ -c shader.cpp
g++ -c phys.cpp
g++ -c collision.cpp
g++ -o phys phys.o controls.o shader.o collision.o -lGLEW -lglfw -lGL
