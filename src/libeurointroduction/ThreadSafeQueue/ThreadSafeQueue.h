/*
* ThreadSafeQueue.h
*
* Created on: Sep 30, 2013
* Author: jamie
*/

#ifndef THREADSAFEQUEUE_H_
#define THREADSAFEQUEUE_H_


/*------------------------------------------------------------------------------
ThreadSafeQueue.h

Queue template that can be safely shared by multiple threads.
------------------------------------------------------------------------------*/

#include <queue>
#include <mutex>
#include <condition_variable>
#include <InterruptibleWait/InterruptibleWaiter.h>


/*-----------------------------------------------------------------------------
* Queue template that can be safely shared by multiple threads. Uses
* really convenient Boost primitives for synchronization.
----------------------------------------------------------------------------*/

template<typename T>
class ThreadSafeQueue
{
    // Unprotected queue.
    std::deque<T> _deque;


    // Mutex object we will synchronize on.
    std::mutex _mutex;

    InterruptibleWaiter _waiter;

public:

    // Type of value returned by size().
    typedef typename std::deque<T>::size_type size_type;


    // Returns the number of items in the queue.
    size_type size()
    {
        // Acquire the lock (automagically release when leaving scope).
        std::unique_lock<std::mutex> lock(_mutex);

        return _deque.size();
    }


    // Add an object to the queue & notify any waiters.
    // Adds item at end of queue by default, optionally
    // at front (i.e., next to be dequeued).
    void Enqueue
    (
        T const & data,
        bool const & enqueueAtFront = false
    )
    {
        // Acquire the lock (automagically release when leaving scope).
        std::unique_lock<std::mutex> lock(_mutex);


        // Enqueue the data at the appropriate end of the queue.
        if (enqueueAtFront)
        {
            _deque.push_front(data);
        }
        else
        {
            _deque.push_back(data);
        }


        // Notify another waiting thread that data is ready.
        _waiter.awaken();
    }


    // Wait for data if needed, then pop the oldest item off the
    // queue & return by value.
    T Dequeue()
    {
        // Acquire the lock (automagically release when leaving scope).
        std::unique_lock<std::mutex> lock(_mutex);


        // If queue is empty, wait.
        while(_deque.size() == 0)
        {
            // Note: !lock! is automatically released while we wait,
            // then re-acquired after the wait. Sweet!
            _waiter.block(lock);
        }


        // When we get to here: (a) at least one item is in the queue,
        // and (b) we have the lock. Remove oldest data from queue & return it.
        T result = _deque.front();
        _deque.pop_front();
        return result;
    }

    //throws a ThreadInterruptedException in (one of) the blocking threads
    void Interrupt()
    {
    	_waiter.interrupt();
    }
};


#endif /* THREADSAFEQUEUE_H_ */
