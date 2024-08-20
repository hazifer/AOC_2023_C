#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1000

typedef struct List {
	int val;
	struct List *next;
} List;

int *set_seeds(char *line, int num_of_seeds);
int count_seeds(char *line);
void print_arr(int *arr, int arr_len);
List *set_list(char *line);
void print_list(List *list);

int main() 
{
	FILE *fp = fopen("almanac-example.txt", "r");
	char line[MAX_LINE_LENGTH + 1], *seeds_ptr;
	List *seeds_list;
	if (!fp)
		return 1;
	while (fgets(line, MAX_LINE_LENGTH, fp) && ((seeds_ptr = strstr(line, "seeds: ")) == NULL))
			; /* get seeds ptr into seeds_ptr */
	if (!seeds_ptr)
		return 1; /* failed */

	seeds_ptr += strlen("seeds: ");
	printf("seeds as list:\n");
	seeds_list = set_list(seeds_ptr);
	print_list(seeds_list);



	free(seeds_list);
	return 0;
}

List *set_list(char *line)
{
	List *head, *ptr;
	while (*line == ' ' || *line == '\t')
		++line;
	if (isdigit(*line))
		head = (List *)malloc(sizeof(List));
	else
	{
		printf("Not an integer\n");
		return NULL;
	}

	head->val = 0;
	while (isdigit(*line))
		head->val = (head->val) * 10 + *line++ - '0';

	ptr = head;
	while (*line)
	{
		while (*line == ' ' || *line == '\t' || *line == '\n')
			++line;
		if (isdigit(*line))
			ptr->next = (List *)malloc(sizeof(List));
		else
		{
			printf("Not an integer\n");
			return NULL;
		}

		ptr = ptr->next;
		ptr->val = 0;
		while (isdigit(*line))
			ptr->val = (ptr->val) * 10 + *line++ - '0';
		++line;
	}
	return head;
}

void print_list(List *list)
{
	int i = 0;
	while (list)
	{
		printf("val %d = %d\n", ++i, list->val);
		list = list->next;
	}
}
