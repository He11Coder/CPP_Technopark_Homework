#pragma once

#include "ICalculatable.hpp"

#include <vector>
#include <string>
#include <memory>
#include <stack>

class ExpressionTreeNode;
using ICalcUptr = std::unique_ptr<ICalculatable>;

std::unique_ptr<ICalculatable> makeTree(const std::vector<std::string>& expr);
