#ifndef _ARQUIVO_H
#define ARQUIVO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define TAM 16
#define DEL "*"

typedef int tipoChave;

typedef struct
{
	tipoChave numusp;
	int offset;
} tipoPrimaryIndex;

typedef struct
{
	char sobrenome[TAM];
	int offset;
} tipoSecondaryIndex;

typedef struct
{
	tipoChave numusp;
	int next;
} tipoInvertedList;

typedef struct
{
	char nome[TAM];
	tipoChave numUSP;
	char sobrenome[TAM];
	char curso[TAM];
	float nota;
} tipoAluno;

FILE* criaArquivo(FILE *df, char *filename);
void bubbleSort(tipoPrimaryIndex arr[], int n);
void* carregaIndice(FILE *file,int typesize, int *nRegisters);
void insereRegistro(FILE *file, tipoPrimaryIndex **primaryIndexArray,tipoSecondaryIndex **secondaryIndexArray, tipoInvertedList **invertedListArray, int *nRegisters,int *nSecondaryIndexRegisters);
int binarySearchNumUsp(tipoPrimaryIndex arr[], int l, int r, int x);
int binarySearchSobrenome(tipoSecondaryIndex arr[], int l, int r, char* sobrenome);
tipoAluno *pesquisaRegistro(int offset, FILE *df);
void imprimeRegistro(tipoAluno *registro);
int removeIndex(tipoChave numUSP, tipoPrimaryIndex *indice, int n_registros);
void finalizaExecucao(FILE *file, FILE *primaryIndexFile,FILE *secondaryIndexFile, FILE *invertedListFile , tipoPrimaryIndex *primaryIndexArray,tipoSecondaryIndex *secondaryIndexArray,tipoInvertedList *invertedListArray, int n_registros);
void removeRegistro(FILE *df, int offset);
#endif