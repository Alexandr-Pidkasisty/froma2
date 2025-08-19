set(PROGRAMM_NAME ManufactItem_example)			# Имя программы
add_executable("${PROGRAMM_NAME}" main.cpp)		# Сборка исполняемого файла с именем в переменной PROGRAMM_NAME из указанных модулей
target_link_libraries("${PROGRAMM_NAME}" froma2)	# Определение подключаемых компановщиком библиотек к исполняемому файлу