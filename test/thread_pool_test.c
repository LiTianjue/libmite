#include "inc/threadpool/threadpool.h"

void *work(void *arg)
{
    char *p = (char *)arg;
    printf("threadpool callback function:%s.\n",p);
    sleep(2);
}

int main(int argc,char **argv) 
{

    struct threadpool *pool = threadpool_init(5,5);
    threadpool_add_job(pool,work,"1");
    printf("add 1.\n");
    threadpool_add_job(pool,work,"2");
    printf("add 2.\n");
    threadpool_add_job(pool,work,"3");
    printf("add 3.\n");
    threadpool_add_job(pool,work,"4");
    printf("add 4.\n");
    threadpool_add_job(pool,work,"5");
    printf("add 5.\n");
    threadpool_add_job(pool,work,"6");
    printf("add 6.\n");
    threadpool_add_job(pool,work,"7");
    printf("add 7.\n");
    threadpool_add_job(pool,work,"8");
    printf("add 8.\n");
    threadpool_add_job(pool,work,"9");
    printf("add 9.\n");

    sleep(20);
    printf("\n#############\n");
    threadpool_add_job(pool,work,"6");
    printf("add 6.\n");
    threadpool_add_job(pool,work,"7");
    printf("add 7.\n");
    threadpool_add_job(pool,work,"8");
    printf("add 8.\n");
    threadpool_add_job(pool,work,"9");
    printf("add 9.\n");
    sleep(20);

    threadpool_destroy(pool);

    return 0;
}
