#pragma once

#include <getopt.h>
#include <iostream>
#include <tuple>
#include <string>
#include <vector>

struct Args
{
    std::string title_file_name;
    std::string rating_file_name;
    std::string info_file_name;
    std::string duration;
};

Args parseCommLineArgs(int argc, char* argv[]);

std::vector<std::string> getFirstInPairs(std::vector<std::pair<std::string, double>> pairs);
