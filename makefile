all:
	g++ main.cpp object.cpp utils.cpp objloader.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr
clean:
	@rm *.o
