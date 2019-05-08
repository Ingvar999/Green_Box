#pragma once

typedef void (*pHandler)(void *);

int AddTask(pHandler task, void *context, unsigned delay);
void DispatchTasks(void);
