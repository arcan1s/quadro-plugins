#ifndef VERSION_H
#define VERSION_H

// information
#define NAME "Quadro Plugins"
#define VERSION "0.9.0"
#define COMMIT_SHA "8726359"
#define AUTHOR "Evgeniy Alekseev"
#define TRANSLATORS ""
#define EMAIL "esalexeev@gmail.com"
#define LICENSE "GPLv3"
#define TRDPARTY_LICENSE "tasks,BSD,https://github.com/mhogomchungu/tasks"

// links
#define HOMEPAGE "https://arcanis.name/projects/quadro/"
#define REPOSITORY "https://github.com/arcan1s/quadro-core"
#define BUGTRACKER "https://github.com/arcan1s/quadro-core/issues"
#define TRANSLATION "https://github.com/arcan1s/quadro-core/issues/1"
#define AUR_PACKAGES "https://aur.archlinux.org/pkgbase/quadro-core/"

// build information
#define BUILD_DATE "2017-08-17 15:01"
#define DATE "2015-2017"

// cmake properties
#define CMAKE_BUILD_TYPE "Debug"
#define CMAKE_CXX_COMPILER "/usr/bin/clang++"
#define CMAKE_CXX_FLAGS "-Wall -std=c++11 -stdlib=libc++"
#define CMAKE_CXX_FLAGS_DEBUG "-g -O0"
#define CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG"
#define CMAKE_CXX_FLAGS_OPTIMIZATION "-Ofast -DNDEBUG"
#define CMAKE_DEFINITIONS                                                      \
    "QT_NETWORK_LIB;QT_CORE_LIB;QT_CORE_LIB;QT_DBUS_LIB;QT_CORE_LIB;QT_"       \
    "WIDGETS_LIB;QT_GUI_LIB;QT_CORE_LIB"
#define CMAKE_INSTALL_PREFIX "/usr"
#define CMAKE_SHARED_LINKER_FLAGS "-lc++abi"
// components
#define BUILD_DEB_PACKAGE "OFF"
#define BUILD_RPM_PACKAGE "OFF"
#define CLANGFORMAT_EXECUTABLE "/usr/bin/clang-format"
#define CPPCHECK_EXECUTABLE "/usr/bin/cppcheck"

#endif /* VERSION_H */
