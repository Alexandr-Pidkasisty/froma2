cmake_minimum_required(VERSION 3.31)			# Минимальная версия инструмента cmake

set(PROJECT_NAME froma2_examples)				# Присваиваем переменной PROJECT_NAME строковую константу
project(${PROJECT_NAME})						# Название проекта

#настраиваем места хранения скомпилированных файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

#сораняем путь до директории froma2
set(FROMA2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libsource")

# Добавление путей к примерам
add_subdirectory("${FROMA2_DIR}")
add_subdirectory(examples/Storage_stress_test)
add_subdirectory(examples/SKU_example)
add_subdirectory(examples/Storage_example)
add_subdirectory(examples/ManufactItem_example)