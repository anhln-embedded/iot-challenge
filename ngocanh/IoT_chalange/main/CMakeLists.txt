# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

#idf_component_register(
#    SRCS ./app_main.c ./app_driver.c        # list the source files of this component
#    INCLUDE_DIRS        # optional, add here public include directories
#    PRIV_INCLUDE_DIRS   # optional, add here private include directories
#    REQUIRES            # optional, list the public requirements (component names)
#    PRIV_REQUIRES       # optional, list the private requirements
#)
idf_component_register(SRCS  app_driver.c  app_priv.h app_main.cpp
                       INCLUDE_DIRS ".")