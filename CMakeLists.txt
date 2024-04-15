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

if(WIN32)
  set(Edit_WIN_SRC src/wintty.c)
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
    ${Edit_WIN_SRC}
)
target_compile_definitions(Edit PUBLIC LIBEDIT_STATIC)
target_include_directories(Edit PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Curses REQUIRED)

configure_file(config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)

build_root_exec(sh -c "src/makelist -h src/common.c > src/common.h")
build_root_exec(sh -c "src/makelist -h src/emacs.c  > src/emacs.h")
build_root_exec(sh -c "src/makelist -h src/vi.c > src/vi.h")
build_root_exec(sh -c "src/makelist -fh src/common.h src/emacs.h src/vi.h > src/fcns.h")
build_root_exec(sh -c "src/makelist -fc src/common.h src/emacs.h src/vi.h > src/func.h")
build_root_exec(sh -c "src/makelist -bh src/common.c src/emacs.c src/vi.c > src/help.h")

if (BUILD_ROOT_OS__MSYS)
    find_package(wcwidth REQUIRED)
    if (NOT EXISTS "${CYGDIR}clang64/lib/libregex.a")
    message(FATAL_ERROR "${CYGDIR}clang64/lib/libregex.a not found")
    endif()
    if (NOT EXISTS "${CYGDIR}clang64/lib/libtre.a")
    message(FATAL_ERROR "${CYGDIR}clang64/lib/libtre.a not found")
    endif()
    if (NOT EXISTS "${CYGDIR}clang64/lib/libintl.a")
    message(FATAL_ERROR "${CYGDIR}clang64/lib/libintl.a not found")
    endif()
    add_library(LLVM_STATIC_Regex UNKNOWN IMPORTED)
    set_target_properties(LLVM_STATIC_Regex PROPERTIES IMPORTED_LOCATION "${CYGDIR}clang64/lib/libregex.a")
    add_library(LLVM_STATIC_Tre UNKNOWN IMPORTED)
    set_target_properties(LLVM_STATIC_Tre PROPERTIES IMPORTED_LOCATION "${CYGDIR}clang64/lib/libtre.a")
    add_library(LLVM_STATIC_Intl UNKNOWN IMPORTED)
    set_target_properties(LLVM_STATIC_Intl PROPERTIES IMPORTED_LOCATION "${CYGDIR}clang64/lib/libintl.a")
    add_library(LLVM_STATIC_Iconv UNKNOWN IMPORTED)
    set_target_properties(LLVM_STATIC_Iconv PROPERTIES IMPORTED_LOCATION "${CYGDIR}clang64/lib/libiconv.a")
    target_link_libraries(Edit PUBLIC "${WCWIDTH_TARGET};${CURSES_TARGET};userenv.lib;LLVM_STATIC_Regex;LLVM_STATIC_Tre;LLVM_STATIC_Intl;LLVM_STATIC_Iconv")
elseif(BUILD_ROOT_OS__CYGWIN)
    # cygwin provides wcwidth, we assume it is implicit
    # cygwin provides regex, we assume it is implicit
    # cygwin provides systre, we assume it is implicit
    # cygwin provides intl, we assume it is implicit
    # cygwin provides iconv, we assume it is implicit
    target_link_libraries(Edit PUBLIC PROPERTIES INTERFACE_LINK_LIBRARIES "${CURSES_TARGET};userenv.lib")
else()
  # linux provides wcwidth, we assume it is implicit
  # linux provides regex, we assume it is implicit
  # linux provides systre, we assume it is implicit
  target_link_libraries(Edit PUBLIC PROPERTIES INTERFACE_LINK_LIBRARIES ${CURSES_TARGET})
endif()

add_executable(fileman examples/fileman.c)
target_link_libraries(fileman PUBLIC Edit)

if ()
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

install(TARGETS fileman
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/histedit.h DESTINATION "${INSTALL_INC_DIR}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/editline/readline.h DESTINATION "${INSTALL_INC_DIR}/editline")
