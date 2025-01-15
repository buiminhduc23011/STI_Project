set(PROJECT_NAME "SCADA_GUNZE")
set(CMAKE_CXX_STANDARD 17)
idf_component_register(SRCS "app_main.cpp"
                            "socketioclient_api.cpp"
                            "flag.c"
                            "eeprom.cpp"
                            "_uart.cpp"
                            "Websetup.c"
                            "Ethernet.c"
                            "Wifi.c"
                            "IO.c"
                            "ota.c"
                        INCLUDE_DIRS ".")
