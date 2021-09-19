# CMakeList.txt: проект CMake для ThreadPool; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set(ATOMIC TRUE)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (ThreadPoolProject "main.cpp")

set(CMAKE_CXX_STANDARD 17)

add_library(ThreadPool STATIC "ThreadPool.cpp" "ThreadPool.h" "functionwrapper.h" "${CMAKE_SOURCE_DIR}/ThreadPool/threadsafe_queue/atomic_threadsafe_queue/threadsafe_queue.cpp")

if(${ATOMIC})
	set(threadsafe_queue_directory "atomic_threadsafe_queue")
else()
	set(threadsafe_queue_directory "mutex_threadsafe_queue")
endif()

target_sources(
		ThreadPool
		INTERFACE "${CMAKE_SOURCE_DIR}/ThreadPool/threadsafe_queue/${threadsafe_queue_directory}/threadsafe_queue.h"
		PRIVATE "${CMAKE_SOURCE_DIR}/ThreadPool/threadsafe_queue/${threadsafe_queue_directory}/threadsafe_queue.cpp"
		PRIVATE "threadsafe_queue/misc.cpp"
)
include_directories("${CMAKE_SOURCE_DIR}/ThreadPool/threadsafe_queue/${threadsafe_queue_directory}/")

target_link_libraries(ThreadPoolProject ThreadPool)
# TODO: Добавьте тесты и целевые объекты, если это необходимо.
