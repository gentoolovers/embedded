#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define  DATABASE   "stu.db"

#define  N  128

typedef struct {
    int id;
    char name[32];
    char sex;
    int score;
}MSG;

int do_insert(sqlite3 *db)
{
    MSG msg;
    char sql[N] = {};
    char *errmsg;

    printf("Input id:");
    scanf("%d", &msg.id);
    getchar();

    printf("Input name:");
    scanf("%s", msg.name);
    getchar();

    printf("Input sex:");
    scanf("%c", &msg.sex);
    getchar();

    printf("Input score:");
    scanf("%d", &msg.score);
    getchar();

    sprintf(sql, "insert into student values(%d, '%s', '%c', %d)", msg.id, msg.name, msg.sex, msg.score);

    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("Insert done.\n");
    }

    return 0;
}

int do_delete(sqlite3 *db)
{

    MSG msg;
    char sql[N] = {};
    char *errmsg;

    printf("Input id:");
    scanf("%d", &msg.id);
    getchar();
    sprintf(sql, "delete from student where is %d", msg.id);
    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("delete done.\n");
    }

    return 0;
}

int callback(void *arg, int ncolumn, char **f_value, char ** f_name)
{
    int i = 0;

    for(i = 0; i < ncolumn; i++)
    {
        printf("%-11s  %s  ", f_value[i], f_name[i]);
    }
    putchar(10);
    printf("++++++++++++++++\n");

    return 0;
}

int do_query(sqlite3 *db)
{
    MSG msg;
    char sql[N] = {};
    char *errmsg;

#if 0
    printf("Input id:");
    scanf("%d", &msg.id);
    getchar();

    sprintf(sql, "select * from student where id=%d", msg.id);
#endif

    sprintf(sql, "select * from student");
    

    if(sqlite3_exec(db, sql, callback, NULL, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("query done.\n");
    }


    return 0;
}

int do_update(sqlite3 *db)
{
    MSG msg;
    char sql[N] = {};
    char *errmsg;

    printf("Input id:");
    scanf("%d", &msg.id);
    getchar();

    printf("Input name:");
    scanf("%s", msg.name);
    getchar();

    printf("Input sex:");
    scanf("%c", &msg.sex);
    getchar();

    printf("Input score:");
    scanf("%d", &msg.score);
    getchar();

    sprintf(sql, "update student set   name='%s', sex='%c',score= %d where %d)",msg.name, msg.sex, msg.score,msg.id);

    if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("Insert done.\n");
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    sqlite3 *db;
    char *errmsg;
    int n;

    if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
    {
        printf("%s\n", sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        printf("Create or Open %s success.\n", DATABASE);
    }

    if(sqlite3_exec(db,"create table student (id int, name vchar(20), sex char, score int);",
                NULL, NULL, &errmsg) != SQLITE_OK)
    {
        printf("%s\n", errmsg);
    }
    else
    {
        printf("Create table success.\n");
    }

    while(1)
    {
        printf("*******************************************\n");
        printf("1.insert  2.delete 3.query 4.update 5.quit \n");
        printf("*******************************************\n");
        scanf("%d", &n);
        getchar();

        switch(n)
        {
            case 1:
                do_insert(db);
                break;
            case 2:
                do_delete(db);
                break;
            case 3:
                do_query(db);
                break;
            case 4:
                do_update(db);
                break;
            case 5:
                sqlite3_close(db);
                exit(0);
                break;
            default:
                printf("Error input cmd.\n");
        }
    }

    
    return 0;
}
