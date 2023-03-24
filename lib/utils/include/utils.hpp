#pragma once

#include <getopt.h>
#include <iostream>
#include <tuple>
#include <string>

std::tuple<std::string, std::string, std::string, std::string> parseCommLineArgs(int argc, char* argv[]);