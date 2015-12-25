# network module
find_package(Qt5Network)
add_definitions(
        ${Qt5Network_DEFINITIONS}
)
set(Qt_INCLUDE
        ${Qt5Network_INCLUDE_DIRS}
)

# quadro library
find_package(Quadro)
include_directories("${QUADRO_INCLUDE_DIRS}")
