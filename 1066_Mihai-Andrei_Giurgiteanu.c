#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"

// Binary Search Tree - same dataset as before.

struct Flight // Struct-ul employee ce contine info despre un emp.
{
	char* aiplaneCode;
	unsigned short landingTime;
	char* comingFrom;
	unsigned short noOfPassagers;
};
typedef struct Flight NodeInfo;

typedef struct BST // Structul binary search tree
{
	struct BST* left; // copilul stanga
	NodeInfo* info; // Informatia nodului
	struct BST* right; // copilul dreapta
}BinarySearchTree;


#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(char*, unsigned short, char*, unsigned short);
BinarySearchTree* createNode(NodeInfo*);
void printInfo(NodeInfo* info);
/*functions signatures for BST operations*/
void insertBST(BinarySearchTree**, NodeInfo*);
void deleteBST(BinarySearchTree**, short);
void inorder_LPR(BinarySearchTree*);
void preorder_PLR(BinarySearchTree*);
void printLeaves(BinarySearchTree*);
void afisInvers(BinarySearchTree* root);
void calcMaxPas(BinarySearchTree*, int*, NodeInfo**);
void printInterval(BinarySearchTree*, int, int);
void calcMinLDT(BinarySearchTree*, int*, NodeInfo**);
void deleteNextFlight(BinarySearchTree*);
void NoPlanesLowerT(BinarySearchTree*, int*, int);
void FindLeafParents(BinarySearchTree*, int*, NodeInfo**,int,int*);
void main()
{
	BinarySearchTree* bTree = NULL; // declaram bst-ul nostru

	FILE* pFile = fopen("1066_Mihai-Andrei_Giurgiteanu.txt", "r"); // deschidem fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], *sepList = ","; // declarari pentru citirea din document
	char* aiplaneCode = NULL, *comingFrom = NULL;unsigned short landingTime = 0;unsigned short noOfPassagers = 0.0; // declarari pentru stocarea datelor citite
	if (pFile) // Daca exista fisierul
	{

		/*char* aiplaneCode;
		unsigned short landingTime;
		char* comingFrom;
		unsigned short noOfPassagers;*/
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // Atunci citim pana la finalul lui
		{

			aiplaneCode = strtok(lineBuffer, sepList);
			token = strtok(NULL, sepList);
			landingTime = atoi(token); // Bagam codu intr-o variabila
			comingFrom = strtok(NULL, sepList); // numele
			token = strtok(NULL, sepList);
			noOfPassagers = atoi(token);

			NodeInfo* info = createInfo(aiplaneCode, landingTime, comingFrom, noOfPassagers); // Si cream un nou employee cu informatiile citite
			//printInfo(info);
			insertBST(&bTree, info); // Dupa care il inseram in binary search tree
			//printf("\n*********************************\n");
		//	preorder_PLR(bTree); // Afisarea in preorder (Root - left - right)

		}
		printf("\n************ Afish Invers ************\n");
		afisInvers(bTree);
		printf("\n************* Nod cu maxim de persoane *************\n");
		int maxim = -1;
		NodeInfo* nodMaxim=NULL;
		calcMaxPas(bTree, &maxim, &nodMaxim);
		printInfo(nodMaxim);
		printf("\n************* Nodurile din intervalul [x, y] *************\n");
		int x = 22;
		int y = 27;
		printInterval(bTree, x, y);
		
		printf("\n***********After deletion****************\n");
		deleteNextFlight(&bTree);
		inorder_LPR(bTree);
		printf("\n************ How many planes are scheduled to land and times <=t **************\n");
		int t = 25;
		int noPlanes = 0;
		NoPlanesLowerT(bTree,&noPlanes,t);
		printf("Number of planes with arrival time lower than %d: %d", t, noPlanes);
		//deleteBST(&bTree, 10003); // Stergerea nodului cu un anumit cod.
		////inorder_LPR(bTree); // functia de afisare inorder (left - root - parent)
		printf("\n************ Print all the way to frunza **************\n");
		//printLeaves(bTree); // Functia de afisare a frunzelor
		int landingTimeP = 36;
		NodeInfo** arrayRet = (NodeInfo**)malloc(sizeof(NodeInfo*) * 100);
		int gasit = 0;
		int index = 0;
		FindLeafParents(bTree, &gasit, arrayRet, landingTimeP, &index);
		for (int i = 0; i < index; i++)
		{
			printInfo(arrayRet[i]);
		}
	}
}

void FindLeafParents(BinarySearchTree* root,int* gasit,NodeInfo** arrayRet, int landingTimeP,int* index) {
	if (root) // daca avem un BST
	{
		FindLeafParents(root->left, gasit, arrayRet, landingTimeP,index);
		FindLeafParents(root->right, gasit, arrayRet, landingTimeP,index);
		if (landingTimeP == root->info->landingTime)
			(*gasit) = 1;
		if ((*gasit) == 1)
		{
			NodeInfo* newNode = createInfo(root->info->aiplaneCode,root->info->landingTime,root->info->comingFrom, root->info->noOfPassagers);
			//printInfo(newNode);
			arrayRet[(*index)] = newNode;
			(*index)++;
		}
		
	}
}

void deleteNextFlight(BinarySearchTree** root) {
	int minim = 9999;
	NodeInfo* nodMinim = NULL;
	calcMinLDT((*root), &minim, &nodMinim);
	//printInfo(nodMinim);
	deleteBST(root, nodMinim->landingTime);
	
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

void calcMinLDT(BinarySearchTree* root, int* minim, NodeInfo** nodAfish) // Afisarea in preorder (Root - left - right)
{
	if (root) // daca avem un arbore
	{
		if (root->info->landingTime < (*minim))
		{
			(*minim) = root->info->landingTime;
			(*nodAfish) = root->info;
		}
		calcMinLDT(root->left, minim, nodAfish); // si mergem la stanga
		calcMinLDT(root->right, minim, nodAfish); // si la dreapta
										// (loc de unde vom afisa informatia si apoi din nou, vom afisa)
	}
}

void calcMaxPas(BinarySearchTree* root,int* maxim,NodeInfo** nodAfish) // Afisarea in preorder (Root - left - right)
{
	if (root) // daca avem un arbore
	{
		if (root->info->noOfPassagers > (*maxim))
		{
			(*maxim) = root->info->noOfPassagers;
			(*nodAfish) = root->info;
		}
		calcMaxPas(root->left,maxim,nodAfish); // si mergem la stanga
		calcMaxPas(root->right,maxim,nodAfish); // si la dreapta
										// (loc de unde vom afisa informatia si apoi din nou, vom afisa)
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

void afisInvers(BinarySearchTree* root) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{

		afisInvers(root->right); // mergem la dreapta
		afisInvers(root->left); // mergem la stanga
		printInfo(root->info); // afisham
	}
}
void NoPlanesLowerT(BinarySearchTree* root, int* noPlanes, int t) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		NoPlanesLowerT(root->left,noPlanes, t); // mergem la stanga
		if (root->info->landingTime <= t)
		{
			(*noPlanes)++;
		}

		NoPlanesLowerT(root->right,noPlanes, t); // mergem la dreapta
	}
}

void printInterval(BinarySearchTree* root,int x, int y) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		printInterval(root->left,x,y); // mergem la stanga
		if (root->info->landingTime >= x && root->info->landingTime <= y)
		{
			printInfo(root->info); // afisham
		}
		
		printInterval(root->right,x,y); // mergem la dreapta
	}
}

void deleteBSTT(BinarySearchTree* root) // afisarea in ordine (Left - Root - right)
{
	if (root)
	{
		deleteBSTT(root->left); // mergem la stanga
		deleteBSTT(root->right); // mergem la dreapta
		free(root->info->aiplaneCode);
		free(root->info->comingFrom);
		free(root->info);
		free(root);
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
		if ((*root)->info->landingTime > emp->landingTime) // atunci mergem la stanga
			insertBST(&(*root)->left, emp);								//( in stanga vor fi nodurile cu val mai mici, la dreapta mai mari)
		else if ((*root)->info->landingTime < emp->landingTime) // sau la dreapta 
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
		free(tmpInfo->aiplaneCode); // eliberam memoria pt name
		free(tmpInfo->comingFrom); // eliberam dept
		free(tmpInfo); // eliberam nodul
		free(tmpNode); // si eliberam si copilul
	}
}
void deleteBST(BinarySearchTree** root, short key) // Stergerea unui nod in functie de code.
{
	if (*root != NULL) // Daca avem un arbore
	{
		if ((*root)->info->landingTime > key) // Atunci verificam daca suntem la nodul cu code bun. 
											// Daca nu, atunci mergem la stanga sau la dreapta in functie de parametrul cautat.
			deleteBST(&(*root)->left, key);
		else if ((*root)->info->landingTime < key)
			deleteBST(&(*root)->right, key);
		else // In schimb daca il gasim, atunci
		{
			if ((*root)->left == NULL && (**root).right == NULL) // verificam daca nu are copii
			{
				free((*root)->info->aiplaneCode); // daca nu are copii atunci dam free la memoria pentru nume
				free((*root)->info->comingFrom); // la cea pentru departament
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
				free((*root)->info->aiplaneCode); // si eliberam numele
				free((*root)->info->comingFrom); // eliberam dept
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
		printf("Airplane code: %s, Landing time: %d, NoPassangers %d\n", info->aiplaneCode, info->landingTime,info->noOfPassagers);
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
NodeInfo* createInfo(char* aiplaneCode, unsigned short landingTime, char* comingFrom, unsigned short noOfPassagers) // crearea unui employee
{
	/*char* aiplaneCode;
		unsigned short landingTime;
		char* comingFrom;
		unsigned short noOfPassagers;*/
	struct Flight* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam spatiu pentru un nou employee
	

	emp->aiplaneCode = (char*)malloc(strlen(aiplaneCode) + 1); // alocam spatiu pentru nume
	strcpy(emp->aiplaneCode, aiplaneCode); // bagam numele

	emp->landingTime = landingTime; // Bagam codu

	emp->comingFrom = (char*)malloc(strlen(comingFrom) + 1); // alocam spatiu pentru departament
	strcpy(emp->comingFrom, comingFrom); // bagam departamentu
	emp->noOfPassagers = noOfPassagers; // bagam salariu
	return emp; // si returnam employee-ul.
}