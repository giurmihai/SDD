
///////// DOUBLY LINKED / DOUBLY LINKED CIRCULAR LIST => PEEK, GET, PUT, PARCURGERE, SCOATERE ENTRIES CU COD PAR, ETC.

#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"
struct Employee
{
	short code;
	char* name;
	char* dept;
	double salary;
};

typedef struct node
{
	struct Employee* info;
	struct node* pNext;
	struct node* pPrev;
}Node;

typedef struct Employee NodeInfo;
typedef struct node Queue;

#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
Node* createNode(NodeInfo*);
/*functions signatures for list operations*/

void put(Queue**, NodeInfo*);
NodeInfo* get(Queue**);
Node* peek(const Queue*);
void printInfo(NodeInfo*);
void printList2(const Node*);
void printList(const Node*);
void putNOCircular(Queue**, NodeInfo*);
Queue* parcurgerelistacirculara_eliminacoduriimpare(const Node*);

void main()
{
	Queue* queue = NULL; // declaram queue

	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citire
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // decalarari variabile pentru retinere citire
	if (pFile)
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) 
		{
			token = strtok(lineBuffer, sepList);
			code = atoi(token);
			name = strtok(NULL, sepList);
			dept = strtok(NULL, sepList);
			token = strtok(NULL, sepList);
			salary = atof(token); // citim tot

			NodeInfo* info = createInfo(code, name, dept, salary); // si bagam intr-un nou emp


			// FOLOSESTI PUT CA SA CREEZI DUBLY LINKED LIST SI PUTNOCIRCULCAR CA SA FACI DUBLY LINKED LIST
			put(&queue, info);   //  AFISEZI O SINGURA DATA ATAT DOUBLY LINKED LIST CAT SI CIRCULAR CU PRINTLIST SI AFISEZI LA INFINIT CU CEA CIRCULARA DACA APELEZI PRINTLIST2// functia asta primeste ca parametrii informatia si queue-ul. In interiorul ei este creat un nod cu informatia data ca parametru, iar apoi este adaugat in queue.
			//putNOCircular(&queue, info); // CREEZI CIRCULAR DUBLY LINKED LIST functia asta primeste ca parametrii informatia si queue-ul. In interiorul ei este creat un nod cu informatia data ca parametru, iar apoi este adaugat in queue.

		}
		Node* nod = NULL;

		printf("\n --------- Afisare Lista -------- \n");
		printList(queue);
		//printList2(queue);
		printf("\n -------- Parcurgere elimina coduri impare ------- \n");
		//queue = parcurgerelistacirculara_eliminacoduriimpare(queue);
		printf("\n -------  Afiseaza Lista --------- \n");
		printList(queue); // sau printList2 daca vrei sa vezi cum cicleaza la infinit
		printf("\n --------- Afiseaza primul nod din coada ------- \n");
		//NodeInfo* peek(const Queue*);
		nod = peek(queue); // returnez primul nod din lista
		printInfo(nod->info); // afisez info din primul nod din lista
		printf("\n ------ Afisez primu nod e linked totu? -------- \n");
		printList(nod); // sau printList2 daca vrei sa vezi cum cicleaza la infinit

		//NodeInfo* get(Queue * *queueHead)
		NodeInfo* val = NULL;
		val = get(&queue); // scot primul element din lista
		printf("\n ---------- Scot primu element si afisez: ---- \n");
		printInfo(val);
		printf("\n ---------- Afisez lista: ------ \n");
		printList(queue); // sau printList2 daca vrei sa vezi cum cicleaza la infinit
	}
}

void printInfo(NodeInfo* info)
{
	printf("Code: %d - Name: %s\n", info->code, info->name);
}

void printList(const Node* head)
{
	Node* initialnode = head;
	int cnt = 0;
	while (cnt == 0)
	{
		printInfo(head->info);
		head = head->pNext;
		if (head->pNext == NULL)
			cnt++;
		if (head == initialnode)// daca lista e circulara si reapare primul nod, ies din while cat sa nu se afiseze la infinit.
			cnt++;
	}
}

void printList2(const Node* head)
{
	while (head)
	{
		printInfo(head->info);
		head = head->pNext;
	}
}


Queue* parcurgerelistacirculara_eliminacoduriimpare(const Node* head) // parcurge lista circulara si elimina codurile impare
{
	Queue* queue = NULL;
	Node* initial = head;
	int cnt = 0;
	while (cnt == 0)
	{
		if (head->info->code % 2 == 0) {
			printInfo(head->info);
			//value = get(head);
			NodeInfo* value = createInfo(head->info->code, head->info->name, head->info->dept, head->info->salary);
			put(&queue, value);
		}
		head = head->pNext;
		if (initial == head)
			cnt++;
	}

	return queue;
}

NodeInfo* get(Queue** queueHead) // scoate primul element din lista (modificand lista)
{
	NodeInfo* value = NULL;
	if (*queueHead != NULL)
	{
		value = (*queueHead)->info;
		Node* tmp = *queueHead;
		if (tmp->pNext != tmp && tmp->pPrev != tmp)
		{
			tmp->pNext->pPrev = tmp->pPrev;
			tmp->pPrev->pNext = tmp->pNext;
			*queueHead = tmp->pNext;
		}
		else
			*queueHead = NULL;
		free(tmp);
	}
	return value;
}

void put(Queue** queueHead, NodeInfo* info) // put function in doubly linked list: ai jpeg in resurse.
{
	Node* node = createNode(info);
	if (*queueHead == NULL) // Daca lista este goala, se va face un nod care are: pNext = el insusi, pPrev = el insusi, apoi se va adauga la lista.
	{
		node->pNext = node->pPrev = node;
		*queueHead = node;
	}
	else // Daca lista are deja noduri, atunci valorile nodului nou creat vor fi urmatoarele: pNext = valoarea primului nod, pPrev = valoarea ultimului nod din lista, iar pNext al ultimului nod din lista, va primi valoarea nodului curent (ce tocmai a fost creat).
	{
		node->pNext = *queueHead; // 1
		node->pPrev = (*queueHead)->pPrev; // 2

		(*queueHead)->pPrev->pNext = node; // 3
		(*queueHead)->pPrev = node; // 4
	}
}


void putNOCircular(Queue** queueHead, NodeInfo* info) // put function in doubly linked list: ai jpeg in resurse.
{
	Node* node = createNode(info);
	if (*queueHead == NULL) // Daca lista este goala, se va face un nod care are: pNext = el insusi, pPrev = el insusi, apoi se va adauga la lista.
	{
		node->pNext = NULL;
		node->pPrev = node;
		*queueHead = node;
	}
	else // Daca lista are deja noduri, atunci valorile nodului nou creat vor fi urmatoarele: pNext = valoarea primului nod, pPrev = valoarea ultimului nod din lista, iar pNext al ultimului nod din lista, va primi valoarea nodului curent (ce tocmai a fost creat).
	{
		node->pNext = NULL; // 1
		node->pPrev = (*queueHead)->pPrev; // 2

		(*queueHead)->pPrev->pNext = node; // 3
		(*queueHead)->pPrev = node; // 4
	}
}
Node* peek(Queue* topStack) // returneaza primul element din stack fara sa modifice stack ul
{
	return topStack;
}

NodeInfo* createInfo(short code, char* name, char* dept, double salary)
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo));
	emp->code = code;
	emp->name = (char*)malloc(strlen(name) + 1);
	strcpy(emp->name, name);
	emp->dept = (char*)malloc(strlen(dept) + 1);
	strcpy(emp->dept, dept);
	emp->salary = salary;
	return emp;
}
Node* createNode(NodeInfo* info)
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->info = info;
	node->pNext = NULL;
	node->pPrev = NULL;
	return node;
}

