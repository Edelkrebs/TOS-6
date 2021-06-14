/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 											  
 * 	This is the file providing necessary utilities
 *  for controlling mutexes.
 * _______________________________________________
 */ 

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