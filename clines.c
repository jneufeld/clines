#include <stdio.h>
#define BUFFER_SIZE 256

/*
 * Function prototypes
 */
void count_lines (const char *fname);
void print_lines (int total, int code, int comment, 
					int whitespace, const char *name);

/*
 * main
 */
int main (int argc, char **argv)
{
	if (argc == 1)
	{
		printf ("Usage: clines filename\n");
		return -1;
	}

	int i;
	for (i = 1; i < argc; i++)
	{
		if (i > 1)
		{
			printf ("\n");
		}
		count_lines (argv[i]);
	}
	return 0;
}

/*
 * count_lines
 */
void count_lines (const char *fname)
{
	FILE *f = fopen (fname, "r");
	if (!f)
	{
		printf ("Couldn't open file: %s\n", fname);
		return;
	}

	int total		= 0;
	int code		= 0;
	int comment		= 0;
	int whitespace	= 0;

	int buffer[BUFFER_SIZE], c = 0;
	while (c != EOF)
	{
		int i = 0;
		do {
			c = fgetc (f);
			buffer[i++] = c;
		} while (c != '\n' && c != EOF);
		total++;
	}

	print_lines (total, code, comment, whitespace, fname);
	fclose (f);
}

/*
 * print_lines
 */
void print_lines (int total, int code, int comment, 
					int whitespace, const char *name)
{
	printf ("%s\n", name);
	printf ("Total:   %d\n", total);
	printf ("Code:    %d\n", code);
	printf ("Comment: %d\n", comment);
	printf ("Blanks:  %d\n", whitespace);
}
