#pragma once

#include <cmath>
#include <stdexcept>
#include <memory>

class ICalculatable;
using ICalcUptr = std::unique_ptr<ICalculatable>;

class ICalculatable
{
public:

    virtual double Calculate() const = 0;

    virtual ~ICalculatable()
    {
    }
};

class Addition : public ICalculatable
{
private:

    std::unique_ptr<ICalculatable> _left;
    std::unique_ptr<ICalculatable> _right;

public:

    Addition(ICalcUptr left, ICalcUptr right);

    double Calculate() const override;
};

class Subtraction : public ICalculatable
{
private:

    std::unique_ptr<ICalculatable> _left;
    std::unique_ptr<ICalculatable> _right;

public:

    Subtraction(ICalcUptr left, ICalcUptr right = nullptr);

    double Calculate() const override;
};

class Multiplication : public ICalculatable
{
private:

    std::unique_ptr<ICalculatable> _left;
    std::unique_ptr<ICalculatable> _right;
 
public:

    Multiplication(ICalcUptr left, ICalcUptr right);

    double Calculate() const override;
};

class Arcsin : public ICalculatable
{
private:

    std::unique_ptr<ICalculatable> _left;

public:

    explicit Arcsin(ICalcUptr left);

    double Calculate() const override;
};

class Arccos : public ICalculatable
{
private:

    std::unique_ptr<ICalculatable> _left;

public:

    explicit Arccos(ICalcUptr left);

    double Calculate() const override;
};

class Number : public ICalculatable
{
private:

    double data;

public:

    Number() : data(0)
    {
    }

    explicit Number(double number) : data(number)
    {
    }

    double Calculate() const override;
};
