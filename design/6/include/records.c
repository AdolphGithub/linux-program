#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "records.h"
#include "menu.h"

static char *title_file="title.cdb";
static char *tracks_file = "tracks.cdb";
static char *temp_file = "cbd.tmp";

// 添加一个新的CD
void add_record(char *current_cd, char *current_cat) 
{
    char cate_log_number[MAX_STRING];   // 分类编号
    char cd_title[MAX_STRING];          // cd标题
    char cd_type[MAX_STRING];           // cd分类
    char cd_artist[MAX_STRING];         // cd的作曲家
    char cd_entry[MAX_STRING];          // cd的入口 

    int screenrow = MESSAGE_LINE;
    int screencol = 0;

    clear_all_screen(current_cd, current_cat);
    mvprintw(screenrow, screencol, "请输入CD的详细信息");
    screenrow+=2;

    mvprintw(screenrow, screencol, "分类ID:");
    get_string(cate_log_number);
    screenrow++;

    mvprintw(screenrow, screencol, "CD标题:");
    get_string(cd_title);
    screenrow++;

    mvprintw(screenrow, screencol, "CD类型:");
    get_string(cd_type);
    screenrow++;

    mvprintw(screenrow, screencol, "CD作者:");
    get_string(cd_artist);
    screenrow++;

    mvprintw(PROMPT_LINE - 2, 5, "即将添加此新条目:");
    sprintf(cd_entry, "%s,%s,%s,%s", cate_log_number, cd_title, cd_type, cd_artist);
    mvprintw(PROMPT_LINE, 5, "%s", cd_entry);
    refresh();

    move(PROMPT_LINE, 0);

    if(get_confirm()) {
        insert_title(cd_entry);
        strcpy(current_cd, cd_title);
        strcpy(current_cat, cate_log_number);
    }
}

// 获取输入的字符串.
void get_string(char *string) 
{
    int len;
    wgetnstr(stdscr, string, MAX_STRING);
    len = strlen(string);

    if(len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
}

// 确定输入.
int get_confirm(void) 
{
    int confirmed = 0;
    char first_char;
    mvprintw(Q_LINE, 5, "是否继续(Y/N)?");
    clrtoeol();
    refresh();

    cbreak();
    first_char = getch();

    if(first_char == 'Y' || first_char == 'y') {
        confirmed = 1;
    }
    nocbreak();

    if(!confirmed) {
        mvprintw(Q_LINE, 1, "已取消");
        clrtoeol();
        refresh();
        sleep(1);
    }

    return confirmed;
}

// 插入标题.
void insert_title(char *cd_title) 
{
    FILE *fp = fopen(title_file, "a");
    if(!fp) {
        mvprintw(ERROR_LINE, 0, "无法打开存储CD的数据库文件");
    }else{
        fprintf(fp, "%s\n", cd_title);
        fclose(fp);
    }
}

// 更新CD
void update_cd(char *current_cd, char *current_cat)
{
    FILE *tracks_fp;
    char track_name[MAX_STRING];

    int len;
    int track = 1;
    int screen_line = 1;

    WINDOW *box_window_ptr;
    WINDOW *sub_window_ptr;

    clear_all_screen(current_cd, current_cat);
    mvprintw(PROMPT_LINE, 0, "是否需要重新输入CD的曲目");

    if(!get_confirm()) {
        return;
    }

    move(PROMPT_LINE, 0);
    clrtoeol();

    remove_tracks(current_cd, current_cat);
    mvprintw(MESSAGE_LINE, 0, "清空曲目完毕...");
    tracks_fp = fopen(tracks_file, "a");

    box_window_ptr = subwin(stdscr, BOXED_LINES + 2, (int)BOX_ROW_POS + 2, BOX_LINE_POS - 1, BOX_ROW_POS - 1);

    if(!box_window_ptr) {
        return;
    }
    box(box_window_ptr, ACS_VLINE, ACS_HLINE);
    sub_window_ptr = subwin(stdscr, BOXED_LINES, BOXED_ROW, BOX_LINE_POS, BOX_ROW_POS);

    if(!sub_window_ptr){
        return;
    }
    scrollok(sub_window_ptr, TRUE);
    werase(sub_window_ptr);

    touchwin(stdscr);

    do{
        mvwprintw(sub_window_ptr, screen_line++, BOX_ROW_POS + 2, "曲目%d:", track);
        clrtoeol();
        refresh();
        wgetnstr(sub_window_ptr, track_name, MAX_STRING);
        len = strlen(track_name);

        if(len > 0 && track_name[len - 1] == '\n') {
            track_name[len - 1] = '\0';
        }

        if(*track_name) {
            fprintf(tracks_fp, "%s,%d,%s\n", current_cat, track, track_name);
            track++;
        }

        if(screen_line > BOXED_LINES - 1) {
            scroll(sub_window_ptr);
            screen_line--;
        }
    }while(*track_name);

    delwin(sub_window_ptr);
    fclose(tracks_fp);
}

// 删除cd
void remove_cd(char *current_cd, char *current_cat) {
    FILE *titles_fp, *temp_fp;

    char entry[MAX_ENTRY];
    int cat_length;

    if(current_cd[0] == '\0') {
        return;
    }

    clear_all_screen(current_cd, current_cat);
    mvprintw(PROMPT_LINE, 0, "您是否要删除这张CD %s:%s", current_cd, current_cat);

    if(!get_confirm()){
        return;
    }

    cat_length = strlen(current_cat);

    titles_fp = fopen(title_file, "r");
    temp_fp = fopen(temp_file, "w");

    while(fgets(entry, MAX_ENTRY, titles_fp)) {
        if(strncmp(current_cat, entry, cat_length) != 0) {
            fputs(entry, temp_fp);
        }
    }

    fclose(titles_fp);
    fclose(temp_fp);

    unlink(title_file);
    rename(temp_file, title_file);

    remove_tracks(current_cd, current_cat);
    current_cd[0] = '\0';
}

void remove_tracks(char *current_cd, char *current_cat) 
{
    FILE *tracks_fp, *temp_fp;

    char entry[MAX_ENTRY];

    int cat_length;

    if(current_cd[0] == '\0') {
        return;
    }

    cat_length = strlen(current_cat);

    tracks_fp = fopen(tracks_file, "r");
    if(tracks_fp == (FILE *)NULL) return;

    temp_fp = fopen(temp_file, "w");
    
    while(fgets(entry, MAX_ENTRY, tracks_fp)) {
        if(strncmp(current_cat, entry, cat_length) != 0) {
            fputs(entry, temp_fp);
        }
    }

    fclose(tracks_fp);
    fclose(temp_fp);

    unlink(tracks_file);
    rename(temp_file, tracks_file);
}

// 统计曲目.
void count_cds(void) {
    FILE *titles_fp, *tracks_fp;
    char entry[MAX_ENTRY];

    int titles = 0;
    int tracks = 0;

    titles_fp = fopen(title_file, "r");
    if(titles_fp) {
        while(fgets(entry, MAX_ENTRY, titles_fp))
            titles++;
        
        fclose(titles_fp);
    }

    tracks_fp = fopen(tracks_file, "r");
    if(tracks_fp) {
        while(fgets(entry, MAX_ENTRY, tracks_fp)) {
            tracks++;
        }
        fclose(tracks_fp);
    }

    mvprintw(ERROR_LINE, 0, "总共有%d文件, 曲目:%d", titles, tracks);
    get_return();
}

// 查找CD.
void find_cd(char *current_cd, char *current_cat)
{
    char match[MAX_STRING], entry[MAX_ENTRY];
    FILE *titles_fp;

    int count = 0;
    char *found, *title, *catalog;
    mvprintw(Q_LINE, 0, "请输入需要搜索的标题");
    get_string(match);

    titles_fp = fopen(title_file, "r");
    if(titles_fp) {
        while(fgets(entry, MAX_ENTRY, titles_fp)) {
            catalog = entry;
            if(found == strstr(catalog, ",")) {
                *found = '\0';
                title = found + 1;
                if(found = strstr(title, ",")) {
                    *found = '\0';
                    if(found == strstr(title, match)) {
                        count++;
                        strcpy(current_cd, title);
                        strcpy(current_cat, catalog);
                    }
                }
            }
        }

        fclose(titles_fp);
    }

    if(count != 1) {
        if(count == 0) {
            mvprintw(ERROR_LINE, 0, "没有查找到匹配的CD");
        }

        if(count > 1) {
            mvprintw(ERROR_LINE, 0, "匹配到多项 %d", count);
        }

        current_cd[0] = '\0';
        get_return();
    }
}

// 列出列表.
void list_tracks(char *current_cd, char *current_cat) 
{
    FILE *tracks_fp;
    char entry[MAX_ENTRY];
    int cat_length;
    int lines_op = 0;
    WINDOW *track_pad_ptr;
    int tracks = 0;
    int key;
    int first_line = 0;

    if(current_cd[0] == '\0') {
        mvprintw(ERROR_LINE, 0, "您必须选择一个CD");
        get_return();
        return;
    }

    clear_all_screen(current_cd, current_cat);
    cat_length = strlen(current_cd);

    tracks_fp = fopen(tracks_file, "r");
    if(!tracks_fp) {
        return;
    }

    while(fgets(entry, MAX_ENTRY, tracks_fp)) {
        if(strncmp(current_cat, entry, cat_length) == 0) {
            tracks++;
        }
    }

    fclose(tracks_fp);

    track_pad_ptr = newpad(tracks + 1 + BOXED_LINES, BOXED_ROW + 1);
    if(!track_pad_ptr) {
        return;
    }

    tracks_fp = fopen(tracks_file, "r");
    if(!tracks_fp) {
        return;
    }

    mvprintw(4, 0, "CD曲目:");
    while(fgets(entry, MAX_ENTRY, tracks_fp)) {
        if(strncmp(current_cat, entry, cat_length) == 0) {
            mvwprintw(track_pad_ptr, lines_op++, 0, "%s", entry + cat_length + 1);
        }
    }

    fclose(tracks_fp);
    if(lines_op > BOXED_LINES) {
        mvprintw(MESSAGE_LINE, 0, "光标键滚动，返回或q退出");
    }else{
        mvprintw(MESSAGE_LINE, 0, "返回或q退出");
    }
    wrefresh(stdscr);
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    key = 0;

    while(key != 'q' && key != KEY_ENTER && key != '\n') {
        if(key == KEY_UP) {
            if(first_line > 0) {
                first_line--;
            }
        }

        if(key == KEY_DOWN) {
            if(first_line + BOXED_LINES + 1 < tracks) {
                first_line++;
            }
        }

        prefresh(track_pad_ptr, first_line, 0, BOX_LINE_POS, BOX_ROW_POS, BOX_LINE_POS + BOXED_LINES, BOX_ROW_POS + BOXED_ROW);
        key = getch();
    }

    delwin(track_pad_ptr);
    keypad(stdscr, FALSE);
    nocbreak();
    echo();
}

void get_return(void)
{
    int ch;
    mvprintw(23, 0, "%s", " Press return");
    refresh();
    while((ch = getchar()) != '\n' && ch != EOF );
}