# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
	Parser.hpp
	Ops.h
	Machine.h
	Exceptions.h
)

set(SOURCE_FILES
	SrcMain.cpp
	Ops.cpp
	Machine.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
