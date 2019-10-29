#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main(void)
{
    int i;

    initscr();

    if(!has_colors()) {
        endwin();
        printf("Error - no color support on this terminal\n");
        return 0;
    }

    if(start_color() != OK) {
        endwin();
        printf("Error - cloud not initialize colors\n");
        return 0;
    }

    clear();
    mvprintw(5, 5, "There are %d COLORS, and %d COLOR_PAIRS available", COLORS, COLOR_PAIRS);
    refresh();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_GREEN);
    init_pair(3, COLOR_GREEN, COLOR_RED);
    init_pair(4, COLOR_YELLOW, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(6, COLOR_MAGENTA, COLOR_BLUE);
    init_pair(7, COLOR_CYAN, COLOR_WHITE);

    for( i = 0; i <= 7; i++) {
        attroff(A_BOLD);
        attrset(COLOR_PAIR(i));
        mvprintw(5 + i, 5, "Color pair %d", i);
        attrset(COLOR_PAIR(i) | A_BOLD);
        mvprintw(5 + i, 25, "Bold color pair %d", i);
        refresh();
        sleep(1);
    }

    endwin();
    return 0;
}