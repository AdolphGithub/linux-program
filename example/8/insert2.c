#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main(int argc, char *argv)
{
    MYSQL conn;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
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
        res = mysql_query(&conn, "SELECTED LAST_INSERT_ID()");
        if(res) {
            printf("SELECT error: %s\n", mysql_error(&conn));
        }else{
            res_ptr = mysql_use_result(&conn);
            if(res_ptr) {
                while( (sqlrow = mysql_fetch_row(res_ptr)) ){
                    printf("We inserted childno %s\n", sqlrow[0]);
                }
                mysql_free_result(res_ptr);
            }
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