/*  =========================================================================
    fty_log - Log management

    Copyright (C) 2014 - 2018 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    =========================================================================
 */

/*
@header
    fty_log - Log management
@discuss
@end
 */
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fstream>
#include <typeinfo>
#include <thread>
#include <sstream>
#include <log4cplus/hierarchy.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/consoleappender.h>

#include "../../fty_common_classes.h"

using namespace log4cplus::helpers;

//constructor
Ftylog::Ftylog(std::string component, std::string configFile)
{
  init(component,configFile);
}

Ftylog::Ftylog()
{
    std::ostringstream threadId;
    threadId <<  std::this_thread::get_id();
    std::string name = "log-default-" + threadId.str();
    init(name);
}

void Ftylog::init(std::string component, std::string configFile)
{
  if (NULL != _watchConfigFile)
  {
    delete _watchConfigFile;
    _watchConfigFile = NULL;
  }
  _logger.shutdown();
  _agentName = component;
  _configFile = configFile;
  _layoutPattern = LOGPATTERN;

  //initialize log4cplus
  log4cplus::initialize();

  //Create logger
  auto log = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(component));
  _logger = log;

  //Get log level from bios and set to the logger
  //even if there is a log configuration file
  setLogLevelFromEnv();

  //Get pattern layout from env
  setPatternFromEnv();

  //load appenders
  loadAppenders();
}


//Clean objects in destructor
Ftylog::~Ftylog()
{
  if (NULL != _watchConfigFile)
  {
    delete _watchConfigFile;
    _watchConfigFile = NULL;
  }
  _logger.shutdown();
}

//getter
std::string Ftylog::getAgentName()
{
  return _agentName;
}

//setter
void Ftylog::setConfigFile(std::string file)
{
  _configFile = file;
  loadAppenders();
}

void Ftylog::change(std::string name,std::string configFile)
{
  init(name,configFile);
}

//Initialize from environment variables
void Ftylog::setLogLevelFromEnv()
{
  //By default, set TRACE level
  setLogLevelTrace();

  //get BIOS_LOG_LEVEL value and set correction logging level
  const char *varEnv = getenv("BIOS_LOG_LEVEL");
  if (varEnv && !std::string(varEnv).empty())
  {
    setLogLevelFromEnv(varEnv);
  }
}
void Ftylog::setPatternFromEnv()
{
  //Get BIOS_LOG_PATTERN for a default patternlayout
  const char * varEnv = getenv("BIOS_LOG_PATTERN");
  if (varEnv && !std::string(varEnv).empty())
  {
    _layoutPattern = varEnv;
  }
}

//Add a simple ConsoleAppender to the logger
void Ftylog::setConsoleAppender()
{
  _logger.removeAllAppenders();
  //create appender
  // Note: the first bool argument controls logging to stderr(true) as output stream
  SharedObjectPtr<log4cplus::Appender> append(new log4cplus::ConsoleAppender(true, true));
  //Create and affect layout
  append->setLayout(std::auto_ptr<log4cplus::Layout> (new log4cplus::PatternLayout(_layoutPattern)));
  append.get()->setName(LOG4CPLUS_TEXT("Console" + this->_agentName));

  //Add appender to logger
  _logger.addAppender(append);
}

//Switch the logging system to verbose
void Ftylog::setVeboseMode()
{
  //Save the loglevel of the logger
  log4cplus::LogLevel oldLevel = _logger.getLogLevel();
  //set log level of the logger to TRACE
  setLogLevelTrace();
  //Search if a console appender already exist
  for (log4cplus::SharedAppenderPtr & appenderPtr : _logger.getAllAppenders())
  {
    log4cplus::Appender & app = *appenderPtr;

    if (streq(typeid (app).name(), typeid (log4cplus::ConsoleAppender).name()))
    {
      //If any, remove it
      _logger.removeAppender(appenderPtr);
      break;
    }
  }

  //Set all remaining appenders with the old log level as threshold if not defined
  for (log4cplus::SharedAppenderPtr & appenderPtr : _logger.getAllAppenders())
  {
    log4cplus::Appender & app = *appenderPtr;
    if (app.getThreshold() == log4cplus::NOT_SET_LOG_LEVEL)
    {
      app.setThreshold(oldLevel);
    }
  }

  //create and add the appender
  SharedObjectPtr<log4cplus::Appender> append(new log4cplus::ConsoleAppender(false, true));
  //Create and affect layout
  append->setLayout(std::auto_ptr<log4cplus::Layout> (new log4cplus::PatternLayout(_layoutPattern)));
  append.get()->setName(LOG4CPLUS_TEXT("Verbose-" + this->_agentName));
  //Add verbose appender to logger
  _logger.addAppender(append);
}

//Set appenders from log config file if exist
// or set a basic ConsoleAppender
void Ftylog::loadAppenders()
{
  //by default, load console appenders
  setConsoleAppender();

  //If true, load file
  bool loadFile = false;

  //Stop the watch confile file thread if any
  if (NULL != _watchConfigFile)
  {
    delete _watchConfigFile;
    _watchConfigFile = NULL;
  }

  //if path to log config file
  if (!_configFile.empty())
  {
    //file can be accessed with read rights
    if (FILE * file = fopen(_configFile.c_str(), "r"))
    {
      fclose(file);
      loadFile = true;
    }
    else
    {
      log_error_log(this,"File %s can't be accessed with read rights",_configFile.c_str());
      _configFile = "";
    }
  }
  else
  {
    log_warning_log(this,"No log configuration file defined");
  }

  //if no file or file not valid, set default ConsoleAppender
  if (loadFile)
  {
    log_info_log(this,"Load Config file %s ",_configFile.c_str());

    //Remove previous appender
    _logger.removeAllAppenders();

    //Load the file
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(_configFile));
    //Start the thread watching the modification of the log config file
    _watchConfigFile = new log4cplus::ConfigureAndWatchThread(_configFile.c_str(), 60000);
  }
}

//Set the logging level corresponding to the BIOS_LOG_LEVEL value
void Ftylog::setLogLevelFromEnv(const char* level)
{
  //If empty string, set log level to TRACE
  if (std::string(level).empty())
  {
    setLogLevelTrace();
    return;
  }

  if (streq(level, "LOG_DEBUG") )
  {
    setLogLevelDebug();
  }
  else if (streq(level, "LOG_INFO"))
  {
    setLogLevelInfo();
  }
  else if (streq(level, "LOG_WARNING"))
  {
    setLogLevelWarning();
  }
  else if (streq(level, "LOG_ERR"))
  {
    setLogLevelError();
  }
  else if (streq(level, "LOG_CRIT"))
  {
    setLogLevelFatal();
  }
  else
  {
    //Set trace level by default
    setLogLevelTrace();
  }
}

//Set logger to a specific logging level
void Ftylog::setLogLevelTrace()
{
  _logger.setLogLevel(log4cplus::TRACE_LOG_LEVEL);
}

void Ftylog::setLogLevelDebug()
{
  _logger.setLogLevel(log4cplus::DEBUG_LOG_LEVEL);
}

void Ftylog::setLogLevelInfo()
{
  _logger.setLogLevel(log4cplus::INFO_LOG_LEVEL);
}

void Ftylog::setLogLevelWarning()
{
  _logger.setLogLevel(log4cplus::WARN_LOG_LEVEL);
}

void Ftylog::setLogLevelError()
{
  _logger.setLogLevel(log4cplus::ERROR_LOG_LEVEL);
}

void Ftylog::setLogLevelFatal()
{
  _logger.setLogLevel(log4cplus::FATAL_LOG_LEVEL);
}

//Return true if the logging level is include in the logger log level
bool Ftylog::isLogLevel(log4cplus::LogLevel level)
{
  return _logger.getLogLevel() <= level;
}

bool Ftylog::isLogTrace()
{
  return this->isLogLevel(log4cplus::TRACE_LOG_LEVEL);
}

bool Ftylog::isLogDebug()
{
  return this->isLogLevel(log4cplus::DEBUG_LOG_LEVEL);
}

bool Ftylog::isLogInfo()
{
  return this->isLogLevel(log4cplus::INFO_LOG_LEVEL);
}

bool Ftylog::isLogWarning()
{
  return this->isLogLevel(log4cplus::WARN_LOG_LEVEL);
}

bool Ftylog::isLogError()
{
  return this->isLogLevel(log4cplus::ERROR_LOG_LEVEL);
}

bool Ftylog::isLogFatal()
{
  return this->isLogLevel(log4cplus::FATAL_LOG_LEVEL);
}

//Call log4cplus system to print logs in logger appenders
void Ftylog::insertLog(log4cplus::LogLevel level, const char* file, int line,
                       const char* func, const char* format, va_list args)
{
  char *buffer;
  int r;

  //Check if the level of this log is included in the log level
  if (!isLogLevel(level))
  {
    return;
  }
  //Construct the main log message
  r = vasprintf(&buffer, format, args);
  if (r == -1)
  {
    fprintf(stderr, "[ERROR]: %s:%d (%s) can't allocate enough memory for message string: buffer", __FILE__, __LINE__, __func__);
    if (buffer)
    {
      free(buffer);
    }
    return;
  }

  if (buffer == NULL)
  {
    log_error_log(this, "Buffer == NULL");
    return;
  }

  //Give the printing job to log4cplus
  log4cplus::detail::macro_forced_log(_logger, level, LOG4CPLUS_TEXT(buffer), file, line, func);
  free(buffer);
}

void Ftylog::insertLog(log4cplus::LogLevel level, const char* file, int line,
                       const char* func, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  insertLog(level,file,line,func,format,args);
  va_end(args);
}

////////////////////////
//manageftylog section
////////////////////////

Ftylog ManageFtyLog::_ftylogdefault=Ftylog();

Ftylog* ManageFtyLog::getInstanceFtylog()
{
  return &_ftylogdefault;
}

void ManageFtyLog::setInstanceFtylog(std::string componentName, std::string logConfigFile )
{
  _ftylogdefault.change(componentName,logConfigFile);
}

////////////////////////
//Wrapper for C code use
////////////////////////

//construtor
Ftylog * ftylog_new(const char * component, const char * logConfigFile)
{
  return new Ftylog(std::string(component), std::string(logConfigFile));
}

//destructor
void ftylog_delete(Ftylog * log)
{
  delete log;
  log = NULL;
}

//setter
void ftylog_setConfigFile(Ftylog * log, const char * file)
{
  log->setConfigFile(std::string(file));
}

//Set the logger to a specific log level
void ftylog_setLogLevelTrace(Ftylog * log)
{
  log->setLogLevelTrace();
}

void ftylog_setLogLevelDebug(Ftylog * log)
{
  log->setLogLevelDebug();
}

void ftylog_setLogLevelInfo(Ftylog * log)
{
  log->setLogLevelInfo();
}

void ftylog_setLogLevelWarning(Ftylog * log)
{
  log->setLogLevelWarning();
}

void ftylog_setLogLevelError(Ftylog * log)
{
  log->setLogLevelError();
}

void ftylog_setLogLevelFatal(Ftylog * log)
{
  log->setLogLevelFatal();
}

//Check the log level
bool ftylog_isLogTrace(Ftylog * log)
{
  return log->isLogTrace();
}

bool ftylog_isLogDebug(Ftylog * log)
{
  return log->isLogDebug();
}

bool ftylog_isLogInfo(Ftylog * log)
{
  return log->isLogInfo();
}

bool ftylog_isLogWarning(Ftylog * log)
{
  return log->isLogWarning();
}

bool ftylog_isLogError(Ftylog * log)
{
  return log->isLogError();
}

bool ftylog_isLogFatal(Ftylog * log)
{
  return log->isLogTrace();
}

//Print log in logger appenders
void ftylog_insertLog(Ftylog * log, int level, const char* file, int line,
               const char* func, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  log->insertLog(level, file, line, func, format, args);
  va_end(args);
}

//Switch to verbose mode
void ftylog_setVeboseMode(Ftylog * log)
{
  log->setVeboseMode();
}

Ftylog * ftylog_getInstance()
{
  return ManageFtyLog::getInstanceFtylog();
}

void ftylog_setInstance(const char * component,const char * configFile)
{
  ManageFtyLog::setInstanceFtylog(std::string(component),std::string(configFile));
}

//Test function
void fty_common_log_fty_log_test(bool verbose)
{
  printf(" * fty_log \n");
  printf(" * Check default log \n");
  ManageFtyLog::getInstanceFtylog()->setLogLevelTrace();
  log_trace("This is a simple %s log with default logger","trace");
  log_debug("This is a simple %s log with default logger","debug");
  log_info("This is a simple %s log with default logger","info");
  log_warning("This is a simple %s log with default logger","warning");
  log_error("This is a simple %s log with default logger","error");
  log_fatal("This is a simple %s log with default logger","fatal");
  printf(" * Check default log : OK \n");



  printf(" * Check level test \n");
  ManageFtyLog::setInstanceFtylog("fty-log-agent");
  Ftylog* test = ManageFtyLog::getInstanceFtylog();

  test->setLogLevelTrace();
  log_trace_log(test, "This is a simple trace log");
  log_trace_log(test, "This is a %s log test number %d", "trace", 1);
  assert(test->isLogTrace());
  assert(test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelDebug();
  log_debug_log(test, "This is a simple debug log");
  log_debug_log(test, "This is a %s log test number %d", "debug", 1);
  assert(!test->isLogTrace());
  assert(test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelInfo();
  log_info_log(test, "This is a simple info log");
  log_info_log(test, "This is a %s log test number %d", "info", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelWarning();
  log_warning_log(test, "This is a simple warning log");
  log_warning_log(test, "This is a %s log test number %d", "warning", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelError();
  log_error_log(test, "This is a simple error log");
  log_error_log(test, "This is a %s log test number %d", "error", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(!test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelFatal();
  log_fatal_log(test, "This is a simple fatal log");
  log_fatal_log(test, "This is a %s log test number %d", "fatal", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(!test->isLogWarning());
  assert(!test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelTrace();
  printf(" * Check level test : OK \n");
  printf(" * Check log config file test\n");
  test->setConfigFile("./src/selftest-ro/not-a-valid-file.conf");
  test->setConfigFile("./src/selftest-ro/test-config.conf");

  log_info_log(test, "This is a simple info log");
  log_info_log(test, "This is a %s log test number %d", "info", 1);

  //file is created
  assert(access("./src/selftest-rw/logfile.log", F_OK) != -1);

  //this size is not 0
  std::ifstream file("./src/selftest-rw/logfile.log", std::ifstream::in | std::ifstream::binary);
  file.seekg(0, std::ios::end);
  int fileSize = file.tellg();

  assert(0 != fileSize);
  printf(" * Check log config file test : OK\n");

  printf(" * Check verbose \n");
  test->setVeboseMode();
  log_trace_log(test, "This is a verbose trace log");
  printf(" * Check verbose : OK \n");

  //delete the log file test
  remove("./src/selftest-rw/logfile.log");

  //  @selftest
  printf("OK\n");
}
