cmake_minimum_required(VERSION 3.5)
project(rkaexchange-csharp)

find_program(MCS mcs)
if(NOT MCS)
    message(FATAL_ERROR "Mono C# compiler (mcs) not found. Install mono-devel package.")
endif()

set(CSHARP_SOURCES
    Program.cs
    Edb.cs
    Errors.cs
    Inbuf.cs
)

add_custom_target(rkaexchange-csharp ALL
    COMMAND ${MCS} -out:rkaexchange.exe ${CSHARP_SOURCES}
    COMMENT "Building rkaexchange.exe with Mono C# compiler"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
