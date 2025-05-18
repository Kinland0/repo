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