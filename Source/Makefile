OBJ =  main.o
INC = -I "./"

PROJET: $(OBJ)
    g++ $(OBJ) -o PROJET.exe
    rm -f $(OBJ)

main.o:
    g++ -c main.cpp $(INC)

clean:
    rm -f $(OBJ) PROJET
