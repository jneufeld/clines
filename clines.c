#include <stdio.h>

#define BUFFER_SIZE 256
#define KEY_SPACE   32
#define KEY_TAB     9
#define KEY_NEWLINE '\n'

/*
 * Global counts
 */
int code_g        = 0;
int comment_g     = 0;
int whitespace_g  = 0;
int total_g       = 0;

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

    // If multiple files given, print totals for all files
    if (argc > 2) {
        printf ("\n");
        print_lines (total_g, code_g, comment_g, whitespace_g, "Totals");  
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

    // Local counts, just for this file
    int total_l         = 0;
    int code_l          = 0;
    int comment_l       = 0;
    int whitespace_l    = 0;
    int in_comment      = 0;

    int buffer[BUFFER_SIZE], c = 0;
    while (c != EOF) {
        // Put next line in buffer
        int i = 0;
        do {
            c = fgetc (f);
            buffer[i++] = c;
        } while (c != '\n' && c != EOF);
        total_l++;
        total_g++;

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
            comment_l++;
            comment_g++;
        }

        // Not in multi-line comment
        else {
            // Only whitespace characters
            if (i == s) {
                whitespace_l++;
                whitespace_g++;
            }

            // Single-line comment
            else if (buffer[s] == '/' && buffer[s + 1] == '/') {
                comment_l++;
                comment_g++;
            }

            // Multi-line comment
            else if (buffer[s] == '/' && buffer[s + 1] == '*') {
                comment_l++;
                comment_g++;
                in_comment = 1;
            }
        
            // Must be a line of code
            else {
                code_l++;
                code_g++;
            }
        }
    }

    // Print result and return
    print_lines (total_l, code_l, comment_l, whitespace_l, fname);
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
