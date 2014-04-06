/*
 * LogOutput.h
 *
 * Backend code that hooks together three objects to form a log backend.
 */

#ifndef LOGOUTPUT_H_
#define LOGOUTPUT_H_

#include <memory>
#include <boost/thread/thread.hpp>

#include <LogMessage.h>
#include <ThreadSafeQueue/ThreadSafeQueue.h>

// base class so functions can accept an unknown template
class LogOutputBaseClass
{
public:
	virtual void enqueueMessage(std::shared_ptr<LogMessage> messagePtr) = 0;

	virtual ~LogOutputBaseClass(){}
};

template<class Acceptor_T, class Formatter_T, class Writer_T>
class LogOutput : public LogOutputBaseClass
{
	//holds all of the messages to be logged
	ThreadSafeQueue<std::shared_ptr<LogMessage>> _inputQueue;

	volatile bool _shouldStop;

	std::shared_ptr<Acceptor_T> _acceptorPtr;
	std::shared_ptr<Formatter_T> _formatterPtr;
	std::shared_ptr<Writer_T> _writerPtr;

	boost::thread _objectThread;

	//is run in a different thread.
	//(every logger object has its own thread)
	//gets a message out of the queue and then calls acceptMessage, formatMessage, and writeString();
	//run by the object's thread
	void run()
	{
		while(!_shouldStop)
		{
			//blocks until there is a message
			std::shared_ptr<LogMessage> messagePtr = _inputQueue.Dequeue();


			if(!_shouldStop)
			{
				if((*_acceptorPtr)(messagePtr))
				{
					std::string messageText = (*_formatterPtr)(messagePtr);
					(*_writerPtr)(messageText);
				}
			}
		}
	}
public:

	LogOutput(std::shared_ptr<Acceptor_T> acceptorPtr, 	std::shared_ptr<Formatter_T> formatterPtr, std::shared_ptr<Writer_T> writerPtr)
	:_inputQueue(),
	 _shouldStop(false),
	 _acceptorPtr(acceptorPtr),
	 _formatterPtr(formatterPtr),
	 _writerPtr(writerPtr),
	 _objectThread(&LogOutput::run, this) //initialize thread IN THE CONSTRUCTOR!!!
		//MUAHAHAHAHAHAHAHAH
	{

	}

	//convinience constructor that default constructs all of the types
	//use this if all of the template arguments have no-argument constructors
	LogOutput()
	:_inputQueue(),
	 _shouldStop(false),
	 _acceptorPtr(std::make_shared<Acceptor_T>()),
	 _formatterPtr(std::make_shared<Formatter_T>()),
	 _writerPtr(std::make_shared<Writer_T>()),
	 _objectThread(&LogOutput::run, this) //initialize thread IN THE CONSTRUCTOR!!!
		//MUAHAHAHAHAHAHAHAH
	{

	}

	void enqueueMessage(std::shared_ptr<LogMessage> messagePtr)
	{
		_inputQueue.Enqueue(messagePtr, false);
	}

	virtual ~LogOutput()
	{
        //in this destructor we have to get the thread out of waiting at its condition_variable.
        //so we Enqueue an empty LogMessage
        _shouldStop = true;

        std::shared_ptr<LogMessage> message(new LogMessage({{"foo", "bar"}}));

        _inputQueue.Enqueue(message, true);

        _objectThread.join();
	}
};



#endif /* LOGOUTPUT_H_ */
