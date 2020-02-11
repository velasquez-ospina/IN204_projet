
output: main.o
	mpirun -np 8 a.out

main.o: main.cpp
	mpic++ main.cpp json11.cpp

clean:
	rm *.o a.out Scene_no_aliasing.bmp
