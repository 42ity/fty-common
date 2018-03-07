# fty-common
Library providing :  
* Mutual methods and functions that can be used by any fty-agent 
* A logging system based on the [log4cplus 1.1-9](https://github.com/log4cplus/log4cplus/tree/1.1.x) utility. 

## How to build
To build fty-common project run:
```bash
./autogen.sh [clean]
./configure
make
make check # to run self-test
```
## How to use Log System
By default (no log configuration file), the logging system use the **BIOS_LOG_LEVEL** variable to set the log level of the agent
and logs a redirected to the stdout console.

This system set 6 levels for logs with in order of importance  (lowest to highest) : 
* **TRACE** Designates finer-grained informational events than the DEBUG.
* **DEBUG** Designates fine-grained informational events that are most useful to debug an application.
* **INFO**  Designates informational messages that highlight the progress of the application at coarse-grained level.
* **WARN**  Designates potentially harmful situations.
* **ERROR** Designates error events that might still allow the application to continue running.
* **FATAL** Designates very severe error events that will presumably lead the application to abort.  

With this matching between the values of BIOS_LOG_LEVEL and the log level set by default :  

|    BIOS_LOG_LEVEL   |     Log level    |  
| ------------------- | ---------------- |  
| LOG_CRIT            |     FATAL        |  
| LOG_ERR             |     ERROR        |  
| LOG_WARNING         |     WARN         |  
| LOG_INFO            |      INFO        |  
| LOG_DEBUG           |     DEBUG        |  
| Other               |    TRACE         |  

### For agents coded with C++ : 
In the main method of the cpp file, use this method :  
<code>void ManageFtyLog::setInstanceFtylog(std::string component, std::string configFile = "" )</code>

This method has two parameters : 
* component : name of the agent ex : fty-alert-list
* configFile (optional) : path to the log config file.

The Ftylog object is still accessible by using the function :  
<code>Ftylog * ManageFtyLog::getInstanceFtylog()</code>  

### For agents coded with C : 
In the main method of the c file, for initialize the log object, call this method :  
<code>void ftylog_setInstance(const char * component, const char * logConfigFile )</code>
 
Same parameters as the c++ method with configFile not optional.

And for get the Ftylog object, call <code>Ftylog * ftylog_getInstance()</code>  

### How to log
Use these macros to log any event in the agent (C++ or C) : 

* log_trace(...) : log a TRACE event.  
* log_debug(...) : log a DEBUG event.   
* log_info(...) : log a INFO event.  
* log_warning(...) : log a WARN event.  
* log_error(...) : log a ERROR event.  
* log_fatal(...) : log a FATAL event.  
  
The "..." section is a string follow by any parameters as the printf functions.

### How to format log
The logging system use the format from patternlayout of log4cplus (see http://log4cplus.sourceforge.net/docs/html/classlog4cplus_1_1PatternLayout.html).

If there is no configuration file, the default format is :  
<code>"%c [%t] -%-5p- %M (%l) %m%n"</code>

For example, with this code :  
<code>log_info(test, "This is a %s log test number %d", "info", 1);</code>

The log generated will be :  
<code>log-default-67358592 [140004551534400] -INFO - log_fty_log_test (src/log/fty_log.cc:481) This is a info log test number 1</code> 

In your system, you can set an environment variable named BIOS_LOG_PATTERN to set a format pattern for all agents using fty-common and if the agent does not use a log configuration file.

### Log configuration file
The agent can set a path to a log configuration file. The file use the syntax of a log4cplus configuration file (which is largely inspired from log4j syntax).
So for a fty-test agent, to redirect INFO messages to the console and ERROR messages in a file, 
the log configuration file will be : 

````
#Logger definition  
log4cplus.logger.fty-test=INFO, console, file  

#Console Definition  
log4cplus.appender.console=log4cplus::ConsoleAppender  
log4cplus.appender.console.layout=log4cplus::PatternLayout  
log4cplus.appender.console.layout.ConversionPattern=[%-5p][%d] %m%n  

#File definition  
log4cplus.appender.file=log4cplus::RollingFileAppender  
log4cplus.appender.file.File=/tmp/logging.txt  
log4cplus.appender.file.MaxFileSize=16MB  
log4cplus.appender.file.MaxBackupIndex=1  
log4cplus.appender.file.Threshold=ERROR  
log4cplus.appender.file.layout=log4cplus::PatternLayout  
log4cplus.appender.file.layout.ConversionPattern=[%-5p][%D{%Y/%m/%d %H:%M:%S:%q}][%-l][%t] %m%n
````

Note that the name after the "log4cplus.logger." string **MUST BE** the same as the "component"
parameter when you create a Ftylog object.  
  
Every minute, a small thread check if the log configuration file has been modified or not.  
If modified, the logging system reload the log configuration file.

The object where log events are redirected is call "appender".  
Log4cplus define several types of appenders :  

|    Type   |     Description    |  
| ------------------- | ---------------- |  
| AsyncAppender            |  Log events asynchronously.   |  
| CLFSAppender             |  Log object based on Microsoft Common Log File System API   |  
| ConsoleAppender         |   Log events in stdout         |  
| FileAppender            |   Log events in a file        |  
| DailyRollingFileAppender |  Log events in a file with backup after a period of time        |  
| RollingFileAppender |     Log events in file with backup when a size has been reached       |  
| Log4jUdpAppender |    Sends log events as Log4j XML to a remote log server.         |  
| NullAppender |  Never outputs a log event to any device.           |  
| Qt4DebugAppender |  Log object based on Qt4's qDebug(), qWarning() and qCritical() functions.  |  
| SocketAppender |  Log events to a remote log server, usually a socketNode.  |  
| SyslogAppender |  Log events to a remote syslog daemon.           |  

See [here](http://log4cplus.sourceforge.net/docs/html/classlog4cplus_1_1Appender.html) for more informations about appenders

### Verbose mode
For agent with a verbose mode, you can call the class method FtyLog::setVerboseMode()
(or ftylog_setVerboseMode(Ftylog* log) for C code) to change logging system : 

* It set (or overwrites if existing ) a ConsoleAppender object with the TRACE logging level with default format or 
    format defined by the BIOS_LOG_PATTERN environment variable.

### Utilities
The following class functions test if a log level is included in the log level of the agent :
* bool FtyLog::isLogTrace()
* bool FtyLog::isLogDebug()
* bool FtyLog::isLogInfo()
* bool FtyLog::isLogWarning()
* bool FtyLog::isLogError()
* bool FtyLog::isLogFatal()

And for C code : 
* bool ftylog_isLogTrace(Ftylog * log)
* bool ftylog_isLogDebug(Ftylog * log)
* bool ftylog_isLogInfo(Ftylog * log)
* bool ftylog_isLogWarning(Ftylog * log)
* bool ftylog_isLogError(Ftylog * log)
* bool ftylog_isLogFatal(Ftylog * log)

For example if the log level of the agent is INFO, the function isLogError() will return true and the function isLogDebug() will return false.

### Use for Test only 
The following methods change dynamically the logging level of the Ftylog object to a specific log level, so **only use it for testing**.

This class methods are : 
* void FtyLog::setLogLevelTrace()
* void FtyLog::setLogLevelDebug()
* void FtyLog::setLogLevelInfo()
* void FtyLog::setLogLevelWarning()
* void FtyLog::setLogLevelError()
* void FtyLog::setLogLevelFatal()

And for C code : 
* void ftylog_setLogLevelTrace(Ftylog * log)
* void ftylog_setLogLevelDebug(Ftylog * log)
* void ftylog_setLogLevelInfo(Ftylog * log)
* void ftylog_setLogLevelWarning(Ftylog * log)
* void ftylog_setLogLevelError(Ftylog * log)
* void ftylog_setLogLevelFatal(Ftylog * log)

## How to use Common functions
### For agent coded with C++ : 
In the main .h file add #include \<fty_common.h\> 

### For agents coded with C : 
In the main .h file add #include \<fty-common/common/fty_commons.h\> 

### Macro
The following macros are defined :  
 * #define STR(X) #X
 * #define streq(s1,s2) (!strcmp ((s1), (s2))) (if not already set)  

## How compile using fty-common  

### project.xml  
Add this bloc in the project.xml file :   

````
<use project = "fty-common" libname = "libfty_common" header="fty_common.h"
        repository = "https://github.com/42ity/fty-common.git"
        test = "fty_commmon_selftest" >
        <use project = "log4cplus" header = "log4cplus/logger.h"
        test = "appender_test" release="REL_1_1_2" repository="https://github.com/log4cplus/log4cplus.git" />
        <use project = "czmq"
            repository="https://github.com/42ity/czmq.git" release = "v3.0.2-FTY-master"
            min_major = "3" min_minor = "0" min_patch = "2" >
            <use project = "libzmq"
                repository="https://github.com/42ity/libzmq.git" release = "4.2.0-FTY-master" >
                <use project = "libsodium" prefix = "sodium"
                    repository = "https://github.com/42ity/libsodium.git"
                    release = "1.0.5-FTY-master"
                    test = "sodium_init" />
            </use>
        </use>

        <use project = "cxxtools" test="cxxtools::Utf8Codec::Utf8Codec" header="cxxtools/allocator.h"
            repository = "https://github.com/42ity/cxxtools.git"
            release = "2.2-FTY-master"
            />

        <use project = "libtntnet" optional = "1"
            repository = "https://github.com/42ity/tntnet.git"
            release = "2.2-FTY-master"
            />

        <use project = "tntdb"
            test="tntdb::Date::gmtime"
            repository = "https://github.com/42ity/tntdb.git"
            release = "1.3-FTY-master"
            builddir = "tntdb" />
    </use>
````  

The header value must be change from fty_common.h to fty-common/log/fty_log.h for C project.  
In this use section, remove the dependecy already needed byt the agent/library.

### How to pass travis check
Travis use an old version of log4cplus, so to avoid any errors,   
add this two lines in the before_install section of the travis.yml file:  
<code>- sudo apt-get remove liblog4cplus-dev</code>  
<code>- sudo apt-get autoremove</code>
