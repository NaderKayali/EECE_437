#pragma once

#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

enum EOperation
{
    NOT, MINUS, CEIL, FLOOR,
    AND, OR, XOR, IMPLIES,
    ADD, SUBTRACT, MULTIPLY, DIVIDE,
    LESSTHAN, MORETHAN, EQUALS,
    EOPERATION_END
};

class TAType;

class TAElement
{
public:
    virtual void evaluate(TAType& output) const = 0;
    virtual void list() const = 0;
    virtual void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const = 0;
};

class TAType : public TAElement
{
public:
};

class TABool;
class TAInt;
class TADouble;

class TABool : public TAType
{
private:
    bool myValue;

public:
    TABool(bool init = false)
    {
        myValue = init;
    }

    bool get() const
    {
        return myValue;
    }

    void set(bool value)
    {
        myValue = value;
    }

    void evaluate(TAType& output) const
    {
        ((TABool&)output).set(myValue);
    }

    void list() const
    {
        if (myValue)
            cout << "true";
        else
            cout << "false";
    }

    void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const
    {
        TABool result;
        if (rhs)
            rhs->evaluate(result);
        switch (operation)
        {
        case NOT:
            ((TABool&)output).set(!myValue);
            break;
        case AND:
            ((TABool&)output).set(myValue && result.get());
            break;
        case OR:
            ((TABool&)output).set(myValue || result.get());
            break;
        case XOR:
            ((TABool&)output).set(myValue ^ result.get());
            break;
        case IMPLIES:
            ((TABool&)output).set(!myValue || result.get());
            break;
        default:
            break;
        }
    }
};

class TAInt : public TAType
{
private:
    int myValue;

public:
    TAInt(int init = 0)
    {
        myValue = init;
    }

    int get() const
    {
        return myValue;
    }

    void set(int value)
    {
        myValue = value;
    }

    void evaluate(TAType& output) const
    {
        ((TAInt&)output).set(myValue);
    }

    void list() const
    {
        cout << myValue;
    }

    void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const
    {
        TAInt result;
        if (rhs)
            rhs->evaluate(result);

        switch (operation)
        {
        case MINUS:
            ((TAInt&)output).set(-myValue);
            break;
        case ADD:
            ((TAInt&)output).set(myValue + result.get());
            break;
        case SUBTRACT:
            ((TAInt&)output).set(myValue - result.get());
            break;
        case MULTIPLY:
            ((TAInt&)output).set(myValue * result.get());
            break;
        case DIVIDE:
            ((TAInt&)output).set(myValue / result.get());
            break;
        case LESSTHAN:
            ((TABool&)output).set(myValue < result.get());
            break;
        case MORETHAN:
            ((TABool&)output).set(myValue > result.get());
            break;
        case EQUALS:
            ((TABool&)output).set(myValue == result.get());
            break;
        default:
            break;
        }
    }
};

class TADouble : public TAType
{
private:
    double myValue;

public:
    TADouble(double init = 0)
    {
        myValue = init;
    }

    double get() const
    {
        return myValue;
    }

    void set(double value)
    {
        myValue = value;
    }

    void evaluate(TAType& output) const
    {
        ((TADouble&)output).set(myValue);
    }

    void list() const
    {
        cout << myValue;
    }

    void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const
    {
        TADouble result;
        if (rhs)
            rhs->evaluate(result);

        switch (operation)
        {
        case MINUS:
            ((TADouble&)output).set(-myValue);
            break;
        case ADD:
            ((TADouble&)output).set(myValue + result.get());
            break;
        case SUBTRACT:
            ((TADouble&)output).set(myValue - result.get());
            break;
        case MULTIPLY:
            ((TADouble&)output).set(myValue * result.get());
            break;
        case DIVIDE:
            ((TADouble&)output).set(myValue / result.get());
            break;
        case LESSTHAN:
            ((TABool&)output).set(myValue < result.get());
            break;
        case MORETHAN:
            ((TABool&)output).set(myValue > result.get());
            break;
        case EQUALS:
            ((TABool&)output).set(myValue == result.get());
            break;
        case CEIL:
            ((TAInt&)output).set((int)ceil(myValue));
            break;
        case FLOOR:
            ((TAInt&)output).set((int)floor(myValue));
            break;
        default:
            break;
        }
    }
};

class TACompound : public TAType
{
protected:
    vector<TAElement*> myElements;

    TACompound(int length, TAElement* init) : myElements(length, init)
    {}
};

class TAPair : public TACompound
{
public:
    TAPair(TAElement& first, TAElement& next) : TACompound(2, nullptr)
    {
        myElements[0] = &first;
        myElements[1] = &next;
    }

    TAElement& first()
    {
        return *(myElements[0]);
    }

    TAElement& next()
    {
        return *(myElements[1]);
    }

    vector<TAElement*>& get() 
    {
        return myElements;
    }

    void set(const vector<TAElement*>& value)
    {
        myElements.assign(value.begin(), value.end());
    }

    void evaluate(TAType& output) const
    {
        ((TAPair&)output).set(myElements);
    }

    void list() const
    {
        cout << "(";
        myElements[0]->list();
        cout << ", ";
        myElements[1]->list();
        cout << ")";
    }

    void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const
    {
        vector<TAElement*>& outElements = ((TAPair&)output).get();
        myElements[0]->operate(operation, (TAType&)*outElements[0], ((TAPair*)rhs)->get()[0]);
        myElements[1]->operate(operation, (TAType&)*outElements[1], ((TAPair*)rhs)->get()[1]);
    }
};

class TAArray : public TACompound
{
public:
    TAArray(int size = 0) : TACompound(size, nullptr)
    {
        
    }

    TAElement& operator[](int index)
    {
        return *myElements[index];
    }

    vector<TAElement*>& get()
    {
        return myElements;
    }

    void set(const vector<TAElement*>& value)
    {
        myElements.assign(value.begin(), value.end());
    }

    void evaluate(TAType& output) const
    {
        ((TAArray&)output).set(myElements);
    }

    void list() const
    {
        cout << "{";
        for (int i = 0; i < myElements.size(); i++)
        {
            myElements[i]->list();
            cout << ", ";
        }
        cout << "}";
    }

    void operate(EOperation operation, TAType& output, const TAElement* rhs = nullptr) const
    {
        vector<TAElement*>& outElements = ((TAArray&)output).get();
        for (int i = 0; i < myElements.size(); i++)
        {
            myElements[i]->operate(operation, (TAType&)*outElements[i], ((TAArray*)rhs)->get()[i]);
        }
    }
};

class TAOperator : public TAElement
{
public:
    void operate(EOperation operation, TAType& output, const TAElement* rhs) const
    {
        evaluate(output);
        output.operate(operation, output, rhs);
    }
};

class TANot : public TAOperator
{
private:
    TAElement& myFirst;

public:
    TANot(TAElement& first) : myFirst(first)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(NOT, output);
    }

    void list() const
    {
        cout << "(! ";
        myFirst.list();
        cout << ")";
    }
};

class TAMinus : public TAOperator
{
private:
    TAElement& myFirst;

public:
    TAMinus(TAElement& first) : myFirst(first)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(MINUS, output);
    }

    void list() const
    {
        cout << "(minus ";
        myFirst.list();
        cout << ")";
    }
};

class TACeil : public TAOperator
{
private:
    TAElement& myFirst;

public:
    TACeil(TAElement& first) : myFirst(first)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(CEIL, output);
    }

    void list() const
    {
        cout << "(ceil ";
        myFirst.list();
        cout << ")";
    }
};

class TAFloor : public TAOperator
{
private:
    TAElement& myFirst;

public:
    TAFloor(TAElement& first) : myFirst(first)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(FLOOR, output);
    }

    void list() const
    {
        cout << "(floor ";
        myFirst.list();
        cout << ")";
    }
};

class TAAnd : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAAnd(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(AND, output, &myNext);
    }

    void list() const
    {
        cout << "(and ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAOr : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAOr(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(OR, output, &myNext);
    }

    void list() const
    {
        cout << "(or ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAXor : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAXor(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(XOR, output, &myNext);
    }

    void list() const
    {
        cout << "(xor ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAImplies : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAImplies(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(IMPLIES, output, &myNext);
    }

    void list() const
    {
        cout << "(implies ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAAdd : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAAdd(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(ADD, output, &myNext);
    }

    void list() const
    {
        cout << "(+ ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TASubtract : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TASubtract(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(SUBTRACT, output, &myNext);
    }

    void list() const
    {
        cout << "(- ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAMultiply : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAMultiply(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(MULTIPLY, output, &myNext);
    }

    void list() const
    {
        cout << "(* ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TADivide : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TADivide(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(DIVIDE, output, &myNext);
    }

    void list() const
    {
        cout << "(/ ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TALessThan : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TALessThan(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(LESSTHAN, output, &myNext);
    }

    void list() const
    {
        cout << "(< ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAMoreThan : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAMoreThan(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(MORETHAN, output, &myNext);
    }

    void list() const
    {
        cout << "(> ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};

class TAEquals : public TAOperator
{
private:
    TAElement& myFirst;
    TAElement& myNext;

public:
    TAEquals(TAElement& first, TAElement& next) : myFirst(first), myNext(next)
    {

    }

    void evaluate(TAType& output) const
    {
        myFirst.operate(EQUALS, output, &myNext);
    }

    void list() const
    {
        cout << "(== ";
        myFirst.list();
        cout << " ";
        myNext.list();
        cout << ")";
    }
};