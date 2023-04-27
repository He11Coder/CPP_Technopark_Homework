add_library(Calculator src/ICalculatable.cpp src/ExpressionParser.cpp src/ExpressionTree.cpp)
target_include_directories(Calculator PUBLIC include)
