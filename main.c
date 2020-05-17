#include "arquivo+indices.h"

int main(void)
{
	printf("Bem vindo ao sistema de gerenciamento de notas");
	FILE *file = NULL, *primaryIndexFile = NULL, *secondaryIndexFile = NULL, *invertedListFile = NULL;
	tipoPrimaryIndex *primaryIndexArray = NULL;
	tipoSecondaryIndex *secondaryIndexArray = NULL;
	tipoInvertedList *invertedListArray = NULL;
	int selection, nRegisters = 0, nSecondaryIndexRegisters = 0, tempNumUsp;

	file = criaArquivo(file, "data/alunos.dad");
	primaryIndexFile = criaArquivo(primaryIndexFile, "data/primaryIndex.dad");
	secondaryIndexFile = criaArquivo(secondaryIndexFile, "data/secondaryIndex.dad");
	invertedListFile = criaArquivo(invertedListFile, "data/InvertedList.dad");

	invertedListArray = (tipoInvertedList *)carregaIndice(invertedListFile, sizeof(tipoInvertedList), &nRegisters);
	primaryIndexArray = (tipoPrimaryIndex *)carregaIndice(primaryIndexFile, sizeof(tipoPrimaryIndex), &nRegisters);
	secondaryIndexArray = (tipoSecondaryIndex *)carregaIndice(secondaryIndexFile, sizeof(tipoSecondaryIndex), &nSecondaryIndexRegisters);

	do
	{
		printf("\nSelecione uma Operacao:\n1 - Gravar\n2 - Pesquisar\n3 - Deletar\n0 - Finalizar: \n\n");
		scanf("%d", &selection);

		if (selection == 1) //Gravar
		{
			insereRegistro(file, &primaryIndexArray, &secondaryIndexArray, &invertedListArray, &nRegisters, &nSecondaryIndexRegisters);
		}
		if (selection == 2) //Pesquisar
		{
			int searchType;
			printf("Deseja buscar por 1-NUSP ou 2-Sobrenome");
			scanf(" %d", &searchType);
			if (searchType == 1)
			{
				int registryIndex;
				tipoAluno *tempAluno;
				printf("\nDigite o numero USP: ");
				scanf(" %d", &tempNumUsp);
				bubbleSort(primaryIndexArray, nRegisters);
				registryIndex = binarySearchNumUsp(primaryIndexArray, 0, nRegisters - 1, tempNumUsp);
				if (registryIndex != -1)
				{
					tempAluno = pesquisaRegistro(primaryIndexArray[registryIndex].offset, file);
					imprimeRegistro(tempAluno);
					free(tempAluno);
				}
				else
					printf("indice não encontrado");
			}
			else if (searchType == 2)
			{
				int secondaryRegistryIndex;
				char sobrenome[TAM];
				tipoAluno *tempAluno;
				printf("\nDigite o sobrenome:");
				scanf(" %s", sobrenome);
				secondaryRegistryIndex = binarySearchSobrenome(secondaryIndexArray, 0, nSecondaryIndexRegisters - 1, sobrenome);
				if (secondaryRegistryIndex == -1)
					printf("\nregistro não encontrado");
				else
				{
					while (secondaryRegistryIndex != -1)
					{
						tempAluno = pesquisaRegistro(primaryIndexArray[binarySearchNumUsp(primaryIndexArray, 0, nRegisters - 1, invertedListArray[secondaryRegistryIndex].numusp)].offset, file);
						imprimeRegistro(tempAluno);
						printf("\n");
						secondaryRegistryIndex = invertedListArray[secondaryRegistryIndex].next;
						if (invertedListArray[secondaryRegistryIndex].next == -1)
						{
							free(tempAluno);
						}
					}
				}
			}
		}
		if (selection == 3) //Remover
		{
			printf("Digite o numero USP: ");
			scanf(" %d", &tempNumUsp);
			if (binarySearchNumUsp(primaryIndexArray, 0, nRegisters - 1, tempNumUsp) != -1)
			{
				int posRemocao =  removeIndex(tempNumUsp, primaryIndexArray, nRegisters);
				removeRegistro(file,posRemocao );
				nRegisters--;
				//removeSecondaryIndex();
			}
			else
				printf("Registro não encontrado");
		}


	} while (selection > 0);

	finalizaExecucao(file, primaryIndexFile, secondaryIndexFile, invertedListFile, primaryIndexArray, secondaryIndexArray, invertedListArray, nRegisters);
	return 0;
}
