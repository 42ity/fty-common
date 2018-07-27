# fty-common
Library providing :  
* Mutual methods and functions that can be used by any fty-agent 

## How to build
To build fty-common project run:
```bash
./autogen.sh [clean]
./configure
make
make check # to run self-test
```
## How to use Common functions
### For agent coded with C++ : 
In the main .h file add #include \<fty_common.h\> 

### For agents coded with C : 
In the main .h file add #include \<fty_common_base.h\> 

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
    release = "master" >

    <use project = "fty-common-logging" libname = "libfty_common_logging" header="fty_log.h"
        repository = "https://github.com/42ity/fty-common-logging.git"
        release = "master"
        test = "fty_common_logging_selftest" >

        <use project = "log4cplus" header = "log4cplus/logger.h" test = "appender_test"
            repository = "https://github.com/42ity/log4cplus.git"
            release = "1.1.2-FTY-master"
            />
    </use>

</use>
````  

The header value must be change from fty_common.h to fty_common_base.h for C project.  
In this use section, remove the dependecy already needed byt the agent/library.
