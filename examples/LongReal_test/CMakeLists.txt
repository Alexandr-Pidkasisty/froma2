set(PROGRAMM_NAME LongReal_test)			# Имя программы
add_executable("${PROGRAMM_NAME}" main.cpp)		# Сборка исполняемого файла с именем в переменной PROGRAMM_NAME из указанных модулей
target_link_libraries("${PROGRAMM_NAME}" froma2)	# Определение подключаемых компановщиком библиотек к исполняемому файлу
target_include_directories("${PROGRAMM_NAME}" PRIVATE ./include)	# Подключаем каталог с заголовочными файлами
add_custom_command(
			TARGET "${PROGRAMM_NAME}"
			POST_BUILD
			COMMAND cmake -E make_directory output) # Создание каталога для вывода результатов внешними командами
