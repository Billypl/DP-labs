#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARSNUM 100
#define memalloc(type, size) (type*)malloc(sizeof(type) * size)

char* getStr()
{
	char buffer[MAXCHARSNUM];
	do {
		fgets(buffer, MAXCHARSNUM, stdin);
	} while (buffer[0] == '\n');
	char* result = memalloc(char, strlen(buffer) + 1);
	strcpy(result, buffer);

	return result;
}

int isEalier(const char* str1, const char* str2)
{
	int i = 0;
	while (i < strlen(str1) && i < strlen(str2))
	{
		if (str1[i] == str2[i])
		{
			i++;
			continue;
		}
		return str1[i] > str2[i];
	}
}

typedef struct Node
{
	struct Node* prev;
	char* str;
	struct Node* next;
} Node;
struct Node* begin = NULL;

void setupList()
{
	begin = memalloc(Node, 1);
	begin->prev = NULL;
	begin->next = NULL;
	begin->str = NULL;
}

void destroyList()
{
	Node* currentNode = begin;
	while (currentNode != NULL)
	{
		Node* nextNode = currentNode->next;
		free(currentNode->str);
		free(currentNode);
		currentNode = nextNode;
	}
}

void addNode(char* str)
{
	Node* currentNode = begin;
	Node* lastNode = begin;

	while (currentNode != NULL)
	{
		if (isEalier(currentNode->str, str))
		{
			lastNode = currentNode;
			currentNode = currentNode->next;
		}
		else
		{
			break;
		}
	}

	// pusta lista
	if(begin == NULL)
	{
		setupList();
		begin->str = str;
		return;
	}

	Node* newNode = memalloc(Node, 1);
	newNode->str = str;
	newNode->prev = NULL;
	newNode->next = NULL;

	// dodanie na koniec
	if (currentNode == NULL)
	{
		lastNode->next = newNode;
		newNode->prev = lastNode;
		return;
	}

	// dodanie na pocz¹tek
	if (currentNode == begin)
	{
		newNode->next = begin;
		begin->prev = newNode;
		begin = newNode;
		return;
	}

	// dodanie w œrodku
	newNode->prev = currentNode->prev;
	newNode->next = currentNode;
	newNode->prev->next = newNode;
	currentNode->prev = newNode;
	return;
}


void removeNode(size_t index)
{
	Node* currentNode = begin;
	for(int i = 0; i < index; i++)
	{
		if(currentNode == NULL)
		{
			printf("Nie ma takiego elementu\n");
			return;
		}		
		currentNode = currentNode->next;
	}


	free(currentNode->str);
	if(currentNode->prev != NULL)
	{
		currentNode->prev->next = currentNode->next;
	}
	else
	{
		begin = currentNode->next;
	}
	if(currentNode->next != NULL)
	{
		currentNode->next->prev = currentNode->prev;
	}
	free(currentNode);
}

void printList()
{
	Node* currentNode = begin;
	printf("\nZawartosc listy:\n");

	while (currentNode != NULL)
	{
		printf("%s", currentNode->str);
		currentNode = currentNode->next;
	}
}



void showMenu()
{
	printf("\n1. Dodaj element:\n");
	printf("2. Usun element (podaj indeks):\n");
	printf("3. Wyswietl liste\n");
	printf("4. Zakoncz\n");
}


int main()
{	
	while (1)
	{
		showMenu();
		int choice;
		int index;

		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("Podaj wartosc:");
			addNode(getStr());
			break;
		case 2:
			printf("Podaj indeks:");
			scanf("%d", &index);
			removeNode(index);
			break;
		case 3:
			printList();
			break;
		case 4:
			destroyList();
			return 0;
		}
	}
}