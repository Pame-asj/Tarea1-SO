run: main.c
	gcc -o run main.c
clean:
	rm -f run *.o 
	rm -f RegistroPokemon.txt