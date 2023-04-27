#include "ExpressionParser.hpp"

const std::string OPERATORS[] = { "+", "-", "*", "asin", "acos", "(", ")" };
const std::string UNARY_MINUS = "u";

int getOperatorsPriority(const std::string& op)
{
    if (op == "+") return 1;
    if (op == "-") return 1;
    if (op == "*") return 2;

    return -1;
}

std::string ExpressionParser::getToken()
{
    if (current_index >= data.size())
        return std::string();

    for (auto& op : OPERATORS)
    {
        if (data.substr(current_index, op.size()) == op)
        {
            int index_to_return = current_index;
            current_index += op.size();
            return data.substr(index_to_return, op.size());
        }
    }

    std::string token;
    while (std::isdigit(data[current_index]) && (current_index < data.size()))
        token.push_back(data[current_index++]);

    return token;
}

std::vector<std::string> ExpressionParser::parseIntoPostfixForm()
{
    std::stack<std::string> tokens;
    std::vector<std::string> result;

    std::string prev_token;
    std::string curr_token;
    while (current_index < data.size())
    {
        std::string curr_token = getToken();

        if (std::isdigit(curr_token[0]))
        {
            result.push_back(curr_token);
        }
        else if ((curr_token == "(") || (curr_token == "asin") || (curr_token == "acos") || 
                ((curr_token == "-") && (prev_token.empty() || ((prev_token != ")") && !std::isdigit(prev_token[0])) )))
        {
            if (curr_token == "-")
                tokens.push(UNARY_MINUS);
            else
                tokens.push(curr_token);
        }
        else if (curr_token == ")")
        {
            while (!tokens.empty() && (tokens.top() != "("))
            {
                result.push_back(tokens.top());
                tokens.pop();

                if (tokens.empty())
                    throw std::runtime_error("Expected ')'");
            }
            tokens.pop();
        }
        else if ((curr_token == "+") || (curr_token == "-") || (curr_token == "*"))
        {
            while (!tokens.empty() && ((tokens.top() == UNARY_MINUS) || (tokens.top() == "asin") || (tokens.top() == "acos") || 
                  (getOperatorsPriority(tokens.top()) >= getOperatorsPriority(curr_token))))
            {
                result.push_back(tokens.top());
                tokens.pop();
            }
            tokens.push(curr_token);
        }
        else
            throw std::runtime_error("Unexpected symbol or invalid input!");

        prev_token = curr_token;
    }

    while (!tokens.empty())
    {
        if (std::isdigit(tokens.top()[0]) || (tokens.top() == "("))
            throw std::runtime_error("Brackets are not matched!");
        else
        {
            result.push_back(tokens.top());
            tokens.pop();
        }
    }
    current_index = 0;

    return result;
}
