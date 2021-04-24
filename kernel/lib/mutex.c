#include <mutex.h>
#include <debug.h>

void lock(mutex_t* mutex){
    while(__sync_bool_compare_and_swap(mutex, UNLOCKED, LOCKED) != 1){
    
    }
}

void unlock(mutex_t* mutex){
    if(*mutex == UNLOCKED) panic("Trying to unlock a already unlocked mutex!");
    *mutex = UNLOCKED;
}