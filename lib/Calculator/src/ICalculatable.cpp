#include "ICalculatable.hpp"

Addition::Addition(ICalcUptr left, ICalcUptr right)
{
    _left = std::move(left);
    _right = std::move(right);
}

Subtraction::Subtraction(ICalcUptr left, ICalcUptr right)
{
    _left = std::move(left);
    _right = std::move(right);
}

Multiplication::Multiplication(ICalcUptr left, ICalcUptr right)
{
    _left = std::move(left);
    _right = std::move(right);
}

Arcsin::Arcsin(ICalcUptr left)
{
    _left = std::move(left);
}

Arccos::Arccos(ICalcUptr left)
{
    _left = std::move(left);
}

double Addition::Calculate() const
{
    return _left->Calculate() + _right->Calculate();
}

double Subtraction::Calculate() const
{
    if (_right == nullptr)
        return -_left->Calculate();
    else
        return _left->Calculate() - _right->Calculate();
}

double Multiplication::Calculate() const
{
    return _left->Calculate() * _right->Calculate();
}

double Arcsin::Calculate() const
{
    double arg = _left->Calculate();
    if ((arg >= -1) && (arg <= 1))
        return asin(arg);
    else
        throw std::runtime_error("Bad argument: out of arcsin scope!");
}

double Arccos::Calculate() const
{
    double arg = _left->Calculate();
    if ((arg >= -1) && (arg <= 1))
        return acos(arg);
    else
        throw std::runtime_error("Bad argument: out of arccos scope!");
}

double Number::Calculate() const
{
    return data;
}
