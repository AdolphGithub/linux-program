#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ncurses.h>
#include "menu.h"

// 获取输入的信息.
int getchoice(char *greet, char *choices[], char *current_cd, char *current_cat)
{
    static int selected_row = 0;
    // 最大行数.
    int max_row = 0;
    // 开始行数和列数.
    int start_screenrow = MESSAGE_LINE, start_screencol = 0;

    char **option;
    int selected;
    int key = 0;

    option = choices;
    while(*option) {
        max_row++;
        option++;
    }
    // 初始化选择的行.
    if(selected_row >= max_row) {
        selected_row = 0;
    }

    // 清除屏幕.
    clear_all_screen(current_cd, current_cat);
    mvprintw(start_screenrow - 2, start_screencol, greet);
    keypad(stdscr, TRUE);   // 开启keypay模式
    cbreak();   // 设置为cbreak模式.
    noecho();   // 不要回显.
    key = 0;
    while(key != 'q' && key != KEY_ENTER && key != '\n') {
        // 判断键盘是否按住了向上键
        if(key == KEY_UP) {
            if(selected_row == 0) {
                selected_row = max_row - 1;
            }else{
                selected_row--;
            }
        }
        // 判断键盘是否按住了向下键
        if(key == KEY_DOWN) {
            if(selected_row == max_row - 1) {
                selected_row = 0;
            }else{
                selected_row++;
            }
        }

        selected = *choices[selected_row];
        // 开始绘制图像.
        draw_menu(choices, selected_row, start_screenrow, start_screencol);

        key = getch();
    }

    keypad(stdscr, FALSE);
    nocbreak();
    echo();

    if(key == 'q') {
        selected = 'q';
    }
    
    return selected;
}

// 绘制菜单.
void draw_menu(char *options[], int current_highlight, int start_row, int start_col)
{
    int current_row = 0;
    char **option_ptr;
    char *txt_ptr;

    option_ptr = options;
    while(*option_ptr) {
        if(current_row == current_highlight) attron(A_STANDOUT);
        txt_ptr = options[current_row];
        txt_ptr++;
        mvprintw(start_row + current_row, start_col, "%s", txt_ptr);
        if(current_row == current_highlight) attroff(A_STANDOUT);
        current_row++;
        option_ptr++;
    }

    mvprintw(start_row + current_row + 3, start_col, "Move highlight the press enter");
    // 刷新.
    refresh();
}

void clear_all_screen(char *current_cd, char *current_cat)
{
    clear();
    mvprintw(2, 20, "%s", "唱片应用");
    if(*(current_cd + 0) != '\0'){
        mvprintw(ERROR_LINE, 0, "当前CD: %s:%s", current_cat, current_cd);
    }

    refresh();
}