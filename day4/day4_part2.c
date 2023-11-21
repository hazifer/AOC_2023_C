#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int get_matches_from_line(char *, int *, int *, int, int);
int count_guess_cards(char *);
int count_winning_cards(char *);
char * read_word(char *, char *);
void load_cards(char *, int *, int);
void print_arr(int *, int);
int count_similar(int *, int *, int, int);
int count_lines(FILE *fp);
void increment_array_cells(int *, int, int);
int sum_array(int *, int);

int main()
{
	char line[1000];
	int *card_count, line_number = 0;
	int matches_in_line, guess_cards_num, winning_cards_num, line_count, current_card_count;
	int *guess_cards, *winning_cards;
	FILE *fp = fopen("scratchcards.txt", "r");
	if (!fp)
	{
		printf("couldn't open file\n");
		return 1;
	}
	line_count = count_lines(fp);
	fseek(fp, 0, SEEK_SET);
	fgets(line, 1000, fp);
	guess_cards_num = count_guess_cards(line);
	winning_cards_num = count_winning_cards(line);
	guess_cards = (int *)malloc(guess_cards_num * sizeof(int));
	winning_cards = (int *)malloc(winning_cards_num * sizeof(int));
	card_count = (int *)calloc(line_count, sizeof(int));
	increment_array_cells(card_count, line_count, line_count);
	fseek(fp, 0, SEEK_SET);
	while (fgets(line, 1000, fp))
	{
		matches_in_line = get_matches_from_line(line, guess_cards, winning_cards, guess_cards_num, winning_cards_num);
		current_card_count = card_count[line_number];
		while (current_card_count)
		{
			increment_array_cells(card_count + line_number + 1, matches_in_line, line_count);
			current_card_count--;
		}
		line_number++;
	}
	printf("total cards: %d\n", sum_array(card_count, line_count));
	return 0;
}

int get_matches_from_line(char *line, int *guess_cards, int *winning_cards, int guess_cards_len, int winning_cards_len)
{
	int similar_count;
	char *tmp;
	line = strchr(line, ':');
	if (!line)
		return 0;
	++line;
	tmp = strchr(line, '|');
	*tmp = '\0';
	load_cards(line, guess_cards, guess_cards_len);
	*tmp = '|';
	line = tmp + 1;
	tmp = strchr(line, '\n');
	*tmp = '\0';
	load_cards(line, winning_cards, winning_cards_len);
	*tmp = '\n';
	similar_count = count_similar(guess_cards, winning_cards, guess_cards_len, winning_cards_len);
	return similar_count;
}

int count_guess_cards(char *line)
{
	char *end, word[100];
	int count = 0;
	line = strchr(line, ':');
	end = strchr(line, '|');
	++line;
	*end = '\0';
	while ((line = read_word(line, word)))
		++count;
	*end = '|';
	return count;
}

int count_winning_cards(char *line)
{
	char *end, word[100];
	int count = 0;
	line = strchr(line, '|');
	end = strchr(line, '\n');
	++line;
	*end = '\0';
	while ((line = read_word(line, word)))
		++count;
	*end = '\n';
	return count;
}

char * read_word(char *line, char *word)
{
	int nc = 0;
	*word = '\0';
	while (*line && (*line == ' ' || *line == '\t'))
		++line;
	while ((word[nc] = *line) && (*line != ' ' && *line != '\t' && *line != '\n'))
		++nc, ++line;
	word[nc] = '\0';
	if (nc == 0)
		return NULL;
	return line;	
}

void load_cards(char *line, int *card_array, int arr_len)
{
	int i;
	char word[100];
	for (i = 0; i < arr_len && (line = read_word(line, word)); ++i)
		card_array[i] = atoi(word);
}

void print_arr(int *arr, int len)
{
	int i;
	for (i = 0; i < len; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}

int count_similar(int *arr1, int *arr2, int len1, int len2)
{
	int i, j, count = 0;
	for (i = 0; i < len1; ++i)
		for (j = 0; j < len2; ++j)
			if (arr1[i] == arr2[j])
				++count;
	return count;
}

int count_lines(FILE *fp)
{
	char line[1000];
	int line_count = 0;
	while (fgets(line, 1000, fp))
		++line_count;
	return line_count;
}

void increment_array_cells(int *arr, int cells_from_arr, int arr_len)
{
	int i;
	for (i = 0; i < cells_from_arr && i < arr_len; i++)
		arr[i]++;
}

int sum_array(int *arr, int len)
{
	int i, sum = 0;
	for (i = 0; i < len; i++)
		sum += arr[i];
	return sum;
}
