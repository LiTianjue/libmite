#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define HOST		"localhost"		//数据库地址
#define	USER		"root"			//用户
#define PASSWD		"admin"			//密码
#define DATABASE	"nfcid"			//数据库名
#define FORM		"dev_list"		//表名


int main()
{
	printf("Hello MySQL.\n");
    MYSQL *conn_ptr;
    conn_ptr = mysql_init(NULL);
    if(!conn_ptr)
    {
        printf("mysql-init failed.\n");
        return -1;
    }

    conn_ptr = mysql_real_connect(conn_ptr,HOST,USER,PASSWD,DATABASE,0,NULL,0);


    int res=0;
    int i,j;
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    MYSQL_FIELD *fd;
    char cmd[256] = {0};
    char *form = FORM;

    if(conn_ptr)
    {
        printf("connect Success .\n");
        sprintf(cmd,"select %s,%s from %s ","serial","flag",form);
        printf("CMD :%s\n",cmd);
        res = mysql_query(conn_ptr,cmd);
        if(res)
        {
            printf("SELECT error:%s\n",mysql_errno(conn_ptr));
        }else
        {
            //取出查询结果
            res_ptr = mysql_store_result(conn_ptr);
            if(res_ptr)
            {
				//查询到的记录个数
                printf("%lu Rows.\n",(unsigned long)mysql_num_rows(res_ptr));
                j = mysql_num_fields(res_ptr);
				//属性个数 j,打印属性的字符串	
				for(i = 0;i<j;i++)
				{
                        fd = mysql_fetch_field(res_ptr);
                        printf("%s\t\t",fd->name);
				}
				printf("\n");
				
                while((sqlrow = mysql_fetch_row(res_ptr)))
                {
                    //依次取出记录输出
                    for(i=0;i<j;i++)
                    {
						/*
						if(i)
						{
                        	printf("flag:%s  ",sqlrow[i]);
						
						}else
						{
                        	printf("serial:%s\t",sqlrow[i]);
						}
						*/
						printf("%s\t",sqlrow[i]);
                    }

                    printf("\n");
                }
                if(mysql_errno(conn_ptr))
                {
                    fprintf(stderr,"Retrive error:%s\n",mysql_error(conn_ptr));
                }

                mysql_free_result(res_ptr);
            }
        }
    }else
    {
        printf("connect Fail.\n");
    }

    mysql_close(conn_ptr);

	return 0;
}

