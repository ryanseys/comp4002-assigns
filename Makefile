all:
	make a4

a1t1:
	g++ -w assign1part1/assign1part1.cpp -o assign1part1/a1t1.out -framework OpenGL -framework GLUT && cd assign1part1 && ./a1t1.out

a1t2:
	g++ -w assign1part2/assign1part2.cpp -o assign1part2/a1t2.out -framework OpenGL -framework GLUT && cd assign1part2 && ./a1t2.out

a2:
	g++ -w assign2/assign2.cpp assign2/Shader.cpp assign2/ryan_camera.cpp  -o assign2/a2.out -framework OpenGL -framework GLUT && cd assign2 && ./a2.out

a3:
	g++ -w assign3/assign3.cpp assign3/Shader.cpp assign3/ryan_camera.cpp  -o assign3/a3.out -framework OpenGL -framework GLUT && cd assign3 && ./a3.out

a4:
	g++ -I/opt/local/include/SOIL -L/opt/local/lib -w assign4/main.cpp assign4/Shader.cpp assign4/camera.cpp assign4/call_back_functions.cpp assign4/SkyBox.cpp -o assign4/a4.out -lSOIL -framework OpenGL -framework GLUT -framework CoreFoundation && cd assign4 && ./a4.out
