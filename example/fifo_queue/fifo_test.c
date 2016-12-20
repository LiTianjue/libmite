#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queue.h"




int test()
{
	int i = 10;
	int max = 20;
	int ret = 0;
	Queue *buffqueue = initQueue(max);
	char *buff = NULL;
	
	while(ret != QUEUE_IS_FULL && QUEUE_SIZE(buffqueue) < max )
	{
		buff= (char *)malloc(1024);
		sprintf(buff,"Hello ,I'am %d",QUEUE_SIZE(buffqueue));
		ret = enqueue(buffqueue,(void *)buff);
	}


	while(ret != QUEUE_IS_EMPTY && QUEUE_SIZE(buffqueue) > 0)
	{
		buff = front(buffqueue);	
		if(buff != NULL)
		{
			printf("pop out : [%s]\n",buff);
			ret = dequeue(buffqueue);
			free(buff);
			buff= NULL;
		}
	}

	return 0;
}

int main(){
	test();
	return 0;
}
