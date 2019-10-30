#include <ncurses.h>
#include <locale.h>
#include "include/menu.h"
#include "include/records.h"

char *main_menu[] = {
    "a:添加一个新的CD",
    "f:查找CD",
    "c:统计CD和曲目信息",
    "q:退出",
    0
};

char *extended_menu[] = {
    "a:添加一个新的CD",
    "f:查找CD",
    "c:统计CD和曲目信息",
    "l:列出当前CD中的曲目",
    "r:删除一个CD",
    "u:更新曲目信息",
    "q:退出",
    0
};
// 主函数负责渲染菜单.
int main(void)
{
    int choice;
    char current_cd[MAX_STRING] = {'\0'};
    char current_cat[MAX_STRING] = {'\0'};

    setlocale(LC_ALL,"");
    initscr();

    do{
        choice = getchoice("Options:", current_cd[0] != '\0' ? extended_menu : main_menu, current_cd, current_cat);

        switch (choice)
        {
            case 'a':
                add_record(current_cd, current_cat);
                break;
            case 'f':
                find_cd(current_cd, current_cat);
                break;
            case 'c':
                count_cds();
                break;
            case 'l':
                list_tracks(current_cd,current_cat);
                break;
            case 'r':
                remove_cd(current_cd,current_cat);
                break;
            case 'u':
                update_cd(current_cd, current_cat);
                break;
            case 'q':
                break;
            default:
                break;
        }
    }while(choice != 'q');

    endwin();
    return 0;
}