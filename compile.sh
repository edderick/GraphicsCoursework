#! /bin/bash

g++ main.cpp  sphere.cpp cone.cpp utils.cpp -I /usr/cslib/include -L /usr/cslib/lib -l glfw -l GLEW -l pthread -l Xrandr
