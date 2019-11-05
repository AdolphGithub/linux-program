#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

void display_row(MYSQL *conn, MYSQL_ROW *sqlrow);

int main(int argc, char *argv)
{
    MYSQL conn;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    int res;

    mysql_init(&conn);

    if(mysql_real_connect(&conn,"localhost","root","root","saas_idc", 3306, NULL, 0)) {
        printf("Connection success\n");
        res = mysql_query(&conn, "SELECT * FROM children");
        if(!res) {
            printf("SELECT %lu rows\n", (unsigned long)mysql_affected_rows(&conn));
        }else{
            res_ptr = mysql_use_result(&conn);
            if(res_ptr) {
                while((sqlrow = mysql_fetch_row(res_ptr))) {
                    printf("Fetched data...\n");
                    display_row(&conn, sqlrow);
                }

                if(mysql_errno(&conn)) {
                    fprintf(stderr, "Retrive error: %s\n", mysql_error(&conn));
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

void display_row(MYSQL *conn, MYSQL_ROW *sqlrow)
{
    unsigned int field_count;
    field_count = 0;

    while(field_count < mysql_field_count(conn)) {
        printf("%s ", sqlrow[field_count]);
        field_count++;
    }
    printf("\n");
}