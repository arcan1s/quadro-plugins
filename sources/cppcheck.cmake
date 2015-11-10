# additional target to perform cppcheck run, requires cppcheck

# get all project files
# HACK this workaround is required to avoid qml files checking ^_^
file(GLOB_RECURSE ALL_SOURCE_FILES "*.cpp" "*.h")

add_custom_target(
        "cppcheck"
        COMMAND "${CPPCHECK_EXECUTABLE}"
        "--enable=warning,performance,portability,information,missingInclude"
        "--std=c++11"
        "--library=qt.cfg"
        "--template=\"[{severity}][{id}] {message} {callstack} \(On {file}:{line}\)\""
        "--verbose"
        "--quiet"
        ${ALL_SOURCE_FILES}
)
