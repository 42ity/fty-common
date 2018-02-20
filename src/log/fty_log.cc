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
#include <sys/syslog.h>
#include <unistd.h>
#include <fstream>
#include <typeinfo>
#include <log4cplus/hierarchy.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/loglevel.h>
#include <log4cplus/logger.h>
#include <log4cplus/consoleappender.h>

#include "../fty_common_classes.h"

using namespace log4cplus::helpers;
//constructor

Ftylog::Ftylog(std::string component, std::string configFile)
{
  _agentName = component;
  _configFile = configFile;
  _watchConfigFile = NULL;
  _layoutPattern = LOGPATTERN;

  //initialize log4cplus
  log4cplus::initialize();

  //Create logger
  auto log = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(component));
  _logger = log;

  //Get log level from bios and set to the logger
  setVarEnv();

  //load appenders
  loadAppenders();

}

//Clean object in destructor

Ftylog::~Ftylog()
{
  if (NULL != _watchConfigFile)
  {
    delete _watchConfigFile;
    _watchConfigFile = NULL;
  }
  _logger.shutdown();
}

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

//Log functions

void Ftylog::setVarEnv()
{
  //By default, set TRACE level
  setLogLevelTrace();

  const char *varEnv = getenv("BIOS_LOG_LEVEL");
  if (varEnv && !std::string(varEnv).empty())
  {
    setLogLevelFromEnv(varEnv);
  }

  varEnv = getenv("BIOS_LOG_PATTERN");
  if (varEnv && !std::string(varEnv).empty())
  {
    _layoutPattern = varEnv;
  }
}

void Ftylog::setConsoleAppender()
{
  _logger.removeAllAppenders();
  //create appender
  SharedObjectPtr<log4cplus::Appender> append(new log4cplus::ConsoleAppender(false, true));
  //Create and affect layout
  append->setLayout(std::auto_ptr<log4cplus::Layout> (new log4cplus::PatternLayout(_layoutPattern)));
  append.get()->setName(LOG4CPLUS_TEXT("Console" + this->_agentName));

  //Add appender to logger
  _logger.addAppender(append);
}

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

void Ftylog::loadAppenders()
{
  bool loadDefault = 0;

  //Remove previous appender
  _logger.removeAllAppenders();
  //Stop the watch confile file thread if any 
  if (NULL != _watchConfigFile)
  {
    delete _watchConfigFile;
    _watchConfigFile = NULL;
  }

  //if no path return 1
  if (_configFile.empty())
  {
    loadDefault = 1;
  }

  //file can be access with read
  if (!loadDefault)
  {
    //file can be accessed with read rights
    if (FILE * file = fopen(_configFile.c_str(), "r"))
    {
      fclose(file);
    }
    else
    {
      //Print a log in the stderr
      fprintf(stderr, "[ERROR]: %s:%d (%s) can't read file %s \n", __FILE__, __LINE__, __func__, _configFile.c_str());
      loadDefault = 1;
    }
  }
  //Load the file
  if (!loadDefault)
  {
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(_configFile));
  }

  //if no appender configured for this logger, set the console appenders
  if (loadDefault || 0 == _logger.getAllAppenders().size())
  {
    setConsoleAppender();
  }
  else
  {
    _watchConfigFile = new log4cplus::ConfigureAndWatchThread(_configFile.c_str(), 10000);
  }
}

void Ftylog::setLogLevelFromEnv(const char* level)
{
  //IF empty string, set log level to TRACE
  if (std::string(level).empty())
  {
    setLogLevelTrace();
    return;
  }
  //Set trace level by default
  setLogLevelTrace();


  if (streq(level, STR(LOG_DEBUG)) == 0)
  {
    setLogLevelDebug();
  }
  else if (streq(level, STR(LOG_INFO)) == 0)
  {
    setLogLevelInfo();
  }
  else if (streq(level, STR(LOG_WARNING)) == 0)
  {
    setLogLevelWarning();
  }
  else if (streq(level, STR(LOG_ERR)) == 0)
  {
    setLogLevelError();
  }
  else if (streq(level, STR(LOG_CRIT)) == 0)
  {
    setLogLevelFatal();
  }
  else
  {
    //Set trace level by default
    setLogLevelTrace();
  }
}

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

void Ftylog::insertLog(log4cplus::LogLevel level,
                       const char* file, int line,
                       const char* func, const char* format, ...)
{
  va_list args;
  va_start(args, format);

  char *buffer;
  int r;

  //Check if the level of this log is included in the log level
  if (!isLogLevel(level))
  {
    va_end(args);
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
    va_end(args);
    return;
  }

  if (buffer == NULL)
  {
    fprintf(stderr, "Buffer == NULL\n");
    va_end(args);
    return;
  }

  //Give the printing job to log4cplus
  log4cplus::detail::macro_forced_log(_logger, level, LOG4CPLUS_TEXT(buffer), file, line, func);

  free(buffer);
  va_end(args);
}

////////////////////////
//Wrapper for C code use
////////////////////////
//construtor

Ftylog * new_ftylog(const char * component, const char * logConfigFile)
{
  return new Ftylog(std::string(component), std::string(logConfigFile));
}
//destructor

void delete_ftylog(Ftylog * log)
{
  delete log;
  log = NULL;
}


//setter

void setConfigFile(Ftylog * log, const char * file)
{
  log->setConfigFile(std::string(file));
}

//Set the logger to a specific log level

void setLogLevelTrace(Ftylog * log)
{
  log->setLogLevelTrace();
}

void setLogLevelDebug(Ftylog * log)
{
  log->setLogLevelDebug();
}

void setLogLevelInfo(Ftylog * log)
{
  log->setLogLevelInfo();
}

void setLogLevelWarning(Ftylog * log)
{
  log->setLogLevelWarning();
}

void setLogLevelError(Ftylog * log)
{
  log->setLogLevelError();
}

void setLogLevelFatal(Ftylog * log)
{
  log->setLogLevelFatal();
}

//Check the log level 

bool isLogTrace(Ftylog * log)
{
  return log->isLogTrace();
}

bool isLogDebug(Ftylog * log)
{
  return log->isLogDebug();
}

bool isLogInfo(Ftylog * log)
{
  return log->isLogInfo();
}

bool isLogWarning(Ftylog * log)
{
  return log->isLogWarning();
}

bool isLogError(Ftylog * log)
{
  return log->isLogError();
}

bool isLogFatal(Ftylog * log)
{
  return log->isLogTrace();
}

void insertLog(Ftylog * log, int level, const char* file, int line,
               const char* func, const char* format, ...)
{
  va_list args;
  va_start(args, format);
  log->insertLog(level, file, line, func, format, args);
  va_end(args);
}

void setVeboseMode(Ftylog * log)
{
  log->setVeboseMode();
}

//Test function

void log_fty_log_test(bool verbose)
{
  printf(" * fty_log \n");
  Ftylog* test = new Ftylog("fty-log-agent", "");

  printf(" * Check level test \n");

  test->setLogLevelTrace();
  log_trace(test, "This is a simple trace log");
  log_trace(test, "This is a %s log test number %d", "trace", 1);
  assert(test->isLogTrace());
  assert(test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelDebug();
  log_debug(test, "This is a simple debug log");
  log_debug(test, "This is a %s log test number %d", "debug", 1);
  assert(!test->isLogTrace());
  assert(test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelInfo();
  log_info(test, "This is a simple info log");
  log_info(test, "This is a %s log test number %d", "info", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelWarning();
  log_warning(test, "This is a simple warning log");
  log_warning(test, "This is a %s log test number %d", "warning", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelError();
  log_error(test, "This is a simple error log");
  log_error(test, "This is a %s log test number %d", "error", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(!test->isLogWarning());
  assert(test->isLogError());
  assert(test->isLogFatal());

  test->setLogLevelFatal();
  log_fatal(test, "This is a simple fatal log");
  log_fatal(test, "This is a %s log test number %d", "fatal", 1);
  assert(!test->isLogTrace());
  assert(!test->isLogDebug());
  assert(!test->isLogInfo());
  assert(!test->isLogWarning());
  assert(!test->isLogError());
  assert(test->isLogFatal());

  printf(" * Check level test : OK \n");
  printf(" * Check log config file test\n");
  test->setConfigFile("./src/selftest-ro/test-config.conf");

  log_info(test, "This is a simple info log");
  log_info(test, "This is a %s log test number %d", "info", 1);

  //file is created
  assert(access("./src/selftest-rw/logfile.log", F_OK) != -1);

  //his size is not 0
  std::ifstream file("./src/selftest-rw/logfile.log", std::ifstream::in | std::ifstream::binary);
  file.seekg(0, std::ios::end);
  int fileSize = file.tellg();

  assert(0 != fileSize);
  printf(" * Check log config file test : OK\n");

  printf(" * Check verbose \n");
  test->setVeboseMode();
  log_trace(test, "This is a verbose trace log");
  printf(" * Check verbose : OK \n");

  //delete the log file test
  remove("./src/selftest-rw/logfile.log");

  delete test;
  test = NULL;

  //  @selftest
  printf("OK\n");
}
