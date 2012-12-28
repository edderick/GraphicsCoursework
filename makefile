all:
	g++ main.cpp object.cpp skybox.cpp material.cpp viewer.cpp utils.cpp mtlloader.cpp objloader.cpp heightmaploader.cpp geometrygenerator.cpp -I /usr/cslib/include -I include -L /usr/cslib -L lib -l glfw -l GLEW -l pthread -l Xrandr -l glimg -l glload

objloader:
	g++ object.cpp material.cpp mtlloader.cpp viewer.cpp utils.cpp objloader.cpp -I /usr/cslib/include -I include -L /usr/cslib -L lib -l glfw -l GLEW -l pthread -l Xrandr -D OLTEST -l glimg -l glload

mtlloader:
	g++ object.cpp material.cpp viewer.cpp utils.cpp mtlloader.cpp objloader.cpp -I /usr/cslib/include -I include -L /usr/cslib -L lib -l glfw -l GLEW -l pthread -l Xrandr -D MTLTEST -l glimg -l glload

clean:
	@rm *.o
