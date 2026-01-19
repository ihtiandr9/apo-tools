set(TOOLS_DIR "${BUILD_DIR}/tools")

set(MINGW_DISTR "${TOOLS_DIR}/tdm-gcc-10.3.0.exe")
set(MINGW_DIR "${TOOLS_DIR}/TDM-GCC-32/bin")
set(MINGW_URL "http://ihtiandr9.fvds.ru/tools/tdm-gcc-10.3.0.exe")

# Проверяем существование файла и загружаем при необходимости

if(NOT EXISTS ${MINGW_DIR})

    # Создаем все необходимые директории
    file(MAKE_DIRECTORY "${MINGW_DIR}")
    
    # Загружаем архив если его нет
    if(NOT EXISTS ${MINGW_DISTR})
        message(STATUS "Downloading ${MINGW_URL} to ${MINGW_DISTR}")
        file(DOWNLOAD ${MINGW_URL} ${MINGW_DISTR} 
            STATUS download_status
            SHOW_PROGRESS
        )
        list(GET download_status 0 status_code)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "Download failed with status: ${download_status}")
        else()
            message(NOTICE "You should manually install TDM-GCC-32, running ${MINGW_DISTR}")
        endif()
    endif()
    
endif()
