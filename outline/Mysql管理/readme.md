### myisam修复工具
myisamchk -e -r *.MYI
```
-c 检查表以发现错误
-e 执行扩展检查
-r 修复发现的错误
```
### C链接ＭySQL
安装mysql-devel或libmysqld-dev用来支持.如下
```
sudo apt install mysql-devel    // or
sudo apt install libmysqld-dev
```
常用函数如下.
```
#include <mysql.h>

MYSQL *mysql_init(MYSQL *); // 初始化mysql
MYSQL *mysql_real_connect(MYSQL *connection, const char *server_host, const char *sql_user_name,const char *sql_password,const char *db_name,unsigned int prot_number); // 链接数据库.
void mysql_close(MYSQL *connection);    // 关闭mysql.
int mysql_options(MYSQL *connection, enum option_to_set, const char *argument); // 设置mysql的选项.例链接超时等.

unsigned int mysql_errno(MYSQL *connection);    // mysql错误代码
char *mysql_error(MYSQL *connection);           // mysql错误信息

int mysql_query(MYSQL *connection, const char *query);  // 执行mysql语句.如果成功则返回0.
my_ulonglong mysql_affected_rows(MYSQL *connection);    // 返回受查询所影响的行数.

MYSQL_RES *mysql_store_result(MYSQL *connection);       // 提取查询结果后所有的数据集,失败返回NULL
MYSQL *mysql_use_result(MYSQL *connection);             // 提取查询结果的结果集.
// 注意msyql_use_result和mysql_store_result的区别在于
// mysql_use_result是将结果集进行同步操作.如果在这期间,执行了查询将无法执行.
// mysql_store_result是将结果集缓存下来.在这期间你可以执行其他查询等操作.

my_ulonglong mysql_num_rows(MYSQL_RES *result);         // 返回结果集的行数.

MYSQL_ROW mysql_fetch_row(MYSQL_RES *result);           // 这个函数从使用mysql_store_result得到的结构中提取一行.
void mysql_data_seek(MYSQL_RES *result, my_ulonglong offset);   // 设置下一个mysql_fetch_row所返回的行.
MYSQL_ROW_OFFSET mysql_row_tell(MYSQL_RES *result);     // 返回一个偏移值,表示当前结果集的位置.
MYSQL_ROW_OFFSET mysql_row_seek(MYSQL_RES *result, MYSQL_ROW_OFFSET); // 设置当前结果集的位置.并返回之前的位置.
void mysql_free_result(MYSQL_RES *result);              // 清空当前的结果集．

unsigned int mysql_fetch_count(MYSQL *connection);      // 获取字段个数.
MYSQL_FIELD *mysql_fetch_field(MYSQL_RES *result);      // 返回一个表字段.

// 其他函数.
char *mysql_get_client_info(void);      // 返回客户端使用的库的版本信息
char *mysql_get_host_info(MYSQL *connection);   // 返回服务器链接信息
char *mysql_get_server_info(MYSQL *connection); // 返回当前链接信息
char *mysql_info(MYSQL *connection);            // 返回最近执行的查询信息

int mysql_select_db(MYSQL *connection, const char *dbname); // 如果用户拥有合适的权限,则把默认数据库改为参数指定的数据库.成功时返回0
int mysql_shutdown(MYSQL *connection,enum mysql_enum_shutdown_level); // 关闭链接的数据库服务.
```
MYSQL_FIELD结构体如下
```
char *name;     // 列名
char *table;    // 表明
char *def;      // 该列的默认值
enum enum_field_types type; // 列的类型
unsigned int length;        // 长度
unsigned int max_length;    // 最长列的长度
unsigned int flags;         // 关于列的标志
unsigned int decimals;      // 小数点的数字个数.仅对数字字段有效.
```