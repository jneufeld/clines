#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "clines.h"

/*
 * main
 */
int main(int argc, char **argv)
{
    // Robust error checking!
    if(argc == 1) {
        printf("Usage: clines cfile1 [cfile2 ...]\n");
        exit(-1);
    }

    // Create file_counts structs for individual files and global count
    struct file_counts current_count;
    struct file_counts global_count; 
    init_file_count(&global_count, "Totals");

    // Display line counts for each file
    int i;
    for(i = 1; i < argc; i++) {
        if(i > 1) {
            printf("\n");
        }

        init_file_count(&current_count, argv[i]);
        count_lines(&current_count);
        print_lines(&current_count);
        calc_globals(&global_count, &current_count);
    }

    // If multiple files given, print totals for all files
    if(argc > 2) {
        printf("\n");
        print_lines(&global_count);
    }

    return 0;
}

/*
 * count_lines: Open file and parse stats into counts struct.
 */
void count_lines(struct file_counts *counts)
{
    FILE *file_ptr = fopen(counts->file_name, "r");
    if(!file_ptr) {
        printf("Couldn't open file: %s\n", counts->file_name);
        return;
    }

    // Local counts, just for this file
    char buffer[MAX_LINE_LEN];
    int in_comment = 0;

    while(fgets(buffer, MAX_LINE_LEN, file_ptr) != NULL) {
        counts->total++;

        int length = strlen(buffer);

        // Check if we're in a multi-line comment
        int start  = start_index(buffer, length);
        if(in_comment) {
            if(comment_ends (buffer, length)) {
                in_comment = 0;
            }

            counts->comment++;
        }

        // Not in multi-line comment
        else {
            // Only whitespace characters
            if(length == start) {
                counts->whitespace++;
            }

            // Single-line comment
            else if(buffer[start] == '/' && buffer[start + 1] == '/') {
                counts->comment++;
            }

            // Multi-line comment
            else if(buffer[start] == '/' && buffer[start + 1] == '*') {
                counts->comment++;

                if(!comment_ends (buffer, length)) {
                    in_comment = 1;
                }
            }
        
            // Must be a line of code
            else {
                counts->code++;
            }
        }
    }

    fclose(file_ptr);
}

/*
 * start_index: Return index of first non-whitespace character.
 */
int start_index(char *line, int i)
{
    // Trim prepending whitespace
    int j;
    for(j = 0; j < i; j++) {
        if (line[j] != KEY_SPACE
                && line[j] != KEY_TAB
                && line[j] != KEY_NEWLINE)
            break;
    }
    return j;
}

/*
 * comment_ends: Return 1 if a multiline comment is terminated in buffer. 
 */
int comment_ends(char *buffer, int len)
{
    int j;
    for(j = 0; j < len; j++) {
        if(buffer[j] == '*' && buffer[j + 1] == '/') {
            return 1;
        }
    }
    return 0;
}

/*
 * print_lines: Print formatted stats for a file.
 */
void print_lines(struct file_counts *counts)
{
    if(!counts) {
        return;
    }

    int code_ratio        = (float) counts->code / counts->total * 100;
    int comment_ratio     = (float) counts->comment / counts->total * 100;
    int whitespace_ratio  = (float) counts->whitespace / counts->total * 100;

    printf("%s\n", counts->file_name);
    printf("%-10s %8d %8d%%\n", "Code", counts->code, code_ratio);
    printf("%-10s %8d %8d%%\n", "Comment", counts->comment, comment_ratio);
    printf("%-10s %8d %8d%%\n", "Blank", counts->whitespace, whitespace_ratio);
    printf("%-10s %8d %8s%%\n", "Total", counts->total, "100");
}

/*
 * calc_globals: Increase global counts by current file's counts.
 */
void calc_globals(struct file_counts *global, struct file_counts *current)
{
    global->code        += current->code;
    global->comment     += current->comment;
    global->whitespace  += current->whitespace;
    global->total       += current->total;
}

/*
 * init_file_count: Initialize file_counts struct with given name.
 */
void init_file_count(struct file_counts *counts, char *name)
{
    // Set the file's name
    counts->file_name = malloc(sizeof(char) * strlen(name));
    strncpy(counts->file_name, name, strlen(name));

    // Set all fields to 0
    counts->code       = 0;
    counts->comment    = 0;
    counts->whitespace = 0;
    counts->total       = 0;
}
