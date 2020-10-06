#include "pch.h"
#include "MyProductImpl.h"

MyProductImpl::MyProductImpl()
{
}

MyProductImpl::~MyProductImpl()
{
}

void MyProductImpl::Info()
{
	cout << _name << ", "
		<< _price << endl;
}