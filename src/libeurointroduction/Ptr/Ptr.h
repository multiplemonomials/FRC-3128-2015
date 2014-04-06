/*
 * Ptr.h
 *
 *  Created on: Feb 21, 2014
 *      Author: jamie
 */

#ifndef PTR_H_
#define PTR_H_

#include <stdint.h>
#include <iomanip>

//NOTE: This file has been modified from a library written by Randall Smith

/*-----------------------------------------------------------------------------
    Small type that allows pointer values to be, e.g., compared and printed.
 ----------------------------------------------------------------------------*/

class Ptr
{
    void *              _pointer;
    uintptr_t           _scalar;

public:

    // Ctor (overload takes raw pointer)
    template <typename T>
    Ptr (T * pointer)
    :   _pointer(((void *) pointer)),
        _scalar((uintptr_t) _pointer)
    {
        // Empty.
    }


    // Ctor (overload takes shared pointer by value)
    template <typename T>
    Ptr (std::shared_ptr<T> sharedPointer)
    :   _pointer(((void *) sharedPointer.get())),
        _scalar((uintptr_t) _pointer)
    {
        // Empty.
    }


    // Return Ptr in string format.  "0x000000000000"
    std::string ToString() const
    {
        std::size_t const widthInChars(sizeof(void *) * 2);     // two hex digits per byte.
        std::ostringstream returnValue;

        returnValue << "0x" << std::hex << std::setw(widthInChars) << std::setfill('0') << _scalar;

        return returnValue.str();
    }


    // Return pointer as POD integer.
    uintptr_t ToInt() const
    {
        return _scalar;
    }


    // Equality operator.
    bool operator==(Ptr const & rhs) const
    {
        return _scalar == rhs._scalar;
    }


    // Inequality operator.
    bool operator!=(Ptr const & rhs) const
    {
        return !(*this == rhs);
    }
};


#endif /* PTR_H_ */
