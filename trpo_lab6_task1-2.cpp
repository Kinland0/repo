#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <memory>

using namespace std;

struct Transformer;

struct Number;
struct Variable;

struct BinaryOperation;
struct FunctionCall;

struct Expression {
    virtual ~Expression() {}
    virtual double evaluate() const = 0;
    virtual Expression* transform(class Transformer* tr) const = 0;
};


struct Transformer {
    virtual ~Transformer() {}
    virtual Expression* transformNumber(Number const*) = 0;
    virtual Expression* transformBinaryOperation(BinaryOperation const*) = 0;
    virtual Expression* transformFunctionCall(FunctionCall const*) = 0;
    virtual Expression* transformVariable(Variable const*) = 0;
};


struct Number : Expression {
    Number(double value) : value_(value) {}
    double value() const { return value_; }
    double evaluate() const override { return value_; }
    Expression* transform(Transformer* tr) const override { return tr->transformNumber(this); }
private:
    double value_;
};

struct Variable : Expression {
    Variable(string const& name) : name_(name) {}
    string const& name() const { return name_; }
    double evaluate() const override { return 0.0; }
    Expression* transform(Transformer* tr) const override { return tr->transformVariable(this); }
private:
    string const name_;
};