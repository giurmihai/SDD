#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"
#include "stdbool.h"


struct Evaluation // Declararea Employee
{
	int evaluationId;
	char* name;
	int noCredits;
	double grade;
	int studentId;
};

typedef struct Node // Declararea listei
{
	struct Evaluation* info; // Informatiile sunt employee
	struct Node* pNext; // Pointer la urmatorul element.
}Queue;



typedef struct Evaluation NodeInfo;

typedef struct BST // Structul binary search tree
{
	struct BST* left; // copilul stanga
	NodeInfo* info; // Informatia nodului
	struct BST* right; // copilul dreapta
}BinarySearchTree;

#define LINE_BUFFEER 1024
/*	functions signatures for memory management*/
NodeInfo* createInfo(int, int , char* , int , double );
Queue* createNode(NodeInfo*);

/*	functions signatures for list operations	*/
void printInfo(NodeInfo*);
Queue* put(Queue*, NodeInfo*);
NodeInfo* get(Queue**);
void deleteQueue(Queue**);
bool isEmpty(Queue*);
void printQueue(Queue**);
void showNoLucrari(Queue**);
Queue* deleteQueueStud(Queue**, int);

//	function signatures for BST operations
void inorder_LPR(BinarySearchTree*);
void createBSTFromList(Queue**, BinarySearchTree**);
void insertBST(BinarySearchTree**, NodeInfo*);
BinarySearchTree* createNodeBST(NodeInfo*);


void main()
{
	Queue* queueTail = NULL; // Se retine coada queue.

	FILE* pFile = fopen("Data.txt", "r"); // Deschidem fisierul.
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ","; //  declarari pentru citire.
	char* name = NULL; int evaluationId = 0, noCredits=0, studentId=0; double grade = 0.0; // declarari variabile.
	int noElements = 0;
	if (pFile)
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // cat timp avem ceva in fisier si citim in linebuffer
		{
			//printf("%s", lineBuffer);
			token = strtok(lineBuffer, sepList); // cu strtok luam de la virgula la virgula
			evaluationId = atoi(token); // Bagam codul.
			token = strtok(NULL, sepList);
			noCredits = atoi(token);
			name = strtok(NULL, sepList); // numele	
			token = strtok(NULL, sepList);
			studentId = atoi(token); // salariul
			token = strtok(NULL, sepList);
			grade = atof(token); // salariul
			noElements++;
			NodeInfo* info = createInfo(evaluationId, noCredits, name, studentId, grade); // Cream nodul.
			//printInfo(info);
			queueTail = put(queueTail, info); // punem la finalul listei.
		}

		printQueue(&queueTail); // Afisarea queue.
		printf("\n--------------------------------\n");

		
		//showNoLucrari(&queueTail,noElements);
		queueTail = deleteQueueStud(&queueTail, 268);
		printQueue(&queueTail);

		printf("\n--------------- BST -----------------\n");
		BinarySearchTree* bTree = NULL;
		createBSTFromList(&queueTail, &bTree);
		inorder_LPR(bTree);

		printf("\n--------------- DeleteQueue -----------------\n");
		deleteQueue(&queueTail); // For some reason share-uiesc aceleasi adrese de memorie si nu am idee dc...

	}
}

#pragma region BST



void insertBST(BinarySearchTree** root, NodeInfo* emp) // Inserarea in binary search tree
{
	if (*root == NULL) // Daca arborele e null
	{
		*root = createNodeBST(emp); // atunci cream radacina cu informatiile din emp.
	}
	else // daca exista
	{
		if ((*root)->info->evaluationId > emp->evaluationId) // atunci mergem la stanga
			insertBST(&(*root)->left, emp);								//( in stanga vor fi nodurile cu val mai mici, la dreapta mai mari)
		else if ((*root)->info->evaluationId < emp->evaluationId) // sau la dreapta 
			insertBST(&(*root)->right, emp);
		else // iar daca e egal
			(*root)->info = emp; // dam override la informatie
	}
}
void inorder_LPR(BinarySearchTree* root) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		inorder_LPR(root->left); // mergem la stanga
		printInfo(root->info); // afisham
		inorder_LPR(root->right); // mergem la dreapta
	}
}
void createBSTFromList(Queue** tail, BinarySearchTree** root) // Functia de print.
{
	Queue* tmp = NULL; // luam un tmp pentru parcurgere
	while (!isEmpty(*tail)) // cat timp nu avem informatii in coada
	{
		NodeInfo* info = get(tail); // Cream un nod cu informatiile din coada
		char* name =NULL; int evaluationId = info->evaluationId, noCredits = info->noCredits, studentId = info->studentId; double grade = info->grade; // declarari variabile.
		name=(char*)malloc(strlen(info->name) + 1);
		strcpy(name, info->name);
		NodeInfo* newInfo = createInfo(evaluationId, noCredits, name, studentId, grade);
		insertBST(&(*root), info);
		tmp = put(tmp, info); // Si le punem inapoi 
		//	Pentru ca la queue nu poti parcurge lista si sa faci ce vrei. 
			// Mereu ai acces doar la ce urmeaza.
	}
	*tail = tmp; // reinitializam coada.
}
BinarySearchTree* createNodeBST(NodeInfo* emp) // Crearea unui nou nod pentru arbore
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree)); // alocam spatiu pentru el
	node->info = emp; // ii dam informatia
	node->left = node->right = NULL; // si stabilim ca nu are copii
	return node; // apoi il returnam
}

#pragma endregion

#pragma region SimpleLinkedList



void showNoLucrari(Queue** tail, int noElements)
{
	Queue* tmp = NULL;
	int* counter = (int)malloc(sizeof(int) * noElements);
	memset(counter, 0, sizeof(int) * noElements);
	char** examene = (char*)malloc(sizeof(char*) * noElements);
	(*tail) = (*tail)->pNext;
	char* rezultat;
	for (int i = 0; i < noElements; i++)
	{
		char* mama = (*tail)->info->name;
		int flag = 1;
		for (int j = 0; j < i; j++)
		{
			if (examene[j] != NULL)
			{
				if (strcmp(mama, examene[j]) == 0)
				{
					flag = 0;
					counter[j]++;
				}
			}
		}
		if (flag == 1)
		{
			examene[i] = mama;
			counter[i]++;
		}
		else
		{
			examene[i] = NULL;
		}
		(*tail) = (*tail)->pNext;
	}
	for (int i = 0; i < noElements; i++)
	{
		if (examene[i] != NULL)
		{
			printf("La Materia \"%s\" s-au dat un numar de %d examene\n", examene[i], counter[i]);
		}

	}


}
Queue* deleteQueueStud(Queue** tail, int studentId) // Functia de print.
{
	Queue* tmp = NULL; // luam un tmp pentru parcurgere
	while (!isEmpty(*tail)) // cat timp nu avem informatii in coada
	{
		NodeInfo* info = get(tail); // Cream un nod cu informatiile din coada
		if (info->studentId != studentId)
			tmp = put(tmp, info); // Si le punem inapoi 
			//	Pentru ca la queue nu poti parcurge lista si sa faci ce vrei. 
				// Mereu ai acces doar la ce urmeaza.
	}
	*tail = tmp; // reinitializam coada.
	return *tail;
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
Queue* createNode(NodeInfo* info) // Crearea unui nou nod.
{
	Queue* node = (Queue*)malloc(sizeof(Queue)); // Alocam memorie pentru noul nod.
	node->info = info; // Bagam informatia.
	node->pNext = NULL; // Nu are un element urmator, pt ca doar il cream momentan.
	return node; // Returnam nodul.
}



#pragma endregion


void printInfo(NodeInfo* info) // Afisarea info din employee
{
	printf("Code: %d - Name: %s - StudentId: %d\n", info->evaluationId, info->name, info->studentId);
}

NodeInfo* createInfo(int evaluationId, int noCredits, char* name, int studentId, double grade) // Functia de creare a unui nou employee
{
	struct Evaluation* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam memorie pentru employee
	emp->evaluationId = evaluationId; // punem codu	
	emp->name = (char*)malloc(strlen(name) + 1); // alocam memorie pentru nume
	strcpy(emp->name, name); // Punem numele
	emp->noCredits = noCredits; // Punem salariul
	emp->grade = grade;
	emp->studentId = studentId;
	return emp; // returnam employee-ul.
}


