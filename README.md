# fty-common
Library providing :  
* Mutual methods and functions that can be used by any fty-agent
  * Common utilities
  * Data base common methods
  * Web helper

## How to build
To build fty-common project run:
```bash
./autogen.sh [clean]
./configure
make
make check # to run self-test
```

The header value must be change from fty_common.h to fty-common/log/fty_log.h for C project.  
In this use section, remove the dependecy already needed byt the agent/library.

### How to pass travis check
Travis use an old version of log4cplus, so to avoid any errors,   
add this two lines in the before_install section of the travis.yml file:  
<code>- sudo apt-get remove liblog4cplus-dev</code>  
<code>- sudo apt-get autoremove</code>
