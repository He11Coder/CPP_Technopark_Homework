#pragma once

#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <stdexcept>

const char SPACE = ' ';

class ExpressionParser
{
private:

    std::string data;
    int current_index;

    std::string getToken();

public:

    ExpressionParser() : current_index(0)
    {
    }

    explicit ExpressionParser(const std::string& expression) : data(expression), current_index(0)
    {
        data.erase(std::remove(data.begin(), data.end(), SPACE), data.end());
    }

    std::vector<std::string> parseIntoPostfixForm();
};
