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
```