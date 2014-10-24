all:
	make a2t2

a1t1:
	g++ -w assign1part1/assign1part1.cpp -o assign1part1/a1t1.out -framework OpenGL -framework GLUT && cd assign1part1 && ./a1t1.out

a1t2:
	g++ -w assign1part2/assign1part2.cpp -o assign1part2/a1t2.out -framework OpenGL -framework GLUT && cd assign1part2 && ./a1t2.out

a2t1:
	g++ -w assign2/assign2task1.cpp -o assign2/a2t1.out -framework OpenGL -framework GLUT && cd assign2 && ./a2t1.out

a2t2:
	g++ -w assign2/assign2task2.cpp -o assign2/a2t2.out -framework OpenGL -framework GLUT && cd assign2 && ./a2t2.out

