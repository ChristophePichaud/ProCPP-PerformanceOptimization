#include "pch.h"
#include "MyProductEx.h"
#include "MyProductImpl.h"

MyProductEx::MyProductEx(string name, float price)
{
	_pImpl = std::make_unique<MyProductImpl>();
	_pImpl->_name = name;
	_pImpl->_price = price;

}

MyProductEx::~MyProductEx()
{		
}

void MyProductEx::Info() const
{
	_pImpl->Info();
}
