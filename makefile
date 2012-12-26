all:
	g++ main.cpp object.cpp material.cpp viewer.cpp utils.cpp mtlloader.cpp objloader.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr

objloader:
	g++ object.cpp material.cpp mtlloader.cpp viewer.cpp utils.cpp objloader.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr -D OLTEST

mtlloader:
	g++ object.cpp material.cpp viewer.cpp utils.cpp mtlloader.cpp objloader.cpp -I /usr/cslib/include -L /usr/cslib -l glfw -l GLEW -l pthread -l Xrandr -D MTLTEST

clean:
	@rm *.o
