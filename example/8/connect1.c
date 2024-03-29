#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main(int argc, char *argv)
{
    MYSQL *conn_ptr;

    conn_ptr = mysql_init(NULL);

    if(!conn_ptr) {
        fprintf(stderr, "mysql init failed\n");
        return EXIT_FAILURE;
    }

    conn_ptr = mysql_real_connect(conn_ptr,"localhost","root","root","saas_idc", 3306, NULL, 0);
    
    if(conn_ptr) {
        printf("Connection success\n");
    }else{
        printf("Connection failed\n");
    }

    mysql_close(conn_ptr);

    return EXIT_SUCCESS;
}