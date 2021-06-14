/*________________________________________________
 *	This file is part of TOS-6, a hobby Operating 
 *	System made by github user Edelkrebs.		  
 * 												  
 * 	All the code you see in this file is licensed 
 * 	under the BSD 2-Clause license.		  
 * 												  
 * 	This file contains necessary string utilities.
 * _______________________________________________
 */ 

#include <string.h>

#include <stddef.h>

size_t strlen(const char* str){
	size_t len = 0;
	while(str[len]){
		len++;
	}
	return len;
}