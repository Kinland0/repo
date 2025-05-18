#include <iostream>
#include <string>

using namespace std;

// Абстрактные продукты
class PostCard {
public:
    virtual void display() const = 0;
    virtual ~PostCard() = default;
};

class Present {
public:
    virtual void show() const = 0;
    virtual ~Present() = default;
};

class Cake {
public:
    virtual void describe() const = 0;
    virtual ~Cake() = default;
};



