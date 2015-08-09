cmake_minimum_required (VERSION 2.6) 
project (mttf-multicore)

add_definitions(-Wall -std=c++0x)
add_definitions(-O3)

include_directories(../mttf-multicore-tc)


add_executable(mttf-multicore
				rainflow.cpp
				rainflow.h
                cycles.h
                cycles.cpp
                main.cpp)