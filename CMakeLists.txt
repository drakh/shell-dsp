cmake_minimum_required(VERSION 2.8)

add_definitions(
  -std=c++0x
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
  -Wall -Wextra -Werror=return-type
  ${GNUTLS_DEFINITIONS})

add_subdirectory(shell)
