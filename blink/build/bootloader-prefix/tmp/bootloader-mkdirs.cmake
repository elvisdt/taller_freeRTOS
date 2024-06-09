# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/esp-idf/v5.2.1/esp-idf-v5.2.1/components/bootloader/subproject"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/tmp"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/src/bootloader-stamp"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/src"
  "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/EJD-FILES/TALLERES/FREERTOS-IDF/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
