#pragma once
class MyProductImpl;

class MyProductEx
{
public:
    MyProductEx(string name, float price);
    ~MyProductEx();

public:
    void Info() const;

private:
    std::unique_ptr<MyProductImpl> _pImpl;
};

