#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH 1000

typedef struct List {
	int val;
	struct List *next;
} List;

typedef struct Map {
	long val;
	long mapped_val;
	char is_mapped;
} Map;

Map *set_seeds(char *line, int num_of_seeds);
int count_seeds(char *line);
void print_arr(Map *arr, int arr_len);
void map_seeds(FILE *fp, Map *arr, int arr_len);
void map_seeds_from_line(FILE *fp, Map *arr, int arr_len, char *line_start);
void read_numbers(char *line, long unsigned *first, long unsigned *second, long unsigned *third);
long unsigned get_minimum(Map *arr, int arr_len);

int main() 
{
	char line[MAX_LINE_LENGTH + 1], *ptr;
	Map *seed_arr;
	int seeds_num;
	long unsigned min_location_mapping;
	FILE *fp = fopen("almanac.txt", "r");
	if (!fp)
		return 1;
	while (fgets(line, MAX_LINE_LENGTH, fp) && ((ptr = strstr(line, "seeds: ")) == NULL))
			; /* get seeds ptr into seeds_ptr */
	if (!ptr || !fp)
		return 1; /* failed */
	ptr += strlen("seeds: ");
	seeds_num = count_seeds(ptr);
	printf("seeds num: %d\n", seeds_num);
	seed_arr = set_seeds(ptr, seeds_num);
	printf("seeds as arr:\n");
	print_arr(seed_arr, seeds_num);
	map_seeds(fp, seed_arr, seeds_num);
	min_location_mapping = get_minimum(seed_arr, seeds_num);
	printf("minimum location number is %lu\n", min_location_mapping);
	free(seed_arr);
	return 0;
}

void print_arr(Map *arr, int arr_len)
{
	int i;
	for (i = 0; i < arr_len; ++i)
		printf("arr[%d]: val = %lu map = %lu\n", i, arr[i].val, arr[i].mapped_val);
}

int count_seeds(char *line)
{
	int i = 0;
	while (*line)
	{
		while (*line == ' ' || *line == '\t' || *line == '\t')
			++line;
		if (isdigit(*line))
			++i;
		while (isdigit(*line++))
			;
	}	
	return i;
}

Map *set_seeds(char *line, int num_of_seeds)
{
	Map *arr;
	int i;
	arr = (Map *)calloc(num_of_seeds, sizeof(Map));
	i = 0;
	while (*line)
	{
		while (*line == ' ' || *line == '\t' || *line == '\t')
			++line;
		while (isdigit(*line))
			arr[i].val = (arr[i].val) * 10 + *line++ - '0';
		arr[i].mapped_val = arr[i].val; /* default to map value to the same value */
		++line, ++i;
	}	
	return arr;
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

void map_seeds(FILE *fp, Map *arr, int arr_len)
{
	printf("\nmap seed to soil\n");
	map_seeds_from_line(fp, arr, arr_len, "seed-to-soil map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap soil to fertilizer\n");
	map_seeds_from_line(fp, arr, arr_len, "soil-to-fertilizer map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap fertilizer to water\n");
	map_seeds_from_line(fp, arr, arr_len, "fertilizer-to-water map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap water to light\n");
	map_seeds_from_line(fp, arr, arr_len, "water-to-light map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap light to termperature\n");
	map_seeds_from_line(fp, arr, arr_len, "light-to-temperature map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap termperature to humidity\n");
	map_seeds_from_line(fp, arr, arr_len, "temperature-to-humidity map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
	printf("\nmap humidity to location\n");
	map_seeds_from_line(fp, arr, arr_len, "humidity-to-location map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	print_arr(arr, arr_len);
}

void map_seeds_from_line(FILE *fp, Map *arr, int arr_len, char *line_search)
{
	long unsigned dst_start, src_start, range;
	int i;
	char line[MAX_LINE_LENGTH], *line_ptr;
	while (fgets(line, MAX_LINE_LENGTH, fp) && (line_ptr = strstr(line, line_search)) == NULL)
		;
	if (!line_ptr)
		return; /* failed */
	while (fgets(line, MAX_LINE_LENGTH, fp))
	{
		line_ptr = line;
		read_numbers(line_ptr, &dst_start, &src_start, &range);
		printf("dst %lu src %lu range %lu\n", dst_start, src_start, range);
		if (!dst_start && !src_start && !range)
		{
			for (i = 0; i < arr_len; ++i)
				arr[i].is_mapped = 0;
			return;
		}
		for (i = 0; i < arr_len; ++i)
			if (!arr[i].is_mapped && arr[i].mapped_val >= src_start && arr[i].mapped_val < src_start + range)
			{
				arr[i].mapped_val += dst_start - src_start;
				arr[i].is_mapped = 1;
			}
	}
}

void read_numbers(char *line, long unsigned *first, long unsigned *second, long unsigned *third)
{
	long unsigned num;
	while (*line == ' ' || *line == '\t')
		++line;
	if (*line  == '\n' || *line == '\0')
		*first = *second = *third = 0;
	if (!isdigit(*line))
		*first = *second = *third = 0;

	num = 0;
	while (isdigit(*line))
		num = num * 10 + *line++ - '0';
	*first = num;
	while (*line == ' ' || *line == '\t')
		++line;

	num = 0;
	while (isdigit(*line))
		num = num * 10 + *line++ - '0';
	*second = num;
	while (*line == ' ' || *line == '\t')
		++line;

	num = 0;
	while (isdigit(*line))
		num = num * 10 + *line++ - '0';
	*third = num;
}

long unsigned get_minimum(Map *arr, int arr_len)
{
	int i;
	long unsigned min = arr[0].mapped_val;
	for (i = 0; i < arr_len; ++i)
		if (min > arr[i].mapped_val)
			min = arr[i].mapped_val;
	return min;
}
