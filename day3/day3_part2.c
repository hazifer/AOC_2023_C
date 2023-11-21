#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned long get_sum_from_line(char *above, char *line, char *below, int len);
unsigned long get_mult_from_adjacent_line(char *line, int start, int end, int *count);
unsigned long gear_value(char *above, char *line, char *below, char *li);
unsigned long read_num_forwards(char *start);
unsigned long read_num_backwards(char *line, char *start);

int main()
{
	char above[1000], line[1000], below[1000];
	FILE *fpin = fopen("engine_schematic.txt", "r");
	unsigned long sum = 0;
	int len = 0;
	if (!fpin)
	{
		printf("error opening file\n");
		return 1;
	}
	len = strlen(line);	/* assuming all lines are of similar length */
	fgets(line, 1000, fpin);
	while (fgets(below, 1000, fpin))
	{
		sum += get_sum_from_line(above, line, below, len);
		strcpy(above, line);
		strcpy(line, below);
	}
	sum += get_sum_from_line(above, line, NULL, len);	/* the last call is made for summing the last line
								   using the line above it as the symbol line */
	printf("sum = %lu\n", sum);
	return 0;
}

unsigned long get_sum_from_line(char *above, char *line, char *below, int len)
{
	char *li;
	long unsigned sum = 0, total;
	li = line;
	while (*li)
	{
		if (*li == '*')
		{
			total = gear_value(above, line, below, li);
			sum += total;
		}
		++li;
	}
	return sum;
}

unsigned long gear_value(char *above, char *line, char *below, char *li)
{
	/* assumes li is pointing to '*' in an arbitrary line */
	unsigned long total = 0, line_mult, next_num;
	int start, end, count = 0;
	/* check if to the left is a num or to the right is a num */
	if (li > line) /* reading to the left is possible */
	{
		total = read_num_backwards(line, li - 1);
		if (total)
			++count;
		start = li - line - 1;
	}
	else
		start = li - line;
	if (*(li + 1)) /* reading to the right is possible */
	{
		next_num = read_num_forwards(li + 1);
		if (next_num)
		{
			if (count)
				total *= next_num;
			else
				total = next_num;
			++count;
		}
		end = li - line + 1;
	}
	else
		end = li - line;
	/* multi into num eligible numbers from adjacent lines */
	if (above)
	{
		if (count)
		{
			/* get_mult_from_adjacent_line affects count, so we check prior to the call */
			line_mult = get_mult_from_adjacent_line(above, start, end, &count);
			if (line_mult)
				total *= line_mult;
		}
		else
		{
			/* count was 0 */
			line_mult = get_mult_from_adjacent_line(above, start, end, &count);
			if (line_mult)
				total = line_mult;
		}
	}
	if (below)
	{
		if (count)
		{
			/* get_mult_from_adjacent_line affects count, so we check prior to the call */
			line_mult = get_mult_from_adjacent_line(below, start, end, &count);
			if (line_mult)
				total *= line_mult;
		}
		else
		{
			/* count was 0 
			 * we can check for count == 2 instead and;
			 * return 0 if it isn't 
			 * return line_mult if it is 
			 * BUT it seems less readable to suddenly change the logic */
			line_mult = get_mult_from_adjacent_line(below, start, end, &count);
			if (line_mult)
				total = line_mult;
		}
	}
	if (count == 2)
		return total;
	return 0;
}

unsigned long read_num_backwards(char *line, char *start)
{
	/* point start to beginning of digit sequence */
	if (!isdigit(*start))
		return 0;
	while (start > line && isdigit(*(start - 1)))
		--start;
	return read_num_forwards(start);
}

unsigned long read_num_forwards(char *start)
{
	int num = 0;
	while (*start && isdigit(*start))
	{
		num = num * 10 + *start - '0';
		++start;
	}
	return num;
}

unsigned long get_mult_from_adjacent_line(char *line, int start, int end, int *count)
{
	int num1, num2;
	num1 = read_num_backwards(line, line + start);
	num2 = read_num_backwards(line, line + end);
	if (num1 && !num2)
	{
		++(*count);
		return num1;
	}
	if (num2 && !num1)
	{
		++(*count);
		return num2;
	}
	if (!num1 && !num2)
	{
		/* should check case of .X. when X is a digit between start and end */
		if (isdigit(*(line + start + 1)))
		{
			++(*count);
			return *(line + start + 1) - '0';
		}
		return 0;
	}
	/* both num1 and num2 have numbers in them */
	if (start + 1 == end)
	{
		/* start and end are 1 digit apart, so a number read into num1 is the same as the one read into num2 */
		++(*count);
		return num1;
	}
	/* if we are here, then end - start == 2 ! */
	if (isdigit(*(line + start + 1)))
	{
		/* line[start] line[start + 1] and line[end] (end is start + 2) are digits, same case as above */
		++(*count);
		return num1;
	}
	*count += 2;
	return num1 * num2;
}
