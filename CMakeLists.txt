# Добавляем файлы библиотеки в проект статической библиотеки froma2
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
	serialization_module.hpp
	warehouse_lib.cpp
	warehouse_module.h)