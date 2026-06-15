cmake_minimum_required(VERSION 3.5)
project(rkaexchange-csharp)

find_program(MCS mcs)
if(NOT MCS)
    message(FATAL_ERROR "Mono C# compiler (mcs) not found. Install mono-devel package.")
endif()

find_program(MONO mono)
if(NOT MONO)
    message(FATAL_ERROR "Mono C# runner (mono) not found. Install mono-devel package.")
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

add_custom_target(test_pack
    COMMAND ${MONO} rkaexchange.exe pack test.asm
    COMMENT "Run rkaexchange.exe with Mono C# runner"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS rkaexchange-csharp
)

add_custom_target(test_unpack
    COMMAND ${MONO} rkaexchange.exe unpack test.rka
    COMMENT "Run rkaexchange.exe with Mono C# runner"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS rkaexchange-csharp
)
