cmake_minimum_required(VERSION 3.20)
add_library(lfs SHARED luafilesystem/src/lfs.c)
target_include_directories(lfs PRIVATE ${CMAKE_SOURCE_DIR}/lua/src)
target_link_libraries(lfs liblua)

add_library(luasocket SHARED
    "luasocket/src/mime.c"
    "luasocket/src/compat.c"
    "luasocket/src/auxiliar.c"
    "luasocket/src/buffer.c"
    "luasocket/src/compat.c"
    "luasocket/src/except.c"
    "luasocket/src/inet.c"
    "luasocket/src/io.c"
    "luasocket/src/luasocket.c"
    "luasocket/src/options.c"
    "luasocket/src/select.c"
    "luasocket/src/tcp.c"
    "luasocket/src/timeout.c"
    "luasocket/src/udp.c"
    "luasocket/src/wsocket.c"
)
target_include_directories(luasocket PRIVATE ${CMAKE_SOURCE_DIR}/lua/src)
target_link_libraries(luasocket PRIVATE liblua ws2_32.lib)

add_library(luasec SHARED
    "luasec/src/config.c"
    "luasec/src/context.c"
    "luasec/src/ec.c"
    "luasec/src/luasocket/buffer.c"
    "luasec/src/luasocket/io.c"
    "luasec/src/luasocket/timeout.c"
    "luasec/src/luasocket/wsocket.c"
    "luasec/src/options.c"
    "luasec/src/ssl.c"
    "luasec/src/x509.c"
)
target_include_directories(luasec PRIVATE
    ${CMAKE_SOURCE_DIR}/lua/src
    ${CMAKE_CURRENT_SOURCE_DIR}/luasec/src
    include
)
target_link_libraries(luasec PRIVATE
    liblua ws2_32.lib
    "${CMAKE_SOURCE_DIR}/libs/openssl/libssl.lib" "${CMAKE_SOURCE_DIR}/libs/openssl/libcrypto.lib"
)
