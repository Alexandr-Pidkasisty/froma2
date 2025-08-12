add_executable(SKU_example main.cpp)		# Сборка исполняемого файла с именем SKU_example из указанных модулей
include_directories("${FROMA2_DIR}")		# Добавляет указанные каталоги к списку каталогов для поиска компилятором
target_link_libraries(SKU_example froma2)	# Определение подключаемых компановщиком библиотек к исполняемому файлу