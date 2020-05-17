
#include "arquivo+indices.h"

int tamanhoDoArquivo(FILE *file)
{
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	return length;
}
FILE *criaArquivo(FILE *file, char *filename)
{
	do
	{
		file = fopen(filename, "r+");
		if (file == NULL)
		{
			file = fopen(filename, "w"); /* Se nao existir, cria o arquivo de dados */
			fclose(file);
			file = NULL;
		}
	} while (file == NULL);
	return file;
}

tipoAluno *leRegistro()
{
	tipoAluno *temp;
	temp = malloc(sizeof(tipoAluno));
	printf("Insira os dados\nNome: ");
	scanf(" %[^\n]s", temp->nome);
	printf("Sobrenome: ");
	scanf(" %[^\n]s", temp->sobrenome);
	printf("NUSP: ");
	scanf(" %d", &temp->numUSP);
	printf("Curso: ");
	scanf(" %[^\n]s", temp->curso);
	printf("Nota: ");
	scanf(" %f", &temp->nota);
	return temp;
}
void insereRegistro(FILE *file, tipoPrimaryIndex **primaryIndexArray, tipoSecondaryIndex **secondaryIndexArray, tipoInvertedList **invertedListArray, int *nRegisters, int *nSecondaryIndexRegisters)
{
	tipoAluno *temp;
	temp = leRegistro();
	int secondaryRegisterPosition;
	(*nRegisters)++;
	*primaryIndexArray = realloc(*primaryIndexArray, sizeof(tipoPrimaryIndex) * (*nRegisters));

	fseek(file, 0, SEEK_END);
	(*primaryIndexArray)[(*nRegisters) - 1].offset = ftell(file);
	(*primaryIndexArray)[(*nRegisters) - 1].numusp = temp->numUSP;
	fwrite(temp, sizeof(tipoAluno), 1, file);

	secondaryRegisterPosition = binarySearchSobrenome(*secondaryIndexArray, 0, *nSecondaryIndexRegisters, temp->sobrenome);
	if (-1 == secondaryRegisterPosition)
	{
		printf("\nNão existe o sobrenome");
		(*nSecondaryIndexRegisters)++;
		if (*nSecondaryIndexRegisters == 1) //verifica se o ponteiro ja tem espço alocado
		{
			*secondaryIndexArray = malloc(sizeof(tipoSecondaryIndex));
		}
		else
		{
			*secondaryIndexArray = realloc(*secondaryIndexArray, sizeof(tipoSecondaryIndex) * (*nSecondaryIndexRegisters));
		}
		memcpy((*secondaryIndexArray)[(*nSecondaryIndexRegisters) - 1].sobrenome, temp->sobrenome, TAM);
		(*secondaryIndexArray)[(*nSecondaryIndexRegisters) - 1].offset = ((*nRegisters)-1);
		
		if (*nRegisters == 1)
		{
			*invertedListArray = malloc(sizeof(tipoInvertedList));
		}
		else
		{
			*invertedListArray = realloc(*invertedListArray, sizeof(tipoInvertedList) * (*nRegisters));
		}
		(*invertedListArray)[(*nSecondaryIndexRegisters) - 1].numusp = temp->numUSP;
		(*invertedListArray)[(*nSecondaryIndexRegisters) - 1].next = -1;
		 
	}
	else
	{
		printf("Já existe esse sobrenome");
		
		int invertedRNN = (*secondaryIndexArray)[secondaryRegisterPosition].offset;
		printf("\n1invertedOffset = %d",invertedRNN);
		while ((*invertedListArray)[invertedRNN].next != -1)
		{
			invertedRNN = (*invertedListArray)[invertedRNN].next;
		}
		
		(*invertedListArray)[invertedRNN].next = (*nRegisters) - 1;
		(*invertedListArray) = realloc(*invertedListArray, *nRegisters * sizeof(tipoInvertedList));
		(*invertedListArray)[(*nRegisters) - 1].next = -1;
		(*invertedListArray)[(*nRegisters) - 1].numusp = temp->numUSP;
	}

	free(temp);
}

tipoAluno *pesquisaRegistro(int offset, FILE *file)
{
	tipoAluno *aluno;
	aluno = malloc(sizeof(tipoAluno));
	fseek(file, offset, SEEK_SET);
	fread(aluno, sizeof(tipoAluno), 1, file);
	return aluno;
}
int removeIndex(tipoChave numUSP, tipoPrimaryIndex *indice, int n_registros)
{
	/* neste método devem sem implementadas as operações de remoção de um RRN no índice, que deve ser mantido ordenado */
	int pos, offset;
	pos = binarySearchNumUsp(indice, 0, n_registros - 1, numUSP);
	offset = indice[pos].offset;
	memcpy(&indice[pos], &indice[pos + 1], n_registros - pos);
	indice = realloc(indice, n_registros - 1);
	return offset;
}

/* demais metodos a serem implementados */

void removeRegistro(FILE *file, int offset)
{
	/* pode ser implementada apenas uma remoção lógica */
	fseek(file, offset, SEEK_SET);
	fwrite(DEL, sizeof(char), 1, file);
	printf("Registro removido");
}

void finalizaExecucao(FILE *file, FILE *primaryIndexFile, FILE *secondaryIndexFile, FILE *invertedListFile, tipoPrimaryIndex *primaryIndexArray,
					  tipoSecondaryIndex *secondaryIndexArray, tipoInvertedList *invertedListArray, int n_registros)
{
	/* Arquivo de dados deve ser fechado com a finalizacao do programa */
	if (primaryIndexArray)
	{
		fseek(primaryIndexFile, 0, SEEK_SET);
		fwrite(primaryIndexArray, sizeof(tipoPrimaryIndex), n_registros, primaryIndexFile);
		free(primaryIndexArray);
	}

	if (secondaryIndexArray)
	{
		fseek(secondaryIndexFile, 0, SEEK_SET);
		fwrite(secondaryIndexArray, sizeof(tipoSecondaryIndex), n_registros, secondaryIndexFile);
		free(secondaryIndexArray);
	}

	if (invertedListArray)
	{
		fseek(invertedListFile, 0, SEEK_SET);
		fwrite(invertedListArray, sizeof(tipoInvertedList), n_registros, invertedListFile);
		free(invertedListArray);
	}

	fclose(file);
	fclose(primaryIndexFile);
	fclose(secondaryIndexFile);
	fclose(invertedListFile);
}
void swap(tipoPrimaryIndex *xp, tipoPrimaryIndex *yp)
{
	tipoPrimaryIndex temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubbleSort(tipoPrimaryIndex arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)

		for (j = 0; j < n - i - 1; j++)
			if (arr[j].numusp > arr[j + 1].numusp)
				swap(&arr[j], &arr[j + 1]);
}
void imprimeRegistro(tipoAluno *registro)
{

	printf("Nome :%s\n", (*registro).nome);
	printf("Sobrenome : %s\n", (*registro).sobrenome);
	printf("Numero USP: %d\n", (*registro).numUSP);
	printf("Curso: %s\n", (*registro).curso);
	printf("Nota: %0.2f\n", (*registro).nota);
}
int binarySearchNumUsp(tipoPrimaryIndex arr[], int l, int r, int numUsp)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		if (arr[mid].numusp == numUsp)
			return mid;

		if (arr[mid].numusp > numUsp)
			return binarySearchNumUsp(arr, l, mid - 1, numUsp);

		return binarySearchNumUsp(arr, mid + 1, r, numUsp);
	}

	return -1;
}
int binarySearchSobrenome(tipoSecondaryIndex arr[], int l, int r, char *sobrenome)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		if (!strcmp(arr[mid].sobrenome, sobrenome))
			return mid;

		if (0 < strcmp(arr[mid].sobrenome, sobrenome))
			return binarySearchSobrenome(arr, l, mid - 1, sobrenome);

		return binarySearchSobrenome(arr, mid + 1, r, sobrenome);
	}

	return -1;
}
void *carregaIndice(FILE *file, int typesize, int *nRegisters)
{
	void *index;
	int fileLength = tamanhoDoArquivo(file);
	if (fileLength)
	{
		index = malloc(fileLength);
		fread(index, typesize, fileLength / typesize, file);
		*nRegisters = fileLength / typesize;
	}

	return index;
}
// corrupted unsorted chunks se tentar adicionar muitos registros na primeira execução