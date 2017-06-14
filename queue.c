#include "CrossReferenceFileBuilder.h"

/******************ENQUEUE******************
Accepts front & rear of queue and line #.
If queue wasn't initialized, it sets front = rear
and assigns data, otherwise new nodes are added 
to the back of the queue.
*/
void enqueue(QUEUENODE **queue, QUEUENODE **rear, int line_num)
{
	QUEUENODE *pnew;
	pnew = (QUEUENODE *)malloc(sizeof (QUEUENODE));
	if (!pnew)
	{
		printf("Error in enqueue!\n");
		return;
	}
	pnew->line_num = line_num;
	pnew->next = NULL;
	if (!(*queue))
		*queue = pnew;
	else
		(*rear)->next = pnew;
	*rear = pnew;
	return;
}

/******************DEQUEUE******************
Accepts front & rear of queue and passes back
the front node. New front = formerly 2nd in queue.
*/
QUEUENODE *dequeue(QUEUENODE **queue, QUEUENODE **rear)
{
	QUEUENODE *first;
	if (!(*queue)) return NULL;
	first = *queue;
	*queue = (*queue)->next;
	if (!(*queue)) *rear = NULL;
	first->next = NULL;
	return first;
}
