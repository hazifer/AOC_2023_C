#include <stdio.h>
#include <string.h>
#include <ctype.h>

char is_possible_game(char *line, int red, int green, int blue);
int max_red_from_line(char *line);
int max_green_from_line(char *line);
int max_blue_from_line(char *line);

int main() 
{
	char line[1000];	/* line storage */
	int i = 0, sum = 0;	/* index of game, sum of possible indexes of games */
	int red = 12;		/* max for red */
	int green = 13;		/* max for green */
	int blue = 14;		/* max for blue */
	FILE *fin = fopen("games.txt", "r");
	if (!fin)
	{
		printf("error opening 'games.txt'.\n");
		return 1;
	}
	while (fgets(line, 1000, fin))
	{
		++i;
		if (is_possible_game(line, red, green, blue))
			sum += i;
	}
	printf("sum = %d\n", sum);
	fclose(fin);
	return 0;
}

char is_possible_game(char *line, int red, int green, int blue)
{
	/* assumes the input is in the correct format */
	printf("max red = %d ", max_red_from_line(line));
	printf("max green = %d ", max_green_from_line(line));
	printf("max blue = %d\n", max_blue_from_line(line));
	if (red < max_red_from_line(line))
		return 0;
	if (green < max_green_from_line(line))
		return 0;
	if (blue < max_blue_from_line(line))
		return 0;
	return 1;
}

int max_red_from_line(char *line)
{
	char *li, *old;
	int mult, cur_num;
	int max = 0;
	while (line && (li = strstr(line, "red"))) /* first time red shows up from line */
	{
		mult = 1, cur_num = 0;
		/* look for digit */
		old = li;
		--li;
		while (li >= line && *li == ' ')
			--li; /* skip blanks */
		while (li >= line && isdigit(*li)) 
		{
			/* reading digits right to left */
			cur_num += mult * (*li - '0');
			mult *= 10;
			--li;
		}
		if (max < cur_num)
			max = cur_num;
		line = strchr(old, ';');	/* point to next set */	
	}
	return max;
}

int max_green_from_line(char *line)
{
	char *li, *old;
	int mult, cur_num;
	int max = 0;
	while (line && (li = strstr(line, "green"))) /* first time green shows up from line */
	{
		mult = 1, cur_num = 0;
		/* look for digit */
		old = li;
		--li;
		while (li >= line && *li == ' ')
			--li; /* skip blanks */
		while (li >= line && isdigit(*li)) 
		{
			/* reading digits right to left */
			cur_num += mult * (*li - '0');
			mult *= 10;
			--li;
		}
		if (max < cur_num)
			max = cur_num;
		line = strchr(old, ';');	/* point to next set */	
	}
	return max;
}

int max_blue_from_line(char *line)
{
	char *li, *old;
	int mult, cur_num;
	int max = 0;
	while (line && (li = strstr(line, "blue"))) /* first time blue shows up from line */
	{
		mult = 1, cur_num = 0;
		/* look for digit */
		old = li;
		--li;
		while (li >= line && *li == ' ')
			--li; /* skip blanks */
		while (li >= line && isdigit(*li)) 
		{
			/* reading digits right to left */
			cur_num += mult * (*li - '0');
			mult *= 10;
			--li;
		}
		if (max < cur_num)
			max = cur_num;
		line = strchr(old, ';');	/* point to next set */	
	}
	return max;
}
