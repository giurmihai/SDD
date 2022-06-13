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
}Node;

typedef struct Employee NodeInfo;
typedef struct node Queue;

#define LINE_BUFFEER 1024
/*functions signatures for memory management*/
NodeInfo* createInfo(short, char*, char*, double);
Node* createNode(NodeInfo*);
/*functions signatures for list operations*/
Queue* put(Queue*, NodeInfo*);
NodeInfo* get(Queue**);
void printInfo(NodeInfo* info);

void main()
{
	Queue* queueTail = NULL;

	FILE* pFile = fopen("Data.txt", "r");
	char* token = NULL, lineBuffer[LINE_BUFFEER], * sepList = ",\n";
	char* name = NULL, * dept = NULL; short code = 0; double salary = 0.0;
	if (pFile)
	{
		while (fgets(lineBuffer, sizeof(lineBuffer), pFile) != NULL)
		{
			token = strtok(lineBuffer, sepList);
			code = atoi(token);
			name = strtok(NULL, sepList);
			dept = strtok(NULL, sepList);
			token = strtok(NULL, sepList);
			salary = atof(token);

			NodeInfo* info = createInfo(code, name, dept, salary);
			queueTail = put(queueTail, info);

		}

		struct Employee* info = NULL;
		while((info = get(&queueTail))!= NULL)
		{
			printInfo(info);
			free(info->dept);
			free(info->name);
			free(info);
		}
		
	}
}

NodeInfo* get(Queue** tail)
{
	NodeInfo* value = NULL;
	if (*tail != NULL)
	{
		value = (*tail)->pNext->info; //1
		Queue* tmp = (*tail)->pNext;  //2
		if (*tail == (*tail)->pNext)
			*tail = NULL;
		else
			(*tail)->pNext = tmp->pNext;  //3
		free(tmp); //4
	}
	return value; //5
}


Queue* put(Queue* tail, NodeInfo* info)
{
	Queue* node = createNode(info);
	if (tail == NULL)
		node->pNext = node;
	else
	{
		node->pNext = tail->pNext;
		tail->pNext = node;
	}
	return node;
}

void printInfo(NodeInfo* info)
{
	printf("Code: %d - Name: %s\n", info->code, info->name);
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
	return node;
}