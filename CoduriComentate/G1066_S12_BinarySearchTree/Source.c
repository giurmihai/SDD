#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "string.h"

// Binary Search Tree - same dataset as before.

struct Employee // Struct-ul employee ce contine info despre un emp.
{
	short code;
	char* name;
	char* dept;
	double salary;
};
typedef struct Employee NodeInfo;

typedef struct BST // Structul binary search tree
{
	struct BST* left; // copilul stanga
	NodeInfo* info; // Informatia nodului
	struct BST* right; // copilul dreapta
}BinarySearchTree;


#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
BinarySearchTree* createNode(NodeInfo*);
void printInfo(NodeInfo* info);
/*functions signatures for BST operations*/
void insertBST(BinarySearchTree**, NodeInfo*);
void deleteBST(BinarySearchTree**, short);
void inorder_LPR(BinarySearchTree*);
void preorder_PLR(BinarySearchTree*);
void printLeaves(BinarySearchTree*);
void main()
{
	BinarySearchTree* bTree = NULL; // declaram bst-ul nostru

	FILE* pFile = fopen("Data.txt", "r"); // deschidem fisierul
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n"; // declarari pentru citirea din document
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0; // declarari pentru stocarea datelor citite
	if (pFile) // Daca exista fisierul
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL) // Atunci citim pana la finalul lui
		{
			token = strtok(lineBuffer, sepList); 
			code = atoi(token); // Bagam codu intr-o variabila
			name = strtok(NULL, sepList); // numele
			dept = strtok(NULL, sepList); // departamentu
			token = strtok(NULL, sepList); 
			salary = atof(token); // salariul

			NodeInfo* info = createInfo(code, name, dept, salary); // Si cream un nou employee cu informatiile citite

			insertBST(&bTree, info); // Dupa care il inseram in binary search tree
			printf("\n*********************************\n");
			preorder_PLR(bTree); // Afisarea in preorder (Root - left - right)

		}

		printf("\n***********************************\n");
		deleteBST(&bTree, 10003); // Stergerea nodului cu un anumit cod.
		//inorder_LPR(bTree); // functia de afisare inorder (left - root - parent)
		printf("\n***********************************\n");
		printLeaves(bTree); // Functia de afisare a frunzelor
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
		if ((*root)->info->code > emp->code) // atunci mergem la stanga
			insertBST(&(*root)->left, emp);								//( in stanga vor fi nodurile cu val mai mici, la dreapta mai mari)
		else if ((*root)->info->code < emp->code) // sau la dreapta 
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
		free(tmpInfo->name); // eliberam memoria pt name
		free(tmpInfo->dept); // eliberam dept
		free(tmpInfo); // eliberam nodul
		free(tmpNode); // si eliberam si copilul
	}
}
void deleteBST(BinarySearchTree** root, short key) // Stergerea unui nod in functie de code.
{
	if (*root != NULL) // Daca avem un arbore
	{
		if ((*root)->info->code > key) // Atunci verificam daca suntem la nodul cu code bun. 
											// Daca nu, atunci mergem la stanga sau la dreapta in functie de parametrul cautat.
			deleteBST(&(*root)->left, key); 
		else if ((*root)->info->code < key)
			deleteBST(&(*root)->right, key);
		else // In schimb daca il gasim, atunci
		{
			if ((*root)->left == NULL && (**root).right == NULL) // verificam daca nu are copii
			{
				free((*root)->info->name); // daca nu are copii atunci dam free la memoria pentru nume
				free((*root)->info->dept); // la cea pentru departament
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
				free((*root)->info->name); // si eliberam numele
				free((*root)->info->dept); // eliberam dept
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
		printf("Code: %d - Name: %s, Salary: %f\n", info->code, info->name, info->salary);
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
NodeInfo* createInfo(short code, char* name, char* dept, double salary) // crearea unui employee
{
	struct Employee* emp = (NodeInfo*)malloc(sizeof(NodeInfo)); // alocam spatiu pentru un nou employee
	emp->code = code; // Bagam codu
	emp->name = (char*)malloc(strlen(name) + 1); // alocam spatiu pentru nume
	strcpy(emp->name, name); // bagam numele
	emp->dept = (char*)malloc(strlen(dept) + 1); // alocam spatiu pentru departament
	strcpy(emp->dept, dept); // bagam departamentu
	emp->salary = salary; // bagam salariu
	return emp; // si returnam employee-ul.
}