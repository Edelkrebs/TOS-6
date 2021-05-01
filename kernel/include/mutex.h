#ifndef __MUTEX_H
#define __MUTEX_H

typedef enum{
    LOCKED = 1,
    UNLOCKED = 0
} mutex_t;

void lock(mutex_t* mutex);
void unlock(mutex_t* mutex);

#endif