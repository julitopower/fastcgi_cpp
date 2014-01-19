# Look for the header file.
find_path(FCGI_INCLUDE_DIR NAMES fastcgi.h fcgi.h fcgio.h)

# Look for the library.
find_library(FCGI_LIBRARY NAMES fcgi)
find_library(FCGIPP_LIBRARY NAMES fcgi++)

# Handle the QUIETLY and REQUIRED arguments and set FCGI_FOUND to TRUE if all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FCGI DEFAULT_MSG FCGI_LIBRARY FCGI_INCLUDE_DIR)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FCGIPP DEFAULT_MSG FCGIPP_LIBRARY FCGI_INCLUDE_DIR)

# Copy the results to the output variables.
if(FCGI_FOUND AND FCGIPP_FOUND)
	set(FCGI_LIBRARIES ${FCGI_LIBRARY} ${FCGIPP_LIBRARY})
	set(FCGI_INCLUDE_DIRS ${FCGI_INCLUDE_DIR})
else()
	set(FCGI_LIBRARIES)
	set(FCGI_INCLUDE_DIRS)
endif()

mark_as_advanced(FCGI_INCLUDE_DIR FCGI_LIBRARY)
