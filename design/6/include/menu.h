#ifndef MENU_H
#define MENU_H

#define MAX_STRING 80   // 最大长度.
#define MESSAGE_LINE 6  // 信息展示长度.
#define ERROR_LINE 22

// 展示菜单和获取菜单的输入项.
int getchoice(char *greet, char *choices[], char *current_cd, char *current_cat);
// 绘制菜单.
void draw_menu(char *options[], int current_highlight, int start_row, int start_col);
// 清除屏幕.
void clear_all_screen(char *current_cd, char *current_cat);

#endif