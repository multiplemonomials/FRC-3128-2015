/*
 * DefineZeroedStruct.h
 *
 *  Created on: Mar 14, 2014
 *      Author: jamie
 */

#ifndef DEFINEZEROEDSTRUCT_H_
#define DEFINEZEROEDSTRUCT_H_

#include <string.h>

/*-----------------------------------------------------------------------------
   Macro that conveniently defines, then zeroes out, a (normally) struct
   of the specified type and name.  Eliminates problems with calls to
   C services that take often-bizzare descriptors.

   E.g.:

       DEFINE_ZEROED_STRUCT(struct itimerspec, myItimerspec);
 ----------------------------------------------------------------------------*/

#define DEFINE_ZEROED_STRUCT(__type, __name) __type __name; memset(&__name, 0, sizeof(__name))


#endif /* DEFINEZEROEDSTRUCT_H_ */
