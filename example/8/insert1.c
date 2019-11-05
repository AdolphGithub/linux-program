#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main(int argc, char *argv)
{
    MYSQL conn;
    int res;

    mysql_init(&conn);

    if(mysql_real_connect(&conn,"localhost","root","root","saas_idc", 3306, NULL, 0)) {
        printf("Connection success\n");
        res = mysql_query(&conn, "INSERT INTO children(fanme, age) VALUES ('Ann',30)");
        if(!res) {
            printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
        }else{
            fprintf(stderr, "Insert error %d:%s", mysql_errno(&conn), mysql_error(&conn));
        }
        mysql_close(&conn);
    }else{
        fprintf(stderr, "Connection failed\n");
        if(mysql_errno(&conn)) {
            fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&conn), mysql_error(&conn));
        }
    }

    return EXIT_SUCCESS;
}