/*
 * Global constants
 */
#define MAX_LINE_LEN 512
#define KEY_SPACE    ' '
#define KEY_TAB      '\t'
#define KEY_NEWLINE  '\n'

/*
 * Line counting struct. Contains file name and line counts.
 */
struct file_counts
{
    char *file_name;
    int code;
    int comment;
    int whitespace;
    int total;
};

/*
 * Function prototypes
 */
void init_file_count(struct file_counts *, char *);

void count_lines(struct file_counts *);
void print_lines(struct file_counts *);
void calc_globals(struct file_counts *, struct file_counts *);

int start_index(char *, int);
int comment_ends(char *, int);
