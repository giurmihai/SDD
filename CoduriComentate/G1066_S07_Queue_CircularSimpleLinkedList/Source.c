#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"
#include "stdbool.h"

// 
// Queue - implementare cu lista simplu inlantuita (circulara?)
// Are si functie de delete.
// 
// Aceeasi cerinta: sa se bage ce e in Data.txt sa se bage in queue.
//

struct Employee // Declararea Employee
{
	short code;
	char* name;
	char* dept;
	double salary;
};

typedef struct Node // Declararea listei
{
	struct Employee* info; // Informatiile sunt employee
	struct Node* pNext; // Pointer la urmatorul element.
}Queue;

typedef struct Employee NodeInfo;

#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
Queue* createNode(NodeInfo*);
/*functions signatures for list operations*/
void printInfo(NodeInfo*);
Queue* put(Queue*, NodeInfo*);
NodeInfo* get(Queue**);
void deleteQueue(Queue**);
bool isEmpty(Queue*);
void printQueue(Queue**);

void main()
{
	Queue* queueTail = NULL; // Se retine coada queue.

	FILE* pFile = fopen("Data.txt", "r"); // Deschidem fisierul.
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; //  declarari pentru citire.
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // declarari variabile.
	if (pFile)
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // cat timp avem ceva in fisier si citim in linebuffer
		{
			token = strtok(lineBuffer, sepList); // cu strtok luam de la virgula la virgula
			code = atoi(token); // Bagam codul.
			name = strtok(NULL, sepList); // numele	
			dept = strtok(NULL, sepList); // departamentul
			token = strtok(NULL, sepList); 
			salary = atof(token); // salariul

			NodeInfo* info = createInfo(code, name, dept, salary); // Cream nodul.
			queueTail = put(queueTail, info); // punem la finalul listei.
		}
		printQueue(&queueTail); // Afisarea queue.
		printf("\n--------------------------------\n");
		deleteQueue(&queueTail);

	}
}
void printQueue(Queue** tail) // Functia de print.
{
	Queue* tmp = NULL; // luam un tmp pentru parcurgere
	while (!isEmpty(*tail)) // cat timp nu avem informatii in coada
	{
		NodeInfo* info = get(tail); // Cream un nod cu informatiile din coada
		printInfo(info); // Le afisam
		tmp = put(tmp, info); // Si le punem inapoi 
		//	Pentru ca la queue nu poti parcurge lista si sa faci ce vrei. 
			// Mereu ai acces doar la ce urmeaza.
	}
	*tail = tmp; // reinitializam coada.
}

bool isEmpty(Queue* queue) {// functia is empty verifica daca avem ceva la adresa aia de memorie.
	return (queue == NULL);
}
void deleteQueue(Queue** tail) // Functia de stergere a unui nod din lista.
{
	while (!isEmpty(*tail)) // daca exista ceva la adresa aia de memorie.
	{
		//NodeInfo* emp = get(&*tail); //queueTail == *tail
		NodeInfo* emp = get(tail); // Atunci luam in emp ce e in coada.
		printInfo(emp); // afisam ce contine.
		free(emp->name); // apoi dealocam memoria, incepand de la ce contine emp.
		free(emp->dept);
		free(emp);		 // apoi dealocam si emp.
	}
}

NodeInfo* get(Queue** tail) // Functia de get la elementul ce urmeaza in coada.
{
	NodeInfo* value = NULL; // Declaram un employee ca sa stocam valoarea
	if (!isEmpty(*tail)) // Daca nu e goala valoarea pe care o vrem
	{
		value = (*tail)->pNext->info; // atunci o luam
		Queue* tmp = (*tail)->pNext; // luam un temp ce contine urmatorul element
		if (tmp->pNext == tmp) // daca urmatorul element e acelasi cu cel curent
			(*tail) = NULL; // atunci lista va fi golita
		else
			(*tail)->pNext = tmp->pNext; // daca nu, il scot din coada
		free(tmp); // si dupa eliberez memoria.
	}
	return value; // returnez valoarea.
}

Queue* put(Queue* tail, NodeInfo* info) // Functia de punere la final.
{
	Queue* node = createNode(info); // Cream un nou nod.
	if (isEmpty(tail)) // Daca e goala lista
		node->pNext = node; // atunci facem tail-ul si cap.
	else // Daca avem o coada
	{
		node->pNext = tail->pNext; // atunci facem nodul nostru sa pointuiasca la head-ul listei
		tail->pNext = node; // si il introducem in lista
	}
	return node; // Returnam tail-ul
}

void printInfo(NodeInfo* info) // Afisarea info din employee
{
	printf("Code: %d - Name: %s\n", info->code, info->name);
}

NodeInfo* createInfo(short code, char* name, char* dept, double salary) // Functia de creare a unui nou employee
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam memorie pentru employee
	emp->code = code; // punem codu	
	emp->name = (char*)malloc(strlen(name) + 1); // alocam memorie pentru nume
	strcpy(emp->name, name); // Punem numele
	emp->dept = (char*)malloc(strlen(dept) + 1); // alocam memorie pentru departament
	strcpy(emp->dept, dept);// Punem departamentu
	emp->salary = salary; // Punem salariul
	return emp; // returnam employee-ul.
}
Queue* createNode(NodeInfo* info) // Crearea unui nou nod.
{
	Queue* node = (Queue*)malloc(sizeof(Queue)); // Alocam memorie pentru noul nod.
	node->info = info; // Bagam informatia.
	node->pNext = NULL; // Nu are un element urmator, pt ca doar il cream momentan.
	return node; // Returnam nodul.
}