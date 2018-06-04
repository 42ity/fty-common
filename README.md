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

### How to pass travis check
Travis use an old version of log4cplus, so to avoid any errors,   
add this two lines in the before_install section of the travis.yml file:  
<code>- sudo apt-get remove liblog4cplus-dev</code>  
<code>- sudo apt-get autoremove</code>
