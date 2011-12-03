#include <stdio.h>

#define BUFFER_SIZE 256
#define KEY_SPACE   32
#define KEY_TAB     9
#define KEY_NEWLINE '\n'

/*
 * Function prototypes
 */
void count_lines (const char *fname);
void print_lines (int total, int code, int comment, 
                  int whitespace, const char *name);
int start_index (const int *line, int i);
void ratio_bar (char *bar_array, int total, int num);

/*
 * main
 */
int main (int argc, char **argv)
{
    // Robust error checking!
    if (argc == 1) {
        printf ("Usage: clines cfile1 [cfile2 ...]\n");
        return -1;
    }

    // Display line counts for each file
    int i;
    for (i = 1; i < argc; i++) {
        if (i > 1) {
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
    if (!f) {
        printf ("Couldn't open file: %s\n", fname);
        return;
    }

    int total      = 0;
    int code       = 0;
    int comment    = 0;
    int whitespace = 0;
    int in_comment = 0;

    int buffer[BUFFER_SIZE], c = 0;
    while (c != EOF) {
        // Put next line in buffer
        int i = 0;
        do {
            c = fgetc (f);
            buffer[i++] = c;
        } while (c != '\n' && c != EOF);
        total++;

        // Check if we're in a multi-line comment
        int s = start_index (buffer, i);
        if (in_comment) {
            // Check if multi-line comment ends
            int j;
            for (j = 0; j < i - 1; j++) {
                if (buffer[j] == '*' && buffer[j + 1] == '/') {
                    in_comment = 0;
                    break;
                }
            }
            comment++;
        }

        // Not in multi-line comment
        else {
            // Only whitespace characters
            if (i == s) {
                whitespace++;
            }

            // Single-line comment
            else if (buffer[s] == '/' && buffer[s + 1] == '/') {
                comment++;
            }

            // Multi-line comment
            else if (buffer[s] == '/' && buffer[s + 1] == '*') {
                comment++;
                in_comment = 1;
            }
        
            // Must be a line of code
            else {
                code++;
            }
        }
    }

    // Print result and return
    print_lines (total, code, comment, whitespace, fname);
    fclose (f);
}

/*
 * start_index
 */
int start_index (const int *line, int i)
{
    // Trim prepending whitespace
    int j;
    for (j = 0; j < i; j++) {
        if (line[j] != KEY_SPACE && line[j] != KEY_TAB
            && line[j] != KEY_NEWLINE)
            break;
    }
    return j;
}

/*
 * num_bars
 */
void ratio_bar (char *bar_array, int total, int num)
{
    int i;
    for (i = 0; i < 10; i++) {
        bar_array[i] = ' ';
    }

    for (i = 0; i < (num * 10) / total; i++) {
        bar_array[i] = 'x';
    } 
}

/*
 * print_lines
 */
void print_lines (int total, int code, int comment, 
                  int whitespace, const char *name)
{
    int code_ratio        = (float) code / total * 100;
    int comment_ratio     = (float) comment / total * 100;
    int whitespace_ratio  = (float) whitespace / total * 100;

    printf ("%s\n", name);
    printf ("%-10s %8d %8d%%\n", "Code", code, code_ratio);
    printf ("%-10s %8d %8d%%\n", "Comment", comment, comment_ratio);
    printf ("%-10s %8d %8d%%\n", "Blank", whitespace, whitespace_ratio);
    printf ("%-10s %8d %8s%%\n", "Total", total, "100");
}
