# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
   SrcMain.h
   sha1.h
   HashTable.hpp
)

set(SOURCE_FILES
   SrcMain.cpp
   sha1.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
