#include "EventSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"

EventSystem* g_theEventSystem = nullptr;
EventSystem::EventSystem(EventSystemConfig const& config)
{
	m_config = config;
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{

}

void EventSystem::ShutDown()
{
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}
//
//void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts)
//{
//	std::map<std::string, SubscriptionList>::const_iterator iter;
//	iter = m_subscriptionListsByEventName.find(eventName);
//	if (iter != m_subscriptionListsByEventName.end())
//	{
//		EventSubscription eventSubscription;
//		SubscriptionList subscriptionList = iter->second;
//		eventSubscription.functionToPoint = functionpts;
//		subscriptionList.push_back(eventSubscription);
//	}	
//	else
//	{
//		SubscriptionList subs;
//		EventSubscription eventSubscription;
//		eventSubscription.functionToPoint = functionpts;
//		subs.push_back(eventSubscription);
//		m_subscriptionListsByEventName[eventName] = subs;
//	}
//
//}
//
//void EventSystem::UnSubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts)
//{
//	for (int i = 0; i < m_subscriptionListsByEventName[eventName].size(); i++)
//	{
//		if (m_subscriptionListsByEventName[eventName][i].functionToPoint == functionpts)
//		{
//			m_subscriptionListsByEventName[eventName].erase(m_subscriptionListsByEventName[eventName].begin() + i);
//		}
//	}
//}
//
//void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
//{
//	std::map<std::string, SubscriptionList>::const_iterator iter;
//	if (!m_subscriptionListsByEventName.empty())
//	{
//		iter = m_subscriptionListsByEventName.find(eventName);
//		if (iter != m_subscriptionListsByEventName.end())
//		{
//			SubscriptionList subs = iter->second;
//			for (int i = 0; i < subs.size(); i++)
//			{
//				m_subscriptionListsByEventName[eventName][i].functionToPoint(args);
//			}
//		}
//	}
//	
//}

//void EventSystem::FireEvent(std::string const& eventName)
//{
//	EventArgs args;
//	if(g_theEventSystem)
//		g_theEventSystem->FireEvent(eventName,args);
//}

void EventSystem::GetRegisteredEventNames(std::vector<std::string>& outNames)
{
	std::map<std::string, SubscriptionList>::iterator itr;
	for (itr = m_subscriptionListsByEventName.begin(); itr != m_subscriptionListsByEventName.end(); ++itr)
	{
		outNames.push_back(itr->first);
	}
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction func)
{
	std::map<std::string, SubscriptionList>::iterator it = m_subscriptionListsByEventName.find(eventName);
	if (it != m_subscriptionListsByEventName.end())
	{
		SubscriptionList& subscribersForThisEvent = it->second;
		FunctionEventSubscription* newSubscription = new FunctionEventSubscription(func);
		subscribersForThisEvent.push_back(newSubscription);
	}
	else
	{
		SubscriptionList list;
		FunctionEventSubscription* newSubscription = new FunctionEventSubscription(func);
		list.push_back(newSubscription);
		m_subscriptionListsByEventName[eventName] = list;
	}

}

void EventSystem::FireEvent(std::string const& eventName, EventArgs args)
{
	bool eventFound = false;
	std::map<std::string, SubscriptionList>::iterator itr;
	for (itr = m_subscriptionListsByEventName.begin(); itr != m_subscriptionListsByEventName.end(); ++itr)
	{
		HashedCaseInsensitiveString insensitiveFunctionCall(eventName);
		eventFound = insensitiveFunctionCall == HashedCaseInsensitiveString(itr->first);
		if (eventFound)
		{
			SubscriptionList& subscribersForThisEvent = itr->second;
			for (int subscriberIndex = 0; subscriberIndex < subscribersForThisEvent.size(); ++subscriberIndex)
			{
				EventSubscription* subscriber = subscribersForThisEvent[subscriberIndex];
				subscriber->Execute(args);
				return;
			}
		}
	}

	//std::map<std::string, SubscriptionList>::iterator it = m_subscriptionListsByEventName.find(eventName);
	
}

//void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts)
//{
//	if (g_theEventSystem)
//	{
//		g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionpts);
//	}
//}
//
//void UnSubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts)
//{
//	if (g_theEventSystem)
//	{
//		g_theEventSystem->UnSubscribeEventCallbackFunction(eventName, functionpts);
//	}
//}
//
//void FireEvent(std::string const& eventName, EventArgs& args)
//{
//	if (g_theEventSystem)
//	{
//		g_theEventSystem->FireEvent(eventName, args);
//	}
//}
//
//void FireEvent(std::string const& eventName)
//{
//	if (g_theEventSystem)
//	{
//		g_theEventSystem->FireEvent(eventName);
//	}
//}
//
//void GetRegisteredEventNames(std::vector< std::string >& outNames)
//{
//	if (g_theEventSystem)
//	{
//		g_theEventSystem->GetRegisteredEventNames(outNames);
//	}
//
//}
