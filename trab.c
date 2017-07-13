#include <stdio.h>
#include <stdlib.h>

#define true  1
#define false 0

typedef struct node_t
{
	int *array;
	int size;
	struct node_t *next;
} node_t;

typedef struct list_t
{
	int size;
	node_t *head;
} list_t;

int array_compare (node_t *cell1, node_t *cell2);
node_t* search_position_to_insert (list_t *list, node_t *key);
node_t* search_position_to_del (list_t *list, node_t *key);
void delete_set (node_t *cell);
void remove_set (list_t *list);
void insert_set (list_t *list);
int search_set (list_t *list);
void print_set (node_t *cell);
void print_list (list_t *list);
void delete_proper_supersets (list_t *list);
void list_proper_supersets (list_t *list);
void list_proper_subsets (list_t *list);

int main()
{
	list_t list;
	int maxVal;
	char operation;

	list.head = (node_t*) malloc (sizeof(node_t));
	list.head->next = NULL;
	list.size = 0;

	scanf("%d", &maxVal);

	do
	{
		scanf("%c", &operation);
		switch (operation)
		{
			case '+':
				insert_set (&list);
				break;
			
			case '-':
			    remove_set (&list);
				break;

			case '=':
			    search_set (&list);
				break;

			case '*':
			    print_list (&list);
				break;

			case '/':
				delete_proper_supersets (&list);
				break;

			case '<':
				list_proper_subsets (&list);
				break;

			case '>':
				list_proper_supersets (&list);
				break;

		}
	} while (operation != '0');

	return 0;
}

int is_proper_superset (node_t *set1, node_t *set2) //is set1 proper superset of set2
{
	int i, j;

	if (set1->size <= set2->size)
		return false;

	for (i = 0, j = 0; (i < set1->size) && (j < set2->size); ++i)
	{
		if (set1->array[i] > set2->array[j])
			return false;

		else if (set1->array[i] == set2->array[j])
			j++;
	}

	if (j == set2->size)
		return true;
	else
		return false;
}

void delete_proper_supersets (list_t *list)
{
	int success = false;
	node_t set;
	node_t *cell;

	scanf("%d", &set.size);
	set.array = (int*) malloc (sizeof(int) * set.size);

	for (int i = 0; i < set.size; ++i)
		scanf("%d", &set.array[i]);

	for (cell = list->head; (cell != NULL) && (cell->next != NULL); cell = cell->next)
	{
		if (is_proper_superset(cell->next, &set))
		{
			delete_set (cell);
			success = true;
		}
	}

	if (success == false)
		printf("E\n");

	printf("0\n");

	free (set.array);
}


void list_proper_supersets (list_t *list)
{
	node_t set;
	node_t *cell;

	scanf("%d", &set.size);
	set.array = (int*) malloc (sizeof(int) * set.size);

	for (int i = 0; i < set.size; ++i)
		scanf("%d", &set.array[i]);

	for (cell = list->head->next; cell != NULL; cell = cell->next)
		if (is_proper_superset(cell, &set))
			print_set (cell);

	printf("0\n");

	free (set.array);
}

void list_proper_subsets (list_t *list)
{
	node_t set;
	node_t *cell;

	scanf("%d", &set.size);
	set.array = (int*) malloc (sizeof(int) * set.size);

	for (int i = 0; i < set.size; ++i)
		scanf("%d", &set.array[i]);

	for (cell = list->head->next; cell != NULL; cell = cell->next)
		if (is_proper_superset(&set, cell))
			print_set (cell);

	printf("0\n");

	free (set.array);
}

int array_compare (node_t *cell1, node_t *cell2)
{
	int i;

	//o tamanho dos vetores também devem ser levados em conta para a ordenação
	if (cell1->size < cell2->size)
		return -1;
	else if (cell1->size > cell2->size)
		return 1;

	//faz comparacao de cada elemento de duas celulas da lista
	for (i = 0; i < cell1->size && i < cell2->size; ++i)
		if (cell1->array[i] < cell2->array[i])
			return -1;
		else if (cell1->array[i] > cell2->array[i])
			return 1;

	return 0;
}

//retorna a posição anterior a que o vetor procurado deveria estar
//se o vetor ja esta na lista, retorna NULL
node_t* search_position_to_insert (list_t *list, node_t *key)
{
	int ret;
	node_t *cell;

	//se a lista esta vazia, retorna a primeira posicao
	if (list->head->next == NULL)
		return list->head;

	//percorre todas as celulas da lista (ignorando a celula head, que é vazia)
	//para quando a PROXIMA celula é vazia
	for (cell = list->head; cell->next != NULL; cell = cell->next)
	{
		//compara o proximo vetor com o vetor procurado
		ret = array_compare (cell->next, key);

		if (ret > 0)
			return cell;
		else if (ret == 0)
			return NULL;
	}

	return cell;
}

//retorna a posição anterior a que o vetor procurado esta
//se nao encontrar, retornar NULL
node_t* search_position_to_del (list_t *list, node_t *key)
{
	node_t *cell;

	if (list->head->next == NULL)
		return list->head;

	for (cell = list->head; cell->next != NULL; cell = cell->next)
		if (array_compare (cell->next, key) == 0)
			return cell;

	return NULL;
}

void delete_set (node_t *cell)
{
	node_t *aux;

	aux = cell->next->next;
	free (cell->next);
	cell->next = aux;

	return;
}

void remove_set (list_t *list)
{
	int size;
	node_t set_to_del;
	node_t *position;

	scanf("%d", &size);
	set_to_del.size = size;
	set_to_del.array = (int*) malloc (sizeof(int) * size);

	for (int i = 0; i < size; ++i)
		scanf("%d", &(set_to_del.array[i]));

	position = search_position_to_del (list, &set_to_del);

	if (position != NULL)
	{
		if (position->next == NULL)
			printf("E\n");
		else
			delete_set (position);
	}
	else
	{
		printf("E\n");
	}

	printf("0\n");

	free (set_to_del.array);

	return;
}

void insert_set (list_t *list)
{
	int i, size;
	node_t *new_set = NULL;
	node_t *position;
	
	//leitura do novo conjunto
	scanf("%d", &size);

	new_set = (node_t*) malloc (sizeof(node_t));
	new_set->array = (int *) malloc (sizeof(int) * size);

	for (i = 0; i < size; ++i)
		scanf("%d", &new_set->array[i]);

	new_set->size = size;


	//insere novo nodo
	position = search_position_to_insert (list, new_set);
	if (position != NULL)
	{
		new_set->next = position->next;
		position->next = new_set;
		list->size++;
	}
	else
	{
		free (new_set->array);
		free (new_set);
		printf("E\n");
	}

	printf("0\n");

	return;
}

int search_set (list_t *list)
{
	node_t key;
	node_t *cell;

	scanf("%d", &key.size);
	key.array = (int*) malloc (sizeof(int) * key.size);

	for (int i = 0; i < key.size; ++i)
		scanf("%d", &key.array[i]);

	for (cell = list->head; cell != NULL; cell = cell->next)
		if (array_compare (cell, &key) == 0)
		{
			free (key.array);
			printf("0\n");
			return 1;
		}

	free (key.array);
	printf("E\n");
	printf("0\n");
	return 0;
}

void print_set (node_t *cell)
{
	int i;

	printf("%d ", cell->size);

	for (i = 0; i < cell->size - 1; ++i)
		printf("%d ", cell->array[i]);
	printf("%d", cell->array[i]);
	
	printf("\n");

	return;
}
void print_list (list_t *list)
{
	node_t *cell;

	for (cell = list->head->next; cell != NULL; cell = cell->next)
		print_set (cell);

	printf("0\n");
}