#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void crearCarpeta(char *dir) {
  
    struct stat st = {0};
  
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0700);
    }
}

int get_GEN(int NumPKMN) {
    if (NumPKMN >= 1 && NumPKMN <= 151){
        return 1;
    } 
    else if (NumPKMN >= 152 && NumPKMN <= 251){
        return 2;  
    } 
    else if (NumPKMN >= 252 && NumPKMN <= 386){
        return 3;  
    } 
    else if (NumPKMN >= 387 && NumPKMN <= 493){
        return 4; 
    } 
    else {
        return -1;
    }
}

void copiaF(char *source, char *destination) {
    FILE *src, *dst;
    int ch;

    src = fopen(source, "r");
    dst = fopen(destination, "w");

    if (src == NULL || dst == NULL) {
        printf("Error");
        return;
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dst);
    }

    fclose(src);
    fclose(dst);
}

//////////////////////////////7


int main() {
  
    struct dirent *de;
    DIR *dr = opendir("./pokemons");

    if (dr == NULL) {
        printf("No se pudo abrir el directorio");
        return 0;
    }

    //char letra;

    crearCarpeta("./Alfabetico");
    for(char letra = 'A'; letra <= 'Z'; letra++) {
        char dir[50];
        sprintf(dir, "./Alfabetico/%c", letra);
        crearCarpeta(dir);
    }
  
    crearCarpeta("./Generacion");
    for (int i = 1; i <= 4; i++) {
        char dir[50];
        sprintf(dir, "./Generacion/%d", i);
      crearCarpeta(dir);
    }

    int cant_gen1 = 0;
    int cant_gen2 = 0;
    int cant_gen3 = 0;
    int cant_gen4 = 0;

    int cant_letras[25];
    int suma = 0;

    while ((de = readdir(dr)) != NULL) {
        char *filename = de->d_name;
        char *baj = strchr(filename, '_');

        if (baj != NULL) {
            char nombre = filename[0];
            for(char letra = 'a'; letra <= 'z'; letra++) {
                if (nombre == letra) {
                    suma++;
                    cant_letras[letra - 'a'] += suma;

                    char old_path[100];
                    sprintf(old_path, "./pokemons/%s", filename);

                    char new_path[100];
                    sprintf(new_path, "./Alfabetico/%c/%s", nombre, filename);

                    copiaF(old_path, new_path);
                }else{
                    suma = 0;
                }
            }

            int NumPKMN = atoi(baj + 1);
            int GEN = get_GEN(NumPKMN);

            if (GEN != -1) {
                char old_path[100];
                sprintf(old_path, "./pokemons/%s", filename);

                char new_path[100];
                sprintf(new_path, "./Generacion/%d/%s", GEN, filename);

                copiaF(old_path, new_path);
            }
            
            if (GEN == 1) {
                cant_gen1++;
            } else if (GEN == 2) {
                cant_gen2++;
            } else if (GEN == 3) {
                cant_gen3++;
            } else{
                cant_gen4++;
            }
        } 
    }

    FILE *new_file = fopen("RegistroPokemon.txt", "w");
    fprintf(new_file, "Genereación\n\n");
    for (int i = 1; i <= 4; i++){
        if (i == 1) {
            fprintf(new_file, "I - %d\n", cant_gen1);
        } else if (i == 2) {
            fprintf(new_file, "II - %d\n", cant_gen2);
        } else if (i == 3) {
            fprintf(new_file, "III - %d\n", cant_gen3);
        } else if (i == 4) {
            fprintf(new_file, "IV - %d\n", cant_gen4);
        }
    }
    
    fprintf(new_file, "\nAlfabético\n\n");
    for(char letra = 'A'; letra <= 'Z'; letra++) {
        fprintf(new_file, "%c - %d\n", letra, cant_letras[letra - 'A']);
    }

    fclose(new_file);
    closedir(dr);
    return 0;
}
