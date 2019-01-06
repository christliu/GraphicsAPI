
#include "BaseApplication.h"
#include <iostream>
bool BaseApplication::m_bQuit = false;

bool BaseApplication::Initialize()
{
	std::cout << "BaseApp Init" << std::endl;
	return true;
}

void BaseApplication::Tick()
{

}

void BaseApplication::Finalize()
{}

void BaseApplication::Quit()
{
	m_bQuit = true;
}

void BaseApplication::OnMouseMove(int x, int y)
{

}