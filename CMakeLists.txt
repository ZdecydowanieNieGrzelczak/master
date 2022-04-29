cmake_minimum_required(VERSION 3.22)
project(master)

set(CMAKE_CXX_STANDARD 20)

add_executable(master main.cpp NEAT/Network.h NEAT/Neuron.h NEAT/Network.cpp NEAT/Connection.cpp NEAT/Connection.h NEAT/Neuron.cpp NEAT/Neuron.fwd.h NEAT/Connection.fwd.h games/tictactoe/TicTacToe.cpp games/tictactoe/TicTacToe.h games/Game.h NEAT/Generation.cpp NEAT/Generation.h)
