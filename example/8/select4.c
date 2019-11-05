#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

void display_row(MYSQL *conn, MYSQL_ROW *sqlrow);
void display_header(MYSQL *conn, MYSQL_RES *res);

int main(int argc, char *argv)
{
    MYSQL conn;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    int res;
    int fetch_header;

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
                    if(fetch_header) {
                        display_header(&conn, res_ptr);
                    }
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

// 展示函数.
void display_header(MYSQL *conn,MYSQL_RES *res)
{
    MYSQL_FIELD *field_ptr;
    printf("Column details: \n");

    while((field_ptr = mysql_fetch_field(res)) != NULL) {
        printf("\t Name: %s\n", field_ptr->name);
        printf("\t Type: ");

        if(IS_NUM(field_ptr->type)) {
            printf("Numeric field\n");
        }else{
            switch (field_ptr->type)
            {
                case FIELD_TYPE_VAR_STRING:
                    printf("varchar\n");
                    break;
        
                case FIELD_TYPE_LONG:
                    printf("LONG\n");
                    break;
                default:
                    printf("Type is %d, check in mysql_com.h\n", field_ptr->type);
                    break;
            }
        }

        printf("\t MAX width %ld\n", field_ptr->length);
        if(field_ptr->flags & AUTO_INCREMENT_FLAG) {
            printf("\t Auto increments\n");
        }
        printf("\n");
    }
}