#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h" 

// Hash Table - stocheaza elementele intr-un array intr-un mod criptat?

struct Employee // structul ce contine informatiile pentru employee
{
	short code;
	char* name;
	char* dept;
	double salary;
};
typedef struct Employee NodeInfo; 

typedef struct HT // Structul Hash Table
{
	NodeInfo** items; // Contine un vector de pointeri la employees
	int size; // dimensiunea arrayului
}HashTable;

#define LINE_BUFFEER 1024
#define HT_INITIAL_CAPACITY 3
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
void printInfo(NodeInfo*);
/*functions signatures for PQ operations*/
void putHT(HashTable*, NodeInfo*);
NodeInfo* get(HashTable, char*);

void main()
{
	HashTable hashTable = { .items = NULL, .size = 0 }; // initializam cu null si size 0 pt ca nu avem nimic momentan in el

	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citire 
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // variabile pentru stocarea a ce citim
	if (pFile) // daca exista fisierul
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // cat timp avem ce citi din fisier o facem
		{
			token = strtok(lineBuffer, sepList);
			code = atoi(token); // bagam codu
			name = strtok(NULL, sepList); // numele
			dept = strtok(NULL, sepList); // departamentul
			token = strtok(NULL, sepList); 
			salary = atof(token); // salariul

			NodeInfo* info = createInfo(code, name, dept, salary); // cream un nod employee cu informatiile citite
			putHT(&hashTable, info); // si il bagam in hash table

			printf("\n***************************\n");
			printf("\nSize: %d\n", hashTable.size);
			for (int i = 0; i < hashTable.size; i++) // afisarea hash table-ului existent pana la momentul acesta.
				if (hashTable.items[i] != NULL)
				{
					printf("Index: %d --", i);
					printInfo(hashTable.items[i]);
				}
			NodeInfo* emp = get(hashTable, "Cazacu Robert"); // preluarea informatiilor despre un employee din hash table.
			printInfo(emp); // si il afisham.
		}
	}
}
NodeInfo* get(HashTable hTable, char* key) // functia de preluare a informatiilor din hash table (search?) 
{
	NodeInfo* result = NULL; // luam un nod de tip employee
	if (hTable.items != NULL) // daca avem ceva in hash table-ul nostru
	{
		int index = fhash(key, hTable.size); // atunci calculam index-ul la care s-ar putea afla employee-ul cautat
		while (index < hTable.size && hTable.items[index] != NULL && strcmp(key, hTable.items[index]->name) != 0) // daca nu e la pozitia respectiva
			index++; // atunci va intra in while si va cauta incepand cu pozitia aia
		if (index < hTable.size && hTable.items[index] != NULL) // daca nu a ajuns la final si exista ceva la index-ul respectiv
			result = hTable.items[index]; // atunci ia valoarea de acolo
	}
	return result; // si o returneaza.
}

int fhash(char* key, int size) // calcularea indexului
{
	//printf(" \n%d : size ->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n",(key[0] % size));
	return key[0] % size; // in functie de codul ASCII de la prima litera si dimensiunea hash table-ului
}
void resizeHT(HashTable* hTable) // functia de dat resize la hash table
{
	NodeInfo** aux = hTable->items; // luam o valriabila auxiliara ce va lua valorile din curentul hash table
	hTable->items = (NodeInfo**)malloc(sizeof(NodeInfo*) * 2 * hTable->size); // vom aloca o noua memorie de dimensiune dubla
	memset(hTable->items, NULL, sizeof(NodeInfo*) * 2 * hTable->size); // vom initializa totul cu 0
	hTable->size *= 2; // dam increase si la dimensiune
	for (int i = 0; i < hTable->size / 2; i++) // si apoi reconstruim hash-table in functie de noua dimensiune
	{
		if (aux[i] != NULL)
			putHT(hTable, aux[i]);
	}
	free(aux); // si dealocam variabila auxiliara
}
int linearProbing(HashTable* hTable, int index, char* key) // cautarea unui nou index
{
	while (hTable->items[index] != NULL &&				// cat timp exista ceva pe pozitia respectiva
		strcmp(hTable->items[index]->name, key) !=0)  // si cat timp ce e pe pozitia respectiva e diferit fata de ce e acolo deja
	{
		index++; // crestem indexu
		if (index == hTable->size) // daca cumva ajungem la finalul hash table-ului
		{
			resizeHT(hTable); // ii marim dimensiunea
			index = fhash(key, hTable->size); // si din moment ce indexu il calculam in functie de dimensiune
												// recalculam si indexu
		}
	}
	return index; // returnam indexul gasit ca fiind liber.
}
void putHT(HashTable* hTable, NodeInfo* emp) // Bagarea in hash table
{
	//first initialization of the hashtable
	if (hTable->items == NULL) // in cazul in care hash table este null
	{
		hTable->items = (NodeInfo**)malloc(sizeof(NodeInfo*) * HT_INITIAL_CAPACITY); // atunci alocam memorie pentru el
		memset(hTable->items, NULL, sizeof(NodeInfo*) * HT_INITIAL_CAPACITY); // si setam tot ce e in el cu null
		hTable->size = HT_INITIAL_CAPACITY; // setam si dimensiunea.
	}
	int index = fhash(emp->name, hTable->size); // calculam indexul 
	if (hTable->items[index] != NULL) // verificam daca e ceva la pozitia respectiva.
	{
		//probing which returns a valid index
		index = linearProbing(hTable, index, emp->name); // daca exista, atunci cautam un alt index la care sa bagam
	}
	hTable->items[index] = emp; // bagam la indexul la care trebuie.
}

void printInfo(NodeInfo* info) // afisarea unui employee
{
	if (info) // daca exista atunci afiseaza asta
		printf("Code: %d - Name: %s\n", info->code, info->name);
	else
		printf("No data to print!"); // in cazul in care nu exista, afiseaza asta.
}

NodeInfo* createInfo(short code, char* name, char* dept, double salary) // crearea unui nod de tip employee
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // ii alocam memorie
	emp->code = code; // bagam codu
	emp->name = (char*)malloc(strlen(name) + 1); // alocam memorie pentru nume
	strcpy(emp->name, name); // bagam numele
	emp->dept = (char*)malloc(strlen(dept) + 1); // alocam memorie pentru departament
	strcpy(emp->dept, dept); // bagam departamentu
	emp->salary = salary; // bagam salariul
	return emp; // si returnam nodul.
}