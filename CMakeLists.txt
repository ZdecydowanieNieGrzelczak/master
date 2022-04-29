cmake_minimum_required(VERSION 3.22)
project(master)

set(CMAKE_CXX_STANDARD 20)

add_executable(master main.cpp Network.h Neuron.h Network.cpp Connection.cpp Connection.h Neuron.cpp Neuron.fwd.h Connection.fwd.h games/tictactoe/TicTacToe.cpp games/tictactoe/TicTacToe.h games/Game.h)
