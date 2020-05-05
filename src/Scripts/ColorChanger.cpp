#include "ColorChanger.h"

ColorChanger::ColorChanger()
{

}

ColorChanger::~ColorChanger()
{

}

void ColorChanger::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	std::cout << "Trigger enter" << std::endl;
}

void ColorChanger::OnTriggerStay(std::shared_ptr<Collider> other)
{
	std::cout << "Trigger stay" << std::endl;
}

void ColorChanger::OnTriggerExit(std::shared_ptr<Collider> other)
{
	std::cout << "Trigger exit" << std::endl;
}