#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *get_first_digit(char *);
char *get_last_digit(char *);

int main() 
{
	int sum;
	FILE *fp;
	char line[1000], fd, ld; /* line input, first digit and last digit */
	fp = fopen("calibration.txt", "r");
	sum = 0;
	if (fp)
	{
		while (fgets(line, 1000, fp))
		{
			fd = *get_first_digit(line);
			ld = *get_last_digit(line);
			if (isdigit(fd) && isdigit(ld)) /* just incase */
				sum += 10 * (fd - '0') + (ld - '0');
		}
		printf("sum = %d\n", sum);
	}
	else
		printf("Error opening file\n");
	fclose(fp);
	return 0;
}

char *get_first_digit(char *line) 
{
	while (*line && !isdigit(*line))
		++line;
	return line;
}
char *get_last_digit(char *line)
{
	int len;
	char *i;
	len = strlen(line);
	i = line + len - 1;
	while (line != i && !isdigit(*i))
		--i;
	return i;

}
