cmake_minimum_required (VERSION 2.6)
project(Neuron)
set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11 -g")

add_executable (neurone Neuron_Main.cpp Neuron.cpp Network.cpp Excitatory.cpp Inhibitory.cpp)

