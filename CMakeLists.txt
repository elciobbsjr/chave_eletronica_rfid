# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(chave_eletronica_rfid C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Adiciona o executável principal
add_executable(chave_eletronica_rfid
    chave_eletronica_rfid.c
    mfrc522.c
    inc/ssd1306_i2c.c
)

# Nome e versão
pico_set_program_name(chave_eletronica_rfid "chave_eletronica_rfid")
pico_set_program_version(chave_eletronica_rfid "0.1")

# Saída via USB (desabilita UART)
pico_enable_stdio_uart(chave_eletronica_rfid 0)
pico_enable_stdio_usb(chave_eletronica_rfid 1)

# Includes para o projeto
target_include_directories(chave_eletronica_rfid PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Linka com bibliotecas padrão
target_link_libraries(chave_eletronica_rfid
    pico_stdlib
    hardware_pwm
    hardware_spi
    hardware_i2c
)

# Geração de UF2, bin e map
pico_add_extra_outputs(chave_eletronica_rfid)
