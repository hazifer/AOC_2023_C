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

Map *set_seeds(char **line, long unsigned *arr_len);
void print_arr(Map *arr, long unsigned arr_len);
void map_seeds(FILE *fp, Map *arr, int arr_len);
void map_seeds_from_line(FILE *fp, Map *arr, int arr_len, char *line_start);
void read_numbers(char *line, long unsigned *first, long unsigned *second, long unsigned *third);
long unsigned get_minimum(Map *arr, int arr_len);

int main() 
{
	char line[MAX_LINE_LENGTH + 1], *ptr;
	Map *seed_arr;
	long unsigned min_location_mapping, curr_min;
	long unsigned seeds_num = 0;
	FILE *fp = fopen("almanac.txt", "r");
	if (!fp)
		return 1;
	while (fgets(line, MAX_LINE_LENGTH, fp) && ((ptr = strstr(line, "seeds: ")) == NULL))
			;
	if (!ptr || !fp)
		return 1; /* failed */
	ptr += strlen("seeds: ");
	printf("ptr is %s\n", ptr);
	seed_arr = set_seeds(&ptr, &seeds_num);
	printf("seeds number = %lu\n", seeds_num);
	printf("seeds as arr:\n");
	if (!fp)
		fp = fopen("alamanac.txt", "r");
	map_seeds(fp, seed_arr, seeds_num);
	fclose(fp);
	min_location_mapping = get_minimum(seed_arr, seeds_num);

	free(seed_arr);
	while (*ptr)
	{

		printf("ptr is %s\n", ptr);
		seed_arr = set_seeds(&ptr, &seeds_num);
		printf("seeds number = %lu\n", seeds_num);

		if (!fp)
			fp = fopen("alamanac.txt", "r");
		map_seeds(fp, seed_arr, seeds_num);
		fclose(fp);
		curr_min = get_minimum(seed_arr, seeds_num);
		min_location_mapping = min_location_mapping < curr_min ? min_location_mapping : curr_min;
		free(seed_arr);
	}
	printf("\nminimum location number is %lu\n", min_location_mapping);
	return 0;
}

void print_arr(Map *arr, long unsigned arr_len)
{
	if (!arr)
		return;
	int i;
	for (i = 0; i < arr_len; ++i)
		printf("arr[%d]: val = %lu map = %lu\n", i, arr[i].val, arr[i].mapped_val);
}

Map *set_seeds(char **line, long unsigned *arr_len)
{
	Map *arr;
	unsigned long start_range, range, i;
	start_range = range = 0;

	while (**line && (**line == ' ' || **line == '\t' || **line == '\n'))
		++(*line);
	if (!isdigit(**line))
	{
		printf("Not an integer\n");
		return NULL;
	}
	while (isdigit(**line))
	{
		start_range = start_range * 10 + **line - '0';
		++(*line);
	}

	while (**line && (**line == ' ' || **line == '\t' || **line == '\n'))
		++(*line);
	if (!isdigit(**line))
	{
		printf("Not an integer\n");
		return NULL;
	}
	while (isdigit(**line))
	{
		range = range * 10 + **line - '0';
		++(*line);
	}

	while (**line && (**line == ' ' || **line == '\t' || **line == '\n'))
		++(*line);

	printf("start %lu range %lu\n", start_range, range);
	arr = (Map *)malloc(sizeof(Map) * range);
	for (i = 0; i < range; ++i) /* set from end of array, x = range cells */
		arr[i].val = arr[i].mapped_val  = start_range + i;

	*arr_len = range;
	return arr;
}

void map_seeds(FILE *fp, Map *arr, int arr_len)
{
	printf("\nmap seed to soil\n");
	map_seeds_from_line(fp, arr, arr_len, "seed-to-soil map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap soil to fertilizer\n");
	map_seeds_from_line(fp, arr, arr_len, "soil-to-fertilizer map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap fertilizer to water\n");
	map_seeds_from_line(fp, arr, arr_len, "fertilizer-to-water map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap water to light\n");
	map_seeds_from_line(fp, arr, arr_len, "water-to-light map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap light to termperature\n");
	map_seeds_from_line(fp, arr, arr_len, "light-to-temperature map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap termperature to humidity\n");
	map_seeds_from_line(fp, arr, arr_len, "temperature-to-humidity map:");
	if (!fp)
		return; /* failed, we expect more lines*/
	printf("\nmap humidity to location\n");
	map_seeds_from_line(fp, arr, arr_len, "humidity-to-location map:");
	if (!fp)
		return; /* failed, we expect more lines*/
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
