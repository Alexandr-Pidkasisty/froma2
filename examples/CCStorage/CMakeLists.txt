set(PROGRAMM_NAME CCStorage)						# Имя программы
file(GLOB hdrs "include/*.h")						# Ищет файлы в текущей директории, подходящие регулярному выражению и
file(GLOB srcs "src/*.cpp")                         # сохраняет результат в переменные hdrs, srcs

# Определение переменных с путями
SET(DIR_INPUTDATA ${CMAKE_CURRENT_BINARY_DIR}/inputdata)
SET(DIR_OUTPUTDATA ${CMAKE_CURRENT_BINARY_DIR}/outputdata)
SET(DIR_TEMP ${CMAKE_CURRENT_BINARY_DIR}/temp)

# Определение конфигурационного файла
configure_file(pathes.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/pathes.h @ONLY)

add_executable("${PROGRAMM_NAME}" main.cpp ${hdrs} ${srcs}) # Сборка исполняемого файла с именем в переменной PROGRAMM_NAME из указанных модулей
target_link_libraries("${PROGRAMM_NAME}" froma2)			# Определение подключаемых компановщиком библиотек к исполняемому файлу
target_include_directories("${PROGRAMM_NAME}" PRIVATE ./include)    # Подключаем каталог с заголовочными файлами

# Создание рабочих папок
add_custom_command(
			TARGET "${PROGRAMM_NAME}"
			POST_BUILD
			COMMAND cmake -E make_directory outputdata inputdata temp)
# Копирование исходных данных в папку для исходных данных
add_custom_command(
			TARGET "${PROGRAMM_NAME}"
			POST_BUILD
			COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/inputdata ${DIR_INPUTDATA})
