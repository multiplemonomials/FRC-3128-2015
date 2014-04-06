EzLogger -- It's easier to use than Boost::Log!
==================================

General-purpose logging program written by Jamie Smith


Manual -- Section 1 -- Most basic logging
-------------------------------------------------

In order to get your first log message through the library, you'll have to set up one thing, the output, first.

This library features a three-component output system.  Every output has an acceptor that
filters messages, a formatter that converts messages to strings, and a writer that outputs them to their final destination.

```cpp
#include <output/LogOutput.h>
#include <output/acceptors/BasicAcceptor.h>
#include <output/formatters/BasicFormatter.h>
#include <output/writers/BasicWriter.h>
```

First, create it:

```cpp
auto outputPtr = std::make_shared<LogOutput<BasicAcceptor, BasicFormatter, BasicWriter>>
(
  std::make_shared<BasicAcceptor>(),
  std::make_shared<BasicFormatter>(),
  std::make_shared<BasicWriter>()
);
```

Then, add it to the registry of active outputs (you'll need to include EzLogger/LogCore.h):

```cpp
LogCore::instance().addOutput("stdio", outputPtr);
```

You can have multiple outputs, and they are identified with a unique string, in this case, "stdio".

Now that the output is set up, you can log your first message:

```cpp
#include <EzLogger/LogMessage.h>
std::shared_ptr<LogMessage> logMessage(new LogMessage());
logMessage->stream() << "Hello World";
LogCore::instance().log(logMessage);
```

This will print "Hello World".
That is really quite a bit of code, however.  You can automate it with a macro:

```cpp
#define LOG_BASIC(args)
{ \/* note angle brackets that keep multiple LOG_BASIC's from interfering with each other */
std::shared_ptr<LogMessage> logMessage(new LogMessage());\
logMessage->stream() << args;\
LogCore::instance().log(logMessage);\
}
```

Now you can invoke it like this:

```cpp
LOG_BASIC("Hello World")
```

or, with multiple arguments, 

```cpp
LOG_BASIC("Hello " << "World")
```

Manual -- Section 2 -- Tags
---------------------------------------------

Let's consider one of the most common things you want to do with a logger: add a severity indicator.

LogMessages support tags, an easy way to categorize your log messages.
Add some tags in the LogMessage constructor:

```cpp
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"Severity", "Debug"}}));
```

This message now has one tag with key "Severity" and value "Debug"
All tag formatting is in the control of each output.  This means it's possible to have different outputs 
print different things based on the tag, but we'll get into that later.
Since BasicFormatter formats every tag as 
```
[<key>: <value>]
```
this will print out "[Severity: Debug] "
before the message.  

Don't want to have that "Severity: " printout in front of "Debug"?
I don't either, so I included JamiesPrettyLogOutput (in EzLogger/output/formatters/JamiesPrettyFormatter.h).
Use that instead of BasicFormatter in the LogOutput construction, and it will print "[Debug]" instead.

Let's consider another use case, where you want to add the time before a message.
There is a helper function in EzLogger/Tags.h that returns a string of
the current time, currentTime().  Here's a demo:

```cpp
std::shared_ptr<LogMessage> logMessage(new LogMessage({{"Severity", "Info"}, {"Time", currentTime()}}));
logMessage->stream() << "is the current time.";
LogCore::instance().log(logMessage);
```

This prints, "[Severity: Info] [Time: 05:58:35] is the current time."

NOTE: You have to be careful about what you put as a tag value.
The initializer list 
```cpp
{{"Line Number", __LINE__}}
```
is an
```cpp
initialzer_list<std::pair<std::string, int>>
```

NOT an 
```cpp
initializer_list<std::pair<std::string, std::string>>
```
which is what LogMessage::LogMessage() takes.

This is almost certainly the cause if you get the compiler error,
```
 "cannot find method LogMessage::LogMessage(<initializer_list>)"
 ```

Instead use, 
```cpp
{{"Line Number", std::to_string(__LINE__)}}
```
