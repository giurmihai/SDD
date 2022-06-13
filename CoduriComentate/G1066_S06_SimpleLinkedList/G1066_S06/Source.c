#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
//keyboad - stdin
//console - stdout

//
// SIMPLE LINKED LIST - lista simplu inlantuita
// 
// Se dau employees in Data.txt -> sa se bage intr-o lista simplu inlantuita
// Si sa se bage un nou employee la pozitia pos
// 
//


#define LINE_BUFFER 128
typedef struct Employee { // Se va folosi un struct pentru a stoca un Employee
	short code;
	char* name;
	char* department;
	double salary;
}Employee;
//typedef struct Employee Employee;
typedef struct Node // Asta e declararea listei noastre
{
	Employee* info; // Vom avea un pointer la un employee
	struct Node* pNext; // Si un pointer la urmatorul element
}Node;

/*functions' signatures for memory management*/
Employee* createInfo(short, char*, char*, double);
Node* createNode(Employee*);
/*functions' signatures for list operations*/
Node* insertHead(Node*, Employee*);
void insertTail(Node**, Employee*);
Node* insertListPos(Node*, Employee*, int);
void printList(Node*);
Employee* extractListTail(Node*);

void main()
{
	Node* list = NULL; // Cream o lista goala
	FILE* pFile = fopen("Data.txt", "r"); // Deschidem fisierul
	char lineBuffer[LINE_BUFFER], * token, sep_list[] = ","; // Declaram variabilele pentru citire
	Employee* emp = NULL; // Cream un employee gol
	if (pFile) // Daca exista fisierul
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile)) // Atunci citim pana la finalul lui
		{
			emp = (Employee*)malloc(sizeof(Employee)); // Alocam spatiu pentru un employee
			//parsing the code
			token = strtok(lineBuffer, sep_list); // Folosim strtok pentru ca fiecare employee e pe cate un rand, 
													//cu informatiile separate cu virgula (luam informatiile de la virgula pana la urm virgula)
			emp->code = atoi(token); // Atoi = array to int, transformam din string in int ca sa putem stoca codul

			//parsing the name
			token = strtok(NULL, sep_list);
			emp->name = (char*)malloc(strlen(token) + 1); // Nu uitam sa alocam memorie pentru ca e char* (pointer la o adresa de memorie)
			strcpy(emp->name, token); // Stocam temporar numele in emp

			//parsing the dept
			token = strtok(NULL, sep_list);
			emp->department = (char*)malloc(strlen(token) + 1); // Same ca la nume
			strcpy(emp->department, token); // Stocam temporar departamentul

			//parsing the salary
			token = strtok(NULL, sep_list);
			emp->salary = atof(token); // stocam temporar salariul

			//insert into the list
			insertTail(&list, emp); // Iar in final introducem tot ce am citit in lista
		} // Si repetam procesul

		printList(list); // Afisarea listei
		

		// Vrem sa introducem un nou employee la o anumita positie.
		Employee* e1 = createInfo(31000, "Badulescu Cristian", "Networking", 984.6); // Cream noul employee
		int pos = 431; // Pozitia la care vrem sa il bagam
		list = insertListPos(list, e1, pos); // Il bagam in lista la pozitia pos
		printf("\n------------After position insert----------------\n");
		printList(list); // Afisam din nou lista
		Employee* emp = extractListTail(list); // Vrem sa luam coada listei - neimplementat.

	}
}

//void printList( const Node* head)
//{
//	//head->pNext = NULL;
//	head = head->pNext;
//}

Employee* extractListTail(Node* head)
{

}
Node* insertListPos(Node* head, Employee* emp, int pos) // Functia de insertie la pozitia pos
{
	int index = 1; // pentru ca nu avem un index ca la vectori, luam un index ca sa numaram elementele prin care trecem
	Node* node = createNode(emp); // Cream un nou employee din informatiile primite
	if (pos <= 1) // Daca vrem sa inseram capu'
	{
		//insert at the head
		node->pNext = head; // nodul nostru va avea ca next actualul cap
		head = node; // Si schimbam capul sa fie nodul nou creat
	}
	else
	{	//insert at the pos
		Node* aux = head; // Daca nu, parcurgem lista, facem un aux ca sa nu pierdem head-ul listei (nu putem merge back pt ca e simplu inlantuita)
		while (aux->pNext && index++ < pos - 1) // mergem la pozitia ceruta
			aux = aux->pNext;
		if (aux->pNext) // Daca exista un element dupa 
		{
			node->pNext = aux->pNext; // atunci nodului nostru ii zicem sa pointuiasca la el
											// altfel, va ramane sa pointuiasca la null, iar in cazul in care nu exista cv dupa, e perf.
		}
		aux->pNext = node; // refacem legatura astfel incat sa introducem nodul nostru
	}
	return head; // Returnam capul listei.
}
void printInfo(Employee* emp)
{
	printf("Code: %d - Name: %s - Dept: %s - Salary: %f\n", // Afisarea unui employee
		emp->code,
		emp->name,
		emp->department,
		emp->salary);
}
void printList(const Node* head) // Afisarea listei.
{
	while (head) // Repetam pana se termina lista
	{
		printInfo(head->info); // il afisam
		head = head->pNext; // si trecem la urmatorul
	}							//repetam 
}
Employee* createInfo(short code, char* name, char* dept, double salary) // Cream un employee din parametrii primiti.
{
	Employee* emp = (Employee*)malloc(sizeof(Employee)); // Alocam spatiu pentru employee.
	emp->code = code; // Bagam codu.
	emp->name = (char*)malloc(strlen(name) + 1); // Alocam spatiu pentru name. Nu uitam +1 pentru ca avem si marca de final 
	strcpy(emp->name, name); // Copiem numele
	emp->department = (char*)malloc(strlen(dept) + 1); // locam spatiu pentru departament. Nu uitam +1 pentru ca avem si marca de final 
	strcpy(emp->department, dept); // Copiem departamentu
	emp->salary = salary; // Bagam si salariul
	return emp; // Returnam employee-ul creat.
}

Node* createNode(Employee* emp) // Cream un nod din lista.
{
	//declare and allocate memory
	Node* node = (Node*)malloc(sizeof(Node)); // Alocam spatiu pentru un nod.
	//initialize variable
	node->info = emp; // Bagam informatiile din parametrul primit. (pointer la ele)
	node->pNext = NULL; // Si din moment ce e nou, nu va avea un next. 
	//return value
	return node; // returnam nodul.
}

void insertTail(Node** head, Employee* emp) // Inseram la finalul listei. 
{
	Node* node = createNode(emp); // Cream un nou nod.
	if (*head == NULL) // Daca lista nu are niciun element.
		*head = node; // Atunci bagam un nou cap.
	else
	{
		Node* aux = *head; 
		while (aux->pNext) // Daca nu, parcurgem lista pana la final
			aux = aux->pNext; 
		aux->pNext = node; // Si introducem la final, nodul nou creat.
	}
}