//Giurgi creation
#define _CRT_SECURE_NO_WARNINGS
#include "memory.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// Vector in 3 dimensiuni.

//		Se da o lista de nume intr-un fisier text. Sa se citeasca ce e in fisier
// Apoi sa se grupeze numele alfabetic intr-o matrice.
// 
//		Practic e o matrice 
// de 26 de linii (litere in alfabet) 
// cu o dimensiune variabila de coloane (pe coloane se baga numele oamenilor)
// iar in fiecare celula mai stocam un char* care e numele respectivului.
// rezultatul final va fi char** catalogue[26] (26 de linii, numar variabil de coloane si numar variabil de litere in fiecare nume)
// 

void main()
{
	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul
	char buffer[128]; // un cuvant pe care il citim
	int index = 0; // variabila ce retine cate cuvinte avem in total (nume)
	char* names[100]; // in variabila asta vom citi ce e in fisier
	char** catalogue[26]; // variabila finala in care vom stoca numele ordonate 
	char freq[26]; // in variabila asta vom retine cate cuvinte cu fiecare litera sunt stocate (2 nume cu A, 13 nume cu B, z nume cu C, etc)
	memset(names, NULL, sizeof(char*) * 100); // Putem citi maxim 100 de cuvinte din fisier.
	memset(freq, 0, sizeof(char) * 26); // Alocam memorie si umplem cu 0 peste tot (ca sa putem numara, plecam de la 0)
	memset(catalogue, NULL, sizeof(char**) * 26); // Initializam un numar de 26 de linii
	if (pFile) { // Daca exista fisierul 
		fscanf(pFile, "%s", buffer); // Atunci citim prima linie
		while (!feof(pFile)) // Cat timp nu suntem la finalul documentului
		{
			names[index] = (char*)malloc(strlen(buffer) + 1); // Alocam memorie pentru a stoca numele

								//(strlen(buffer)+1) * sizeof(char)
								//malloc returns void *

			strcpy(names[index], buffer); // Copiem informatia in names

								//source is const char* because it needs to ramain unchanged
								
			short position = *names[index++] - 'A'; // Calculam pozitia la care vom introduce numele (In functie de codul ASCII al primei litere)
							//names[index][0] <=> *names[index]

			freq[position]++; // Si incrementam numarul de cuvinte(nume) de la pozitia aia. 
								//(adica noi si numaram cate elemente cu A sunt, cate cu B, etc)


			fscanf(pFile, "%s", buffer); // Citim urmatorul cuvant (nume)
		}
		char aux[26];
		/*for (int i = 0; i < 26; i++)
			aux[i] = freq[i];*/
		memcpy(aux, freq, 26 * sizeof(char)); // Luam o variabila auxiliara si copiem tot ce e in freq, in ea. 
												// Putem face asta cu memcpy in loc sa parcurgem pe un for.


		for (int i = 0; i < index; i++) // Parcurgem toate cuvintele citite
		{
			int pos = *names[i] - 'A'; // Calculam pozitia la care vom pune numele(cuvantul)
			if (catalogue[pos] == NULL) // Daca nu am alocat memorie pentru cuvintele care incep cu litera respectiva
				catalogue[pos] = (char**)malloc(sizeof(char*) * freq[pos]); // Atunci o facem acum.
			catalogue[pos][--aux[pos]] = (char*)malloc(strlen(names[i]) + 1); // Alocam spatiu pentru un nou element, in timp ce scadem cate cuvinte mai raman de pus. 
			strcpy(catalogue[pos][aux[pos]], names[i]);// noi scadem de fapt ca sa avem elementele de la 0
		}

		for (int i = 0; i < 26; i++) // Afisam rezultatul final.
		{
			if (catalogue[i]) { // Daca avem cuvinte cu litera respectiva
				printf("Group %c\n", i + 'A'); 
				for (int j = 1; j <= freq[i]; j++) // Afisam toate cuvintele de la litera aia.
					printf("Name: %s\n", catalogue[i][j]);
			}
		}

	}
}