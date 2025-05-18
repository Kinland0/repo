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



// Конкретные продукты для Нового Года
class NYCard : public PostCard {
public:
    void display() const override { cout << "Новогодняя открытка\n"; }
};

class NYPresent : public Present {
public:
    void show() const override { cout << "Новогодний подарок\n"; }
};

class NYCake : public Cake {
public:
    void describe() const override { cout << "Новогодний торт\n"; }
};

// Конкретные продукты для Дня Рождения
class BDCard : public PostCard {
public:
    void display() const override { cout << "Открытка на ДР\n"; }
};

class BDPresent : public Present {
public:
    void show() const override { cout << "Подарок на ДР\n"; }
};

class BDCake : public Cake {
public:
    void describe() const override { cout << "Торт на ДР\n"; }
};

// Конкретные продукты для 8 Марта 
class March8Card : public PostCard {
public:
    void display() const override { cout << "Открытка к 8 Марта\n"; }
};

class March8Present : public Present {
public:
    void show() const override { cout << "Подарок на 8 Марта\n"; }
};

class March8FlowerArrangement : public Cake { 
public:
    void describe() const override { cout << "Букет цветов к 8 Марта\n"; }
};




// Абстрактная фабрика
class AbstractFactory {
public:
    virtual PostCard* createPostCard() const = 0;
    virtual Present* createPresent() const = 0;
    virtual Cake* createCake() const = 0;
    virtual ~AbstractFactory() = default;
};



// Конкретные фабрики
class NYFactory : public AbstractFactory {
public:
    PostCard* createPostCard() const override { return new NYCard(); }
    Present* createPresent() const override { return new NYPresent(); }
    Cake* createCake() const override { return new NYCake(); }
};

class BDFactory : public AbstractFactory {
public:
    PostCard* createPostCard() const override { return new BDCard(); }
    Present* createPresent() const override { return new BDPresent(); }
    Cake* createCake() const override { return new BDCake(); }
};

class March8Factory : public AbstractFactory {
public:
    PostCard* createPostCard() const override { return new March8Card(); }
    Present* createPresent() const override { return new March8Present(); }
    Cake* createCake() const override { return new March8FlowerArrangement(); }
};




