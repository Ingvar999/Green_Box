#include "task_manager.h"
#include "stm32f7xx_hal.h"

#include "stdlib.h"

typedef struct {
	
	pHandler handler;
	void *context;
	unsigned timeToStart;
	void *next;
	
} TaskItem, *pTaskItem;

static pTaskItem head = 0, end = 0;

int AddTask(pHandler task, void *context, unsigned delay){
	pTaskItem item = (pTaskItem)malloc(sizeof(TaskItem));
	item->handler = task;
	item->context = context;
	item->next = 0;
	item->timeToStart = HAL_GetTick() + delay;
	if (end == 0){
		head = end = item;
	}
	else{
		end->next = item;
		end = item;
	}
	return 0;
}

void DispatchTasks(void){
	pTaskItem current = head, previous = 0;
	int done = 0;
	unsigned time = HAL_GetTick();
		
	while (current != 0 && !done){
		if (current->timeToStart <= time){
			current->handler(current->context);
			if (previous != 0){
				previous->next = current->next;
			}
			else{
				head = current->next;
			}
			if (current == end){
				end = previous;
			}
			free(current);
			done = 1;
		}
		else{
			previous = current;
			current = current->next;
		}
	}
	if (!done){
		HAL_Delay(1);
	}
}
