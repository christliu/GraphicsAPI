#pragma once
#include "Derived.h"
#include <iostream>
class TestApp : public Derived
{
public:
	void Draw() {
		std::cout << "In Test App" << std::endl;
	}
};
