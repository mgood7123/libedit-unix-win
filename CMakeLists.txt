cmake_minimum_required(VERSION 3.12)

project(libedit C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0075 NEW)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

macro (build_root_exec)
    message(STATUS "build_root_exec ARGN = \"${ARGN}\"")
    unset(command_list)
    unset(command_list CACHE)
    string(REGEX MATCHALL "([^\"' \r\n\t;\\]+|[\\].|\"([^\"\\]+|[\\].)*\")+( +([^\"' \r\n\t;\\]+|[\\].|\"([^\"\\]+|[\\].)*\")+)*" command_list "${ARGN}")
    unset(command_str)
    unset(command_str CACHE)
    foreach(ARG IN ITEMS ${command_list})
        if (command_str)
          string(APPEND command_str " ")
        endif()
        string(APPEND command_str "${ARG}")
    endforeach()
    execute_process(
        COMMAND ${command_list}
        COMMAND_ECHO STDOUT
        RESULT_VARIABLE EXEC_FAILED
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    if (EXEC_FAILED)
        message(FATAL_ERROR "failed to execute command: '${command_str}'")
    endif()
endmacro()

find_package(Curses REQUIRED)

configure_file(config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

build_root_exec(sh -c "src/makelist -h src/common.c > src/common.h")
build_root_exec(sh -c "src/makelist -h src/emacs.c  > src/emacs.h")
build_root_exec(sh -c "src/makelist -h src/vi.c > src/vi.h")
build_root_exec(sh -c "src/makelist -fh src/common.h src/emacs.h src/vi.h > src/fcns.h")
build_root_exec(sh -c "src/makelist -fc src/common.h src/emacs.h src/vi.h > src/func.h")
build_root_exec(sh -c "src/makelist -bh src/common.h src/emacs.h src/vi.h > src/help.h")

if (WIN32)
    find_package(wcwidth REQUIRED)
endif()

add_library(Edit STATIC
    src/chared.c
    src/chartype.c
    src/common.c
    src/el.c
    src/eln.c
    src/emacs.c
    src/filecomplete.c
    src/getline.c
    src/hist.c
    src/history.c
    src/historyn.c
    src/keymacro.c
    src/literal.c
    src/map.c
    src/parse.c
    src/prompt.c
    src/read.c
    src/readline.c
    src/reallocarr.c
    src/refresh.c
    src/search.c
    src/sig.c
    src/strlcat.c
    src/strlcpy.c
    src/terminal.c
    src/tokenizer.c
    src/tokenizern.c
    src/tty.c
    src/unvis.c
    src/vi.c
    src/vis.c
    src/wcsdup.c
)
target_compile_definitions(Edit PUBLIC LIBEDIT_STATIC)
target_include_directories(Edit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
if (WIN32)
    target_link_libraries(Edit PUBLIC ${WCWIDTH_TARGET} ${CURSES_TARGET} userenv.lib)
else()
    target_link_libraries(Edit PUBLIC ${WCWIDTH_TARGET} ${CURSES_TARGET})
endif()

add_executable(fileman examples/fileman.c)
target_link_libraries(fileman PUBLIC Edit)

add_executable(fuzz1 examples/fuzz1.c)
target_link_libraries(fuzz1 PUBLIC Edit)

add_executable(tc1 examples/tc1.c)
target_link_libraries(tc1 PUBLIC Edit)

add_executable(wtc1 examples/wtc1.c)
target_link_libraries(wtc1 PUBLIC Edit)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")

install(TARGETS Edit
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/histedit.h DESTINATION "${INSTALL_INC_DIR}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/editline/readline.h DESTINATION "${INSTALL_INC_DIR}/editline")
