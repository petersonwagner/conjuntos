#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQUAL 0
#define DIFF 1

int main(int argc, char *argv[])
{
	FILE *arq1, *arq2, *log_file;
	char caractere1, caractere2;
	int result = 0;
	int linha = 0, coluna = 0;

	arq1 = fopen (argv[1], "r");
	arq2 = fopen (argv[2], "r");
	log_file = fopen ("log_file.txt", "a");

	if (arq1 == NULL)
	{
		printf("ERRO AO LER '%s'", argv[1]);
		abort();
	}

	if (arq2 == NULL)
	{
		printf("ERRO AO LER '%s'", argv[2]);
		abort();
	}

	do
	{
		caractere1 = fgetc(arq1);
		caractere2 = fgetc(arq2);

		if (caractere1 != caractere2)
		{
			result = DIFF;
			break;
		}
		else if (caractere1 == '\n')
		{
			linha++;
			coluna = 0;
		}
		else
			coluna++;
	} while (!feof(arq1) && !feof(arq2));


	if (result == EQUAL)
	{
		printf("Iguais: %s - %s\n", argv[1], argv[2]);
		fprintf(log_file, "Iguais: %s - %s\n", argv[1], argv[2]);
	}
	else
	{
		printf("Diferentes: %s - %s\n", argv[1], argv[2]);
		printf("linha: %d\ncoluna: %d\n", linha, coluna);
		fprintf(log_file, "Diferentes: %s - %s\n", argv[1], argv[2]);
		fprintf(log_file, "linha: %d\ncoluna: %d\n", linha, coluna);
	}


	return 0;
}