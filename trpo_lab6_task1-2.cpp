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


struct BinaryOperation : Expression {
    enum { PLUS = '+', MINUS = '-', DIV = '/', MUL = '*' };
    BinaryOperation(Expression const* left, int op, Expression const* right)
        : left_(left), op_(op), right_(right) {
        assert(left_ && right_);
    }
    ~BinaryOperation() { delete left_; delete right_; }
    Expression const* left() const { return left_; }
    Expression const* right() const { return right_; }
    int operation() const { return op_; }
    double evaluate() const override {
        double l = left_->evaluate(), r = right_->evaluate();
        switch (op_) {
        case PLUS: return l + r; case MINUS: return l - r;
        case DIV: return l / r; case MUL: return l * r;
        }
        return 0;
    }
    Expression* transform(Transformer* tr) const override { return tr->transformBinaryOperation(this); }
private:
    Expression const* left_;
    Expression const* right_;
    int op_;
};

struct FunctionCall : Expression {
    FunctionCall(string const& name, Expression const* arg)
        : name_(name), arg_(arg) {
        assert(arg_); assert(name_ == "sqrt" || name_ == "abs");
    }
    ~FunctionCall() { delete arg_; }
    string const& name() const { return name_; }
    Expression const* arg() const { return arg_; }
    double evaluate() const override {
        double val = arg_->evaluate();
        return (name_ == "sqrt") ? sqrt(val) : fabs(val);
    }
    Expression* transform(Transformer* tr) const override { return tr->transformFunctionCall(this); }
private:
    string const name_;
    Expression const* arg_;
};


struct CopySyntaxTree : Transformer {
    Expression* transformNumber(Number const* number) override { return new Number(number->value()); }
    Expression* transformBinaryOperation(BinaryOperation const* binop) override {
        return new BinaryOperation(
            binop->left()->transform(this),
            binop->operation(),
            binop->right()->transform(this)
        );
    }
    Expression* transformFunctionCall(FunctionCall const* fcall) override {
        return new FunctionCall(fcall->name(), fcall->arg()->transform(this));
    }
    Expression* transformVariable(Variable const* var) override { return new Variable(var->name()); }
};

struct FoldConstants : Transformer {
    Expression* transformNumber(Number const* number) override { return new Number(number->value()); }
    Expression* transformVariable(Variable const* var) override { return new Variable(var->name()); }

    Expression* transformBinaryOperation(BinaryOperation const* binop) override {
        Expression* new_left = binop->left()->transform(this);
        Expression* new_right = binop->right()->transform(this);

        Number* left_num = dynamic_cast<Number*>(new_left);
        Number* right_num = dynamic_cast<Number*>(new_right);

        if (left_num && right_num) {
            double result = BinaryOperation(left_num, binop->operation(), right_num).evaluate();
            delete new_left;
            delete new_right;
            return new Number(result);
        }
        return new BinaryOperation(new_left, binop->operation(), new_right);
    }

    Expression* transformFunctionCall(FunctionCall const* fcall) override {
        Expression* new_arg = fcall->arg()->transform(this);
        Number* arg_num = dynamic_cast<Number*>(new_arg);

        if (arg_num) {
            double result = FunctionCall(fcall->name(), arg_num).evaluate();
            delete new_arg;
            return new Number(result);
        }
        return new FunctionCall(fcall->name(), new_arg);
    }
};


void test_copy() {
    Number* n32 = new Number(32.0);
    Number* n16 = new Number(16.0);
    BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
    FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
    Variable* var = new Variable("var");
    BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
    FunctionCall* callAbs = new FunctionCall("abs", mult);

    CopySyntaxTree CST;
    Expression* newExpr = callAbs->transform(&CST);

    delete callAbs;
    cout << "Copy result: " << newExpr->evaluate() << endl;
    delete newExpr;
}

void test_fold() {
    Number* n32 = new Number(32.0);
    Number* n16 = new Number(16.0);
    BinaryOperation* minus = new BinaryOperation(n32, BinaryOperation::MINUS, n16);
    FunctionCall* callSqrt = new FunctionCall("sqrt", minus);
    Variable* var = new Variable("var");
    BinaryOperation* mult = new BinaryOperation(var, BinaryOperation::MUL, callSqrt);
    FunctionCall* callAbs = new FunctionCall("abs", mult);

    FoldConstants FC;
    Expression* newExpr = callAbs->transform(&FC);

    cout << "Folded result: " << newExpr->evaluate() << endl;
    delete newExpr;
    delete callAbs;
}


int main() {
    test_copy();
    test_fold();
    return 0;
}