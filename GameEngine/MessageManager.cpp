#include "MessageManager.h"

MessageManager* Mmanager = nullptr;

MessageManager& MessageManager::Get()
{
	if (Mmanager == nullptr)
		Mmanager = new MessageManager();

	return *Mmanager;
}

void MessageManager::AddMessage(msg* m)
{
	addmsgmutex.lock();
	messages.push_back(m);
	addmsgmutex.unlock();
}

std::list<msg*> MessageManager::GetMessages(destinatary d)
{
	std::list<msg*> list;

	getmsgmutex.lock();
	std::list<msg*>::iterator i = messages.begin();
	while (i != messages.end())
	{
		if ((*i)->dest == d)
		{
			list.push_back(*i);
			messages.erase(i++);  // alternatively, i = items.erase(i);
		}
		else
		{
			++i;
		}
	}
	getmsgmutex.unlock();

	return list;

}

void MessageManager::DeleteMessagesFromList(std::list<msg*>*list)
{
	for(msg* m : *list)
	{
		delete m;
	}
	list->clear();
}

