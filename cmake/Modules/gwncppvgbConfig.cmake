INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GWNCPPVGB gwncppvgb)

FIND_PATH(
    GWNCPPVGB_INCLUDE_DIRS
    NAMES gwncppvgb/api.h
    HINTS $ENV{GWNCPPVGB_DIR}/include
        ${PC_GWNCPPVGB_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GWNCPPVGB_LIBRARIES
    NAMES gnuradio-gwncppvgb
    HINTS $ENV{GWNCPPVGB_DIR}/lib
        ${PC_GWNCPPVGB_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GWNCPPVGB DEFAULT_MSG GWNCPPVGB_LIBRARIES GWNCPPVGB_INCLUDE_DIRS)
MARK_AS_ADVANCED(GWNCPPVGB_LIBRARIES GWNCPPVGB_INCLUDE_DIRS)

