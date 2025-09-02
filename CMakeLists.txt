cmake_minimum_required(VERSION 3.31)	# Минимальная версия инструмента CMake
set(PROJECT_NAME froma2_examples)	# Присваиваем переменной PROJECT_NAME строковую константу
project(${PROJECT_NAME})		# Название проекта

# Установка стандарта для компилятора
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#настраиваем места хранения скомпилированных файлов
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

# Компилятору использовать опцию -fconcepts
add_compile_options(-fconcepts)

# Добавление подпроектов
add_subdirectory(libsource)
add_subdirectory(examples/Storage_stress_test)
add_subdirectory(examples/SKU_example)
add_subdirectory(examples/Storage_example)
add_subdirectory(examples/ManufactItem_example)
add_subdirectory(examples/Manufactory_example)
add_subdirectory(examples/Manufactory_stress_test)
add_subdirectory(examples/Impex_example)