#include <iostream>

#include "ICalculatable.hpp"
#include "ExpressionParser.hpp"
#include "ExpressionTree.hpp"

int main(int argc, char *argv[])
{
    try
    {
        std::string expr = argv[1];
        ExpressionParser parser(expr);

        std::vector<std::string> postfix_tokens = parser.parseIntoPostfixForm();
        std::unique_ptr<ICalculatable> root = makeTree(postfix_tokens);

        if (root)
        {
            double calc_result = root->Calculate();
            std::cout << calc_result << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}
