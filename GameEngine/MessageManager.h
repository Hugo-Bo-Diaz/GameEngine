#ifndef MESSAGE_MANAGER__H
#define MESSAGE_MANAGER__H

#include <list>
#include <mutex>

#include "Messages.h"

class MessageManager
{
private:
	MessageManager() {};
	std::list<msg*> messages;
public:
	std::mutex addmsgmutex;
	void AddMessage(msg*);

	std::mutex getmsgmutex;
	std::list<msg*> GetMessages(destinatary d);
	static void DeleteMessagesFromList(std::list<msg*>*);

	static MessageManager& Get();

};
#endif