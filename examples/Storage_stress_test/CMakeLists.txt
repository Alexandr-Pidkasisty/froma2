add_executable(Storage_stress_test main.cpp)		# Сборка исполняемого файла с именем Storage_stress_test из указанных модулей
include_directories("${FROMA2_DIR}")				# Добавляет указанные каталоги к списку каталогов для поиска компилятором
target_link_libraries(Storage_stress_test froma2)	# Определение подключаемых компановщиком библиотек к исполняемому файлу