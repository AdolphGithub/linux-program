#ifndef RECORDS_H
#define RECORDS_H

#define PROMPT_LINE 18
#define Q_LINE 20 

// 更新数据库
#define BOXED_LINES 11
#define BOXED_ROW 80
#define BOX_LINE_POS 8
#define BOX_ROW_POS 2
#define MAX_ENTRY 1024

// 添加一个新的CD唱片.
void add_record(char *current_cd, char *current_cat);
// 获取一个字符串.
void get_string(char *string);
// 获取用户的输入确认.
int get_confirm(void);
// 存储一个文件title.
void insert_title(char *cdtitle);
// 更新一个CD信息.
void update_cd(char *current_cd, char *current_cat);
// 删除一个CD.
void remove_cd(char *current_cd, char *current_cat);
// 删除曲目信息.
void remove_tracks(char *current_cd, char *current_cat);
// 统计数据库中有多少cd.
void count_cds(void);
// 查找cd
void find_cd(char *current_cd, char *current_cat);
// 列出cd中的曲目.
void list_tracks(char *current_cd, char *current_cat);
// 提示用户按下回车键
void get_return(void);
#endif

