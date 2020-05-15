# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
	imdb.h
)

set(SOURCE_FILES
	SrcMain.cpp
	imdb.cpp
)

if (APPLE)
	include_directories(/usr/local/include)
endif()

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})

if(UNIX)
	find_library(cpprestsdk-lib cpprest)
	find_package(Boost REQUIRED COMPONENTS random system thread filesystem chrono atomic date_time regex)
	find_package(OpenSSL 1.0.0 REQUIRED)
	target_link_libraries(src PRIVATE
		${cpprestsdk-lib}
		${Boost_LIBRARIES}
		OpenSSL::SSL
	)
else()
	find_package(cpprestsdk REQUIRED)
	target_link_libraries(src PRIVATE cpprestsdk::cpprest)
endif()
