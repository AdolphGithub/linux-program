### curses函数库
在ubuntu18.04中,默认是没有这个函数库的,需要安装,安装命令如下
```
sudo apt install libncurses5-dev
```
### 基本函数
```
int addch(const chtype char_to_add);    // 在当前位置添加一个字符
int addchstr(chtype *const string_to_add);  // // 在当前位置添加一行字符

int printw(char *format, ...);  // 按格式化输出
int refresh(void);  // 刷新界面,成功返回OK,发生错误时返回ERR.

int box(WINDOW *win_ptr, chtype vertical_char, chtype horizontal_char); // 窗口绘制方框
int insch(chtype char_to_insert); // 插入一个字符,将已有字符向右移.

int insertln(void); // 添加一行,将现有行向下移一行
int delch(void);    // 删除一个字符,将已有字符往左移
int deleteln(void); // 删除一行,将现有行向上移一行
int beep(void);     // 让程序发出声音
int flash(void);    // 屏幕闪烁

// 从屏幕读取信息
chtype inch(void);  // 返回光标当前位置的字符及其属性信息
int instr(char *string);    // 获取光标当位置的字符及其属性信息并返回到string中.
int innstr(char *string, int number_of_characters); // 获取光标当前位置的字符及其属性信息并返回到string中.

// 清除屏幕
int erase(void);    // 每个屏幕位置上写上空白字符
int clear(void);    // 清屏
int clrtobot(void); // 清除当前光标位置直到屏幕结尾的所有内容
int clrtoeol(void); // 清除当前光标位置有到光标所处行行尾的所有内容

// 移动光标
int move(int new_y,int new_x);  // 移动到指定位置x和y轴.仅改变逻辑屏幕的光标位置,下次的输出内容就将出现在该位置上.
// 最大x的值为LINES,最大y的值为COLUMNS
int leaveok(WINDOW *window_ptr, bool leave_flag);  // 设置一个标志, 该标志用于控制在屏幕刷新后curses将物理光标放置的位置默认情况下,该标志为false

// 字符属性 
int attron(chtype attribute);   // 设置curses属性
int attroff(chtype attribute);  // 关闭指定属性的值
int attrset(chtype attribute);  // 设置指定属性的值
int standout(void);             // 更加通用的强调或"突出"模式 
int standend(void);             //

```
### 键盘
```
#include <curses.h>
// 键盘模式
int echo(void); // 开启字符回显功能
int noecho(void);   // 关闭字符回显功能
int cbreak(void);   // 将输入模式由一行一行的传给程序改为单个传入
int nocbreak(void); // 关闭cbreak模式.
int raw(void);      // 关闭特殊字符的处理
int noraw(void);    // 恢复cooked模式(行处理)和特殊字符处理功能

// 键盘输入
int getch(void);    // 获取一个由键盘输入的字符
int getstr(char *string);   // 获取一行由键盘输入的字符
int getnstr(char *string,int number_of_characters); // 获取一个长度为number_of_characters的由键盘输入的字符串
int scanw(char *format,...); // 格式化读取由键盘输入的字符通scanf
```
### 窗口
```
#include <curses.h>

// 窗口
WINDOW *newwin(int num_of_lines, int num_of_cols, int start_y, int start_x);    // 创建一个新窗口
int delwin(WINDOW *window_to_delete);    // 删除一个窗口.

int addch(const chtype char);   // 输出一个字符
int waddch(WINDOW *window_pointer, const chtype char);
int mvaddch(int y,int x,const chtype cahr);
int mvwaddch(WINDOW *window_pointer, int y, int x, const chtype char);
int printw(char *format,...);
int wprintw(WINDOW *window_pointer, char *format, ...);
int mvprintw(int y, int x, char *format, ...);
int mvwprintw(WINDOW *window_pointer, int y,int x, char *format, ...);

// 移动和更新窗口
int mvwin(WINDOW *window_to_move, int new_y, int new_x);    // 移动窗口.
int wrefresh(WINDOW *window_ptr);   // 刷新指定窗口
int werase(WINDOW *window_ptr);     // 每个屏幕位置上写上空白字符
int touchwin(WINDOW *window_ptr);   // 窗口内容发生改变.
int scrollok(WINDOW *window_ptr, bool scroll_flat); // 开启滚动
int scroll(WINDOW *window_ptr); // 窗口内容向上滚动一行.

int wnoutrefresh(WINDOW *window_ptr);   // 决定把哪些字符发送到屏幕上,并不真正发送这些字符.
int doupdate(void);  // 更新字符到窗口.
```
### 子窗口
```
#include <curses.h>

WINDOW *subwin(WINDOW *parent, int num_of_lins, int num_of_cols, int start_y, int start_x); // 创建子窗口.
int delwin(WINDOW *window_to_delete);   // 删除窗口.
```
### keypad模式
```
#include <curses.h>

int keypad(WINDOW *window_ptr, bool keypad_on); // 启用keypad模式.keypad_on为true则启动.
```
使用keypad需注意下面三点.
```
1.识别escape转义序列的过程是与时间相关的.
2.为了让curses能够区分"单独按下Escape键"和"一个以Escape字符开头的键盘转义序列", 它必须等待一小段时间.
3.curses不能处理二义性的escape转义序列.
```
### 彩色显示
```
#include <curses.h>

bool has_colors(void);  // 是否支持彩色.
int start_color(void);  // 初始化颜色显示

int init_pair(short pari_number, short foreground, short background);   // 初始化展示.
int COLOR_PAIR(int pair_number); 
int pair_content(short pari_number, short *foreground, short *background);  // 获取已定义的颜色组合的信息.
int init_color(short color_number, short red, short green, short blue); // 初始化颜色.
```
### pad
逻辑屏幕
```
#include <curses.h>

WINDOW *newpad(int number_of_lines, int number_of_columns); // 创建一个逻辑屏幕
int prefresh(WINDOW *pad_ptr, int pad_row, int pad_column, int screen_row_min, int screen_col_min, int screen_row_max, int screen_col_max);   // 截取部分屏幕显示到当前窗口.
```

