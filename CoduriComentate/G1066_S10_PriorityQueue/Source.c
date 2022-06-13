#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"

// PriorityQueue - combinatie arbori + vector

struct Employee // Ce contine un employee
{
	short code;
	char* name;
	char* dept;
	double salary;
};
typedef struct Employee NodeInfo;

typedef struct PQ // Un priority queue va avea:
{
	NodeInfo** items; // Un vector de pointeri la emmplyees
	int size; // dimensiunea vectorului
	int nextIndex; //Urmatoarea pozitie la care vom insera
}PriorityQueue;

#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
/*functions signatures for PQ operations*/
void putPQ(PriorityQueue*, NodeInfo*);
NodeInfo* getPQ(PriorityQueue*);
NodeInfo* peek(const PriorityQueue*);
void ReHeapUp(PriorityQueue*, int);
void printInfo(NodeInfo*);

void main()
{
	PriorityQueue pQueue = { .items = NULL, .size = 0, .nextIndex = 0 }; // initializam priority queue. 
																// size va fi 0 ca nu avem nimic si nextIndex va fi 0 ca bagam la poz 0

	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul.
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citire.
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // declarari pentru primirea a ce citim.
	if (pFile)
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // Cat timp citim si avem ce
		{
			// atoi = array to int
			token = strtok(lineBuffer, sepList); // luam chestiile separate prin virgula
			code = atoi(token); // bagam codu
			name = strtok(NULL, sepList); // numele
			dept = strtok(NULL, sepList); // departamentu
			token = strtok(NULL, sepList); 
			salary = atof(token); // Si salariul.

			NodeInfo* info = createInfo(code, name, dept, salary); // cream un nou employee cu informatiile citite

			putPQ(&pQueue, info); // bagam informatia din emp in queue.

			printf("\n------------------------------\n");
			for (int i = 0; i < pQueue.nextIndex; i++)
				printInfo(pQueue.items[i]);

		}
	}
}

void putPQ(PriorityQueue* pQueue, NodeInfo* emp) // introducerea informatiei in PQ.
{
	//initial allocation of memory
	if (pQueue->items == NULL) // Daca nu avem nimic in lista
	{
		pQueue->items = (NodeInfo**)malloc(sizeof(NodeInfo*) * 7); // atunci alocam spatiu cat pentru 7 elemente
		pQueue->size = 7; // si dam dimensiunea de 7
		memset(pQueue->items, NULL, sizeof(NodeInfo*) * 7); // facem totul null, ca sa nu avem balarii
	}
	//resizing PQ structure if it's full
	if (pQueue->nextIndex == pQueue->size) // queue overflow //Daca cumva unde trebuie sa bagam este egal cu dimensiunea atunci nu avem loc
		//Deci trebuie sa redimensionam.
	{
		NodeInfo** tmp; // luam un vector de employee temporar
		tmp = (NodeInfo**)malloc(sizeof(NodeInfo*) * pQueue->size*2); // de dimensiunea dubla fata de ce aveam deja
		memset(tmp, NULL, sizeof(NodeInfo*) * pQueue->size * 2); // ii setam memoria ca sa nu avem chestii random
		memcpy(tmp, pQueue->items, pQueue->size * sizeof(NodeInfo*)); // copiem tot ce aveam inainte si ne ramane un vector
																		//cu o dimensiune mai mare
		free(pQueue->items); // eliberam ce aveam inainte in items
		pQueue->items = tmp; // Si ii dam noua valoare
		pQueue->size *= 2; // redimensionam si size.
	}
	pQueue->items[pQueue->nextIndex] = emp; // bagam la next index ce avem nevoie sa bagam
	ReHeapUp(pQueue, pQueue->nextIndex); // Si dam reheapup (noi avem nevoie mereu pe pozitia 0 
															//sa fie elementul cu cea mai mare importanta)
	pQueue->nextIndex++; // Incrementam pozitia la care vom introduce urmatorul element.
}
void ReHeapUp(PriorityQueue* pQueue, int childIndex) // Functia de resortare a vectorului.
{
	if (childIndex > 0) // Daca am bagat un copil in arbore 
	{
		int parentIndex = (childIndex - 1) / 2; // calculam indexul parintelui noului element adaugat in arbore.
		if (pQueue->items[childIndex]->code > pQueue->items[parentIndex]->code) // sortam in functie de cod.
		{
			NodeInfo* aux = pQueue->items[childIndex]; // interschimbarea clasica intre 2 elemente
			pQueue->items[childIndex] = pQueue->items[parentIndex];
			pQueue->items[parentIndex] = aux;
			ReHeapUp(pQueue, parentIndex); // apelam din nou (sa verificam daca e bine la noua pozitie la care l-am pus.)
												
		}
	}
}

void printInfo(NodeInfo* info) // afisarea unui employee
{
	printf("Code: %d - Name: %s\n", info->code, info->name);
}

NodeInfo* createInfo(short code, char* name, char* dept, double salary) // crearea unui nod de tip employee
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam memorie pentru nod
	emp->code = code; // bagam codu
	emp->name = (char*)malloc(strlen(name) + 1); // alocam memorie pentru nume
	strcpy(emp->name, name); // bagam numele
	emp->dept = (char*)malloc(strlen(dept) + 1); // alocam memorie pentru departament
	strcpy(emp->dept, dept); // bagam departamentu
	emp->salary = salary; // bagam salariul
	return emp; // returnam nodul.
}