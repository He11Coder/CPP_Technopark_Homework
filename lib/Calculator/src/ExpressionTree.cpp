#include "ExpressionTree.hpp"

const std::string UNARY_MINUS = "u";

ICalcUptr makeTree(const std::vector<std::string>& expr)
{
    if (expr.empty())
        return nullptr;

    std::stack<ICalcUptr> aux_stack;

    for(auto& token : expr)
    {
        ICalcUptr node;
        if ((token == "+") || (token == "-") || (token == "*"))
        {
            if (aux_stack.empty())
                throw std::runtime_error("Expression is not in a prefix form!");

            ICalcUptr right(std::move(aux_stack.top()));
            aux_stack.pop();
            ICalcUptr left(std::move(aux_stack.top()));
            aux_stack.pop();

            if (token == "+")
                node = ICalcUptr(new Addition(std::move(left), std::move(right)));
            else if (token == "-")
                node = ICalcUptr(new Subtraction(std::move(left), std::move(right)));
            else if (token == "*")
                node = ICalcUptr(new Multiplication(std::move(left), std::move(right)));

            aux_stack.push(std::move(node));
        }
        else if ((token == UNARY_MINUS) || (token == "asin") || (token == "acos"))
        {
            if (aux_stack.empty())
                throw std::runtime_error("Expression is not in a prefix form!");

            ICalcUptr left(std::move(aux_stack.top()));
            aux_stack.pop();

            if (token == UNARY_MINUS)
                node = ICalcUptr(new Subtraction(std::move(left)));
            else if (token == "asin")
                node = ICalcUptr(new Arcsin(std::move(left)));
            else if (token == "acos")
                node = ICalcUptr(new Arccos(std::move(left)));

            aux_stack.push(std::move(node));
        }
        else if (std::isdigit(token[0]))
            aux_stack.push(ICalcUptr(new Number(std::stoi(token))));
        else
            throw std::runtime_error("Unexpected symbol or invalid input!");
    }

    return ICalcUptr(std::move(aux_stack.top()));
}
