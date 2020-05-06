#include "HelloTriggers.h"

HelloTriggers::HelloTriggers()
{

}

HelloTriggers::~HelloTriggers()
{

}

void HelloTriggers::OnTriggerEnter(std::shared_ptr<Collider> other)
{
	std::cout << "On Trigger Enter from Hello Triggers! <3" << std::endl;
}

void HelloTriggers::OnTriggerStay(std::shared_ptr<Collider> other)
{

}

void HelloTriggers::OnTriggerExit(std::shared_ptr<Collider> other)
{
	std::cout << "On Trigger Exit from Hello Triggers! <3" << std::endl;
}