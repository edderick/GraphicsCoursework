all:
	g++ main.cpp sphere.cpp cone.cpp object.cpp utils.cpp objloader.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr

objtest:
	g++ objloader.cpp utils.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr

clean:
	@rm *.o
