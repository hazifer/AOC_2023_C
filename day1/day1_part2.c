#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *string_digits[9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int get_first_digit(char *);
int get_last_digit(char *);

int main() 
{
	int sum, fd, ld;	/* sum, first digit and last digit */
	FILE *fp;
	char line[1000]; 	/* line input */
	fp = fopen("calibration.txt", "r");
	sum = 0;
	if (fp)
	{
		while (fgets(line, 1000, fp))
		{
			fd = get_first_digit(line);
			ld = get_last_digit(line);
			sum += 10 * fd + ld;
		}
		printf("sum = %d\n", sum);
	}
	else
		printf("Error opening file\n");
	fclose(fp);
	return 0;
}

int get_first_digit(char *line) 
{
	int dig;	/* will store the found first digit */
	char *digp;	/* will store the pointer for the first digit */
	int i;
	char *li;
	/* look for "regular" digits */
	digp = line;
	while (*digp && !isdigit(*digp))
		++digp;
	if (digp)
		dig = *digp - '0';
	/* look for "string" digits */
	for (i = 0; i < 9; ++i)
	{
		li = strstr(line, string_digits[i]);
		if (li)
		{
			if (li < digp)
			{
				/* found a "string" digit prior to the first "regular"/"string" digit found */
				digp = li;
				dig = i + 1;
			}
		}
	}
	if (digp)
		return dig;
	return 0;
}
int get_last_digit(char *line)
{
	int dig;	/* will store the found first digit */
	char *digp;	/* will store the pointer for the first digit */
	int i;
	char *li;
	digp = line + strlen(line) - 1;	/* -1 for '\0' */
	/* look for "regular" digits */
	while (digp >= line && !isdigit(*digp))
		--digp;
	if (digp >= line)
		dig = *digp - '0';
	/* look for "string" digits */
	li = line + strlen(line) - 1 - 3; /* -1 for '\0', -3 for the length of the smallest "string digit" */
	while (li >= line && li > digp)
	{
		for (i = 0; i < 9; ++i)
		{
			
			if (strstr(li, string_digits[i]))
			{
				digp = li;
				dig = i + 1;
			}
		}
		--li;
	}
	if (digp)
		return dig;
	return 0;
}
