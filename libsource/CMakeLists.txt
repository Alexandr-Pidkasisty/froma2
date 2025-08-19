# Добавляем файлы в проект статической библиотеки froma2
add_library(froma2 STATIC
	baseproject_lib.cpp
	baseproject_module.h
	common_values.hpp
	Impex_lib.cpp
	Impex_module.h
	LongReal_lib.cpp
	LongReal_module.h
	manufact_lib.cpp
	manufact_module.h
	progress_module.hpp
	serialization_module.hpp
	warehouse_lib.cpp
	warehouse_module.h)
# Добавляем текущий каталог для использования при компиляции зависимых целей от текущей
target_include_directories(froma2 INTERFACE .)