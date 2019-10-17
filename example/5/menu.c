#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <curses.h>

static FILE *output_stream = (FILE *)0;


char *menus[] = {
    "a - add new record",
    "d - delete record",
    "q - quit",
    NULL
};

// 选择输出.
int getchoice(char *greet, char *choices[], FILE *in, FILE *out);
int char_to_terminal(int char_to_write);

int main(void) 
{
    int choice = 0;

    FILE *input;
    FILE *output;

    struct termios initial_settings, new_settings;

    if(!isatty(fileno(stdout))) {
        printf("Don't redirected");
        return 0;
    }

    input = fopen("/dev/tty", "r");
    output = fopen("/dev/tty", "w");

    if(input == NULL || output == NULL) {
        fprintf(stderr, "Unable to open /dev/tty\n");
        return 0;
    }

    tcgetattr(fileno(input), &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_lflag &= ~ISIG;

    if(tcsetattr(fileno(input), TCSANOW, &new_settings) != 0) {
        fprintf(stderr, "could not set attributes\n");
        return 0;
    }

    do{
        choice = getchoice("Please enter the code:", menus, input, output);
        printf("Your have choicen:%c\n", choice);
    }while(choice != 'q');

    tcsetattr(fileno(input), TCSANOW, &initial_settings);

    return 0;
}

// 循环判断是否选中.
int getchoice(char *greet, char *choices[] , FILE *in, FILE *out) {
    // 准备循环.
    char **options;
    int chosen = 0;
    int selected = 0;

    int screenrow, screencol = 10;
    char *cursor, *clear;

    output_stream = out;
    setupterm(NULL, fileno(out), (int *)0);
    cursor = tigetstr("cup");
    clear  = tigetstr("clear");

    screenrow = 4;
    tputs(clear, 1, (int *) char_to_terminal);
    tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
    fprintf(out, "Choice: %s", greet);

    screenrow += 2;
    options = choices;
    while(*options) {
        tpush(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
        fprintf(out, "%s", *option);
        screenrow++;
        option++;
    }

    fprintf(out, "\n");
    do{
        fflush(out);
        selected = fgetc(in);
        option = choices;

        while(*option) {
            if(selected == *options[0]) {
                chosen = 1;
                break;
            }
            option++;
        }

        if(!chosen) {
            tputs(tparm(cursor, screenrow, screencol), 1, char_to_terminal);
            fprintf(out, "Incorrent choice, select again\n");
        }
    }while(!chosen);

    tputs(clear, 1, char_to_terminal);
    return selected;
}

int char_to_terminal(int char_to_write) 
{
    if (output_stream) putc(char_to_write, output_stream);
    return 0;
}