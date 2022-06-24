cmake_minimum_required(VERSION 3.22)
project(master)

add_executable(master main.cpp NEAT/Network.h NEAT/Neuron.h NEAT/Network.cpp NEAT/Connection.cpp NEAT/Connection.h NEAT/Neuron.cpp NEAT/Neuron.fwd.h NEAT/Connection.fwd.h games/tictactoe/TicTacToe.cpp games/tictactoe/TicTacToe.h games/Game.h NEAT/Generation.cpp NEAT/Generation.h utils/JsonHelper.cpp utils/JsonHelper.h utils/HelperMethods.cpp utils/HelperMethods.h utils/SharedDefinitions.h NEAT/StructureMutator.cpp NEAT/StructureMutator.h NEAT/Spiecie.cpp NEAT/Spiecie.h NEAT/StandardNeat.cpp NEAT/StandardNeat.h NEAT/SimplifiedNeat.cpp NEAT/SimplifiedNeat.h)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(master PUBLIC OpenMP::OpenMP_CXX)
endif()

set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -std=c++20")
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")


set(GMAKE_CXX_COMPILER gcc)
set(GMAKE_CXX_STANDARD 20)
set(GMAKE_CXX_FLAGS_DEBUG "-g")
set(GMAKE_CXX_FLAGS_RELEASE "-O3 -fopenmp -std=gnu++20")

