
set(HEADER_FILES
	SrcMain.h
	Parser.hpp
	DnaTranslator.hpp
	DnaPairwiseAlignment.hpp
)

set(SOURCE_FILES
	SrcMain.cpp
)

add_library(src ${SOURCE_FILES} ${HEADER_FILES})
