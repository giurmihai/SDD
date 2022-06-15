#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"

// Binary Search Tree - same dataset as before.

struct Event // Struct-ul employee ce contine info despre un emp.
{
	int idEvent;
	float ticketPrice;
	char* clientName;
	char eventDate[10];
	int noPersons;
	char* location;
};
typedef struct Event NodeInfo;

typedef struct BST // Structul binary search tree
{
	struct BST* left; // copilul stanga
	NodeInfo* info; // Informatia nodului
	struct BST* right; // copilul dreapta
}BinarySearchTree;


#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(int, float, char*, char*, int, char*);
BinarySearchTree* createNode(NodeInfo*);
void printInfo(NodeInfo* info);
/*functions signatures for BST operations*/
void insertBST(BinarySearchTree**, NodeInfo*);
void deleteBST(BinarySearchTree**, short);
void inorder_LPR(BinarySearchTree*);
void preorder_PLR(BinarySearchTree*);
void printLeaves(BinarySearchTree*);

void getRes(BinarySearchTree*, char*, NodeInfo**,int*);
void calcTotalCost(BinarySearchTree*, char*, float*);
void ShowForEachTotalCost(BinarySearchTree*, BinarySearchTree*,char**,int*);


void main()
{
	BinarySearchTree* bTree = NULL; // declaram bst-ul nostru

	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citirea din document
	int idEvent=0;
	float ticketPrice=0.0f;
	char* clientName=NULL;
	char* eventDate=NULL;
	int noPersons=0;
	char* location=NULL;
	if (pFile) // Daca exista fisierul
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // Atunci citim pana la finalul lui
		{
			token = strtok(lineBuffer, sepList);
			idEvent = atoi(token); // Bagam codu intr-o variabila
			token = strtok(NULL, sepList);
			ticketPrice = atof(token);
			clientName = strtok(NULL, sepList); // numele
			eventDate= strtok(NULL, sepList);
			token = strtok(NULL, sepList);
			noPersons = atoi(token); // salariul
			location = strtok(NULL, sepList);

			NodeInfo* info = createInfo(idEvent, ticketPrice, clientName, eventDate, noPersons, location); // Si cream un nou employee cu informatiile citite
			//printInfo(info);
			insertBST(&bTree, info); // Dupa care il inseram in binary search tree
			//printf("\n************* Afisarea preorder (Root - left - right) *************\n");
			//preorder_PLR(bTree); // Afisarea in preorder (Root - left - right)

		}

		//printf("\n***********************************\n");
		//deleteBST(&bTree, 10003); // Stergerea nodului cu un anumit cod.
		inorder_LPR(bTree); // functia de afisare inorder (left - root - parent)
		printf("\n-------- Get Res -------------\n");
		int NoOfResults=0;
		NodeInfo** merge=(NodeInfo**)malloc(sizeof(NodeInfo*)*20);
		getRes(bTree, "2/11/2022",merge, &NoOfResults);
		for (int i = 0; i < NoOfResults; i++)
		{
			printInfo(merge[i]);
		}
		printf("\n-------- Get Client Cost -------------\n");
		float totalCost = 0;
		calcTotalCost(bTree, "Giur", &totalCost);
		printf("Total cost Giur: %f", totalCost);

		printf("\n-------- Get All Clients Cost -------------");
		int noClients=0;
		char** totals = (char**)malloc(sizeof(char*) * 999);

		ShowForEachTotalCost(bTree,bTree,totals,&noClients);
		for (int i = 0; i < noClients; i++)
		{
			printf("\n%s ", totals[i]);
		}


		//printf("\n***********************************\n");
		//printLeaves(bTree); // Functia de afisare a frunzelor
	}
}
void printLeaves(BinarySearchTree* root) // Functia de print leaves -> Left - root - right
{
	if (root) // daca avem un BST
	{
		printLeaves(root->left); // atunci mergem la stanga
		if (root->left == NULL && root->right == NULL) // Daca nu are copii il afisam
			printInfo(root->info);
		printLeaves(root->right); // si mergem la dreapta 
	}
}

void preorder_PLR(BinarySearchTree* root) // Afisarea in preorder (Root - left - right)
{
	if (root) // daca avem un arbore
	{
		printInfo(root->info); // atunci afisam informatia
		preorder_PLR(root->left); // si mergem la stanga
		preorder_PLR(root->right); // si la dreapta
										// (loc de unde vom afisa informatia si apoi din nou, vom afisa)
	}
}

void getRes(BinarySearchTree* root ,char* dateRetrieve,NodeInfo** result, int* index) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{

	getRes(root->left, dateRetrieve, result,index); // mergem la stanga
	getRes(root->right, dateRetrieve, result, index); // mergem la dreapta

	if (strcmp(root->info->eventDate, dateRetrieve) == 0)
		{
			//printInfo(root->info);
			NodeInfo* newNod = createInfo(root->info->idEvent, root->info->ticketPrice, root->info->clientName, root->info->eventDate,
				root->info->noPersons, root->info->location);
			result[(*index)] = newNod;
			(*index)++;
		}
	}
}

void ShowForEachTotalCost(BinarySearchTree* root, BinarySearchTree* origin, char** totals, int* noClients) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		ShowForEachTotalCost(root->left,origin,totals,noClients); // mergem la stanga
		float totalCost1 = 0.0f;
		calcTotalCost(origin, root->info->clientName, &totalCost1);
		totals[(*noClients)] = (char*)malloc(sizeof(root->info->clientName));
		strcpy(totals[(*noClients)++], root->info->clientName);
		char ceva[30];
		//strcpy(ceva, totalCost1);
		sprintf(ceva, "%f", totalCost1);
		totals[(*noClients)++] = ceva;
		/*char* mama=(char*)malloc(sizeof(char)*sizeof(float));
		sprintf(mama, "%f", totalCost1);
		totals[(*noClients)++] = mama;*/
		//strcpy(totals[(*noClients)++], itoa(totalCost1));
		//printf("\nClientul %s are total cost de %f", root->info->clientName, totalCost1);

		ShowForEachTotalCost(root->right,origin,totals,noClients); // mergem la dreapta
	}
}

void calcTotalCost(BinarySearchTree* root,char* numeClient,float* totalCost) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		calcTotalCost(root->left, numeClient, totalCost); // mergem la stanga
		if (strcmp(root->info->clientName, numeClient) == 0)
		{
			//printInfo(root->info);
			(*totalCost) += root->info->ticketPrice;
		}
		 // afisham
		calcTotalCost(root->right, numeClient, totalCost); // mergem la dreapta
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

void insertBST(BinarySearchTree** root, NodeInfo* emp) // Inserarea in binary search tree
{
	if (*root == NULL) // Daca arborele e null
	{
		*root = createNode(emp); // atunci cream radacina cu informatiile din emp.
	}
	else // daca exista
	{
		if ((*root)->info->idEvent > emp->idEvent) // atunci mergem la stanga
			insertBST(&(*root)->left, emp);								//( in stanga vor fi nodurile cu val mai mici, la dreapta mai mari)
		else if ((*root)->info->idEvent < emp->idEvent) // sau la dreapta 
			insertBST(&(*root)->right, emp);
		else // iar daca e egal
			(*root)->info = emp; // dam override la informatie
	}
}
void logicalDeletion(BinarySearchTree** root, BinarySearchTree** leftSubTree) // Logical deletetion in cazul in care un nod are 2 copii
{
	if ((*leftSubTree)->right) // daca si asta are un copil la dreapta
		logicalDeletion(root, &(*leftSubTree)->right); // atunci mergem pe cel mai din dreapta copil
	else // si cand ajungem la cel mai din dreapta copil
	{
		NodeInfo* tmpInfo = (*root)->info; // pastram un pointer la informatia din nodul ce trb sters
		BinarySearchTree* tmpNode = (*leftSubTree); // luam un pointer la copil
		(*root)->info = (*leftSubTree)->info; // inlocuim informatia
		(*leftSubTree) = (*leftSubTree)->left; // si inlocuim nodul
		free(tmpInfo->clientName); // eliberam memoria pt name
		free(tmpInfo->location); // eliberam dept
		free(tmpInfo); // eliberam nodul
		free(tmpNode); // si eliberam si copilul
	}
}
void deleteBST(BinarySearchTree** root, short key) // Stergerea unui nod in functie de code.
{
	if (*root != NULL) // Daca avem un arbore
	{
		if ((*root)->info->idEvent > key) // Atunci verificam daca suntem la nodul cu code bun. 
											// Daca nu, atunci mergem la stanga sau la dreapta in functie de parametrul cautat.
			deleteBST(&(*root)->left, key);
		else if ((*root)->info->idEvent < key)
			deleteBST(&(*root)->right, key);
		else // In schimb daca il gasim, atunci
		{
			if ((*root)->left == NULL && (**root).right == NULL) // verificam daca nu are copii
			{
				free((*root)->info->clientName); // daca nu are copii atunci dam free la memoria pentru nume
				free((*root)->info->location); // la cea pentru departament
				free((*root)->info); // la cea pentru info
				free((*root)); // si in final la tot nodul.
				*root = NULL; // il facem si null ca sa nu avem un dangling pointer
			}
			else if ((*root)->left == NULL || (*root)->right == NULL) // daca are 1 copil
			{
				BinarySearchTree* desc = NULL; // atunci avem o variabila auxiliara
				if ((*root)->left) // verificam ce copil are, stanga sau dreapta
					desc = (*root)->left; // si il salvam
				else
					desc = (*root)->right;// --^
				free((*root)->info->clientName); // si eliberam numele
				free((*root)->info->location); // eliberam dept
				free((*root)->info); // eliberam informatia
				free(*root); // si nodul
				*root = desc; // apoi ne folosim de pointerul avut deja si ii
									//dam adresa copilului (parintele nodului curent va avea info copilului(nodului curent))
			}
			else // daca are 2 copii atunci e putin mai complicat 
				logicalDeletion(root, &(*root)->left); // si facem un logical deletion
		}
	}
}
void printInfo(NodeInfo* info) // Printinfo de la un employee
{
	if (info)
		printf("Code: %d - Pret Bilet: %f - Name: %s  - Event Date: %s - No Persons: %d - Location: %s\n", info->idEvent,  info->ticketPrice,info->clientName,info->eventDate
		, info->noPersons,info->location);
	else
		printf("No data to print!");
}
BinarySearchTree* createNode(NodeInfo* emp) // Crearea unui nou nod pentru arbore
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree)); // alocam spatiu pentru el
	node->info = emp; // ii dam informatia
	node->left = node->right = NULL; // si stabilim ca nu are copii
	return node; // apoi il returnam
}
NodeInfo* createInfo(int idEvent, float ticketPrice, char* clientName, char* eventDate, int noPersons, char* location) // crearea unui employee
{
	/*
	int idEvent = 0;
	float ticketPrice = 0.0f;
	char* clientName = NULL;
	char eventDate[10];
	int noPersons = 0;
	char* location = NULL;
	*/

	struct Event* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam spatiu pentru un nou employee
	emp->idEvent = idEvent; // Bagam codu
	emp->ticketPrice = ticketPrice;
	emp->clientName = (char*)malloc(strlen(clientName) + 1); // alocam spatiu pentru nume
	strcpy(emp->clientName, clientName); // bagam numele
	strcpy(emp->eventDate, eventDate);
	emp->location = (char*)malloc(strlen(location) + 1); // alocam spatiu pentru departament
	strcpy(emp->location, location); // bagam departamentu
	emp->noPersons = noPersons; // bagam salariu
	return emp; // si returnam employee-ul.
}