#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"

// Binary search Tree - AVL (se auto balanseaza)

struct Employee // structul pentru employee
{
	short code;
	char* name;
	char* dept;
	double salary;
};
typedef struct Employee NodeInfo;

typedef struct BST // struct pentru binary search tree
{
	struct BST* left; // copilul din stanga
	NodeInfo* info; // informatia
	struct BST* right; // copilul din dreapta
	short bFactor; // factorul de rebalansare
}BinarySearchTree;


#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
BinarySearchTree* createNode(NodeInfo*);
void printInfo(NodeInfo* info);
/*functions signatures for BST-AVL operations*/
BinarySearchTree* rebalance(BinarySearchTree*);
void insertBST(BinarySearchTree**, NodeInfo*);
void printLevels(BinarySearchTree*);

void printLeaves(BinarySearchTree* root) // functia de printleaves -> parcurge arborele si afiseaza doar unde nu are copii (Root - left - right)
{
	if (root) // daca exista arborele
	{
		if(root->left == NULL && root->right == NULL) // verificam daca nu are copii
			printInfo(root->info); // afisam
		printLeaves(root->left); // mergem la stanga
		printLeaves(root->right); // mergem la dreapta
	}
}

void preorder_PLR(BinarySearchTree* root) // afisarea, parcurgerea in preorder (root - left - right)
{
	if (root) // daca exista arborele
	{
		printInfo(root->info); // afisham
		preorder_PLR(root->left); // mergem stanga
		preorder_PLR(root->right); // mergem dreapta
	}
}
void inorder_LPR(BinarySearchTree* root)// afisarea inorder (left - root - right)
{
	if (root) // daca exista arborele
	{
		inorder_LPR(root->left); // mergem la stanga
		printInfo(root->info); // afisam
		inorder_LPR(root->right); // mergem la dreapta
	}
}

void main()
{
	BinarySearchTree* bTree = NULL; // declaram un binary search tree gol.

	FILE* pFile = fopen("Data.txt", "r"); // deschidem  fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citirea din fisier
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // declarari pentru stocarea temporara a ce citim
	if (pFile) // daca exista fisierul deschis
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // atunci citim pana la finalul lui
		{
			// atoi - array to int
			token = strtok(lineBuffer, sepList);
			code = atoi(token); // citim codul
			name = strtok(NULL, sepList); // citim numele
			dept = strtok(NULL, sepList); // citim departamentu
			token = strtok(NULL, sepList); 
			salary = atof(token); // citim salariul

			NodeInfo* info = createInfo(code, name, dept, salary); // si cream un nou employee

			insertBST(&bTree, info); // si apoi bagam informatia intr-un nou nod din arbore
			printf("\n*********************************\n");
			preorder_PLR(bTree); // afisarea in preorder

		}
		printLevels(bTree); // afisarea 
	}
}

short height(BinarySearchTree* root) // returneaza adancimea arborelui
{
	if (root) //daca avem un arbore
		return 1 + max(height(root->left), height(root->right));  // atunci calculam adancimea stanga si dreapta si luam maximul. 
																	// la care adaugam 1 (radacina)
	else
		return 0;
}
void printLevel(BinarySearchTree* root, int lvl) // afiseaza nodurile de pe un anumit nivel al arborelui.
{
	if (root) // daca exista arborele
	{
		if(lvl == 0) // daca ajunge la levelul cerut (aici numaram cati pasi a facut) atunci doar afiseaza nodul
			printInfo(root->info);				// in loc sa avem un contor ++, doar am scazut ala
		printLevel(root->left, lvl-1); // altfel se duce la stanga cu un nivel mai mic
		printLevel(root->right, lvl-1); // si la dreapta
	}
}
void printLevels(BinarySearchTree* root) // afiseaza levelele 
{
	int levels = height(root); // numara nivelele arborelui
	for (int i = 0; i < levels; i++) // se duce pe fiecare nivel
	{
		printf("\nLevel %d\n", i); // afiseaza nivelul
		printLevel(root, i); // si nodurile de pe nivel
	}
}
void insertBST(BinarySearchTree** root, NodeInfo* emp) // introducerea informatiei intr-un nou nod din arbore
{
	if (*root == NULL) // daca arborele este null
	{
		*root = createNode(emp); // atunci noul nostru nod va fi si radacina
	}
	else // altfel
	{
		if ((*root)->info->code > emp->code) // parcurgem arborele la stanga
			insertBST(&(*root)->left, emp);
		else if ((*root)->info->code < emp->code) // sau la dreapta pana ii gasim locul unde trebuie inserat
			insertBST(&(*root)->right, emp);
		else
			(*root)->info = emp; // si il bagam in arbore.
	}
	*root = rebalance(*root); // dupa ce l-am inserat in arbore, il rebalansam
}

BinarySearchTree* rightRotation(BinarySearchTree* pivot) // rotirea la dreapta
{
	BinarySearchTree* desc = pivot->left; // luam o variabila auxiliara ce va fi egala cu parintele stang
	pivot->left = desc->right; // si facem rotatia
	desc->right = pivot;
	return desc; // returnam ce a iesit
}
BinarySearchTree* leftRotation(BinarySearchTree* pivot) // rotirea la stanga (ivers fata de cea de la dreapta)
{
	BinarySearchTree* desc = pivot->right; // luam o variabila auxiliara ce va fi egala cu parintele drept
	pivot->right = desc->left; // si facem rotatia
	desc->left = pivot;
	return desc; // returnam ce a iesit
}
BinarySearchTree* rebalance(BinarySearchTree* root) // rebalansarea arborelui
{
	root->bFactor = height(root->right) - height(root->left); // calculam factorul de rebalansare (formula in curs)
	if (root->bFactor == -2) // daca e -2 verificam semnul factorului copilului
	{
		BinarySearchTree* desc = root->left; // luam o variabila auxiliara pentru copilul din stanga
		if (desc->bFactor == -1) // iar daca asta are factorul de acelasi semn cu parintele (ambele sunt cu -) 
			root = rightRotation(root); // atunci il rotim la dreapta
		else // in schimb daca au semne diferite
		{
			root->left = leftRotation(desc); // vom avea 2 rotatii, una la stanbga si una la dreapta
			root = rightRotation(root);
		}
	}
	else if (root->bFactor == 2) // daca e +2 verificam semnul factorului copilului
	{
		BinarySearchTree* desc = root->right; // varibila auxiliara pentru copilul din dreapta
		if (desc->bFactor == -1) // daca semnul este diferit atunci vom face intai o rotatie la dreapta
			root->right = rightRotation(desc);
		root = leftRotation(root); // urmata de o rotatie la stanga

	}
	// Info: cele doua if-uri cu -2 si +2 fac acelasi lucru doar ca in reverse. (sunt scrise diferit pt a arata diferite sintaxe ale if-ului cred)


	return root; // returnam arborele rebalansat
}
void printInfo(NodeInfo* info) // Afisarea unui employee.
{
	if (info) // daca exista afisam 
		printf("Code: %d - Name: %s, Salary: %f\n", info->code, info->name, info->salary);
	else // daca nu, afisam ca nu avem ce printa
		printf("No data to print!");
}

BinarySearchTree* createNode(NodeInfo* emp) // crearea unui nod pentru arbore
{
	BinarySearchTree* node = (BinarySearchTree*)malloc(sizeof(BinarySearchTree)); // alocam spatiu pentru el
	node->info = emp; // bagam informatia
	node->left = node->right = NULL; // si nu va avea copii 
	node->bFactor = 0; // factorul de rebalansare va fi 0
	return node; // il returnam 
}

NodeInfo* createInfo(short code, char* name, char* dept, double salary) // crearea unui nou employee
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam spatiu pentru un nou employee
	emp->code = code; // bagam codu
	emp->name = (char*)malloc(strlen(name) + 1); // alocam spatiu pentru nume
	strcpy(emp->name, name); // bagam numele
	emp->dept = (char*)malloc(strlen(dept) + 1); // alocam spatiu pentru departament
	strcpy(emp->dept, dept); // bagam departamentu
	emp->salary = salary; // bagam si salariul
	return emp; // si returnam employee-ul
}