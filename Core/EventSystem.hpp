#pragma once
#include <vector>
#include <map>
#include <string>
#include "Engine/Core/NamedStrings.hpp"
#include <mutex>
#include <Engine/Core/NamedProperties.hpp>

typedef NamedProperties EventArgs;
typedef bool (*EventCallbackFunction) (EventArgs& args);

struct EventSystemConfig
{
	
};



//------------------------------------------------------------------------------------------------
class EventSubscription
{
public:
	EventSubscription() = default;
	virtual ~EventSubscription() = default;

	virtual void Execute(EventArgs& args) = 0;
};


//struct EventSubscription
//{
//	EventCallbackFunction functionToPoint;
//
//	/*virtual void Execute() =0;*/
//};
typedef std::vector<EventSubscription*> SubscriptionList;

class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup();
	void ShutDown();
	void BeginFrame();
	void EndFrame();

	//void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts);
	//void UnSubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts);
	//void FireEvent(std::string const& eventName, EventArgs& args);
	//void FireEvent(std::string const& eventName);
	void GetRegisteredEventNames(std::vector<std::string>& outNames);

	//------------------------FUNCTION EVENTS----------------------
	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction func);
	template<typename T, typename U>
	void SubscribeEventCallbackObjectMethod(std::string const& eventName,T& object, U MethodType);
	void FireEvent(std::string const& eventName, EventArgs args = EventArgs());
protected:
	EventSystemConfig									m_config;
	std::map<std::string, SubscriptionList>			    m_subscriptionListsByEventName;
	std::mutex											m_eventSystemMutex;
};

//---------------------STANDALONE FUNCTIONS FOR CALLBACKS---------------------------------
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts);
void UnSubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction functionpts);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);
void GetRegisteredEventNames(std::vector<std::string>& outNames);

class FunctionEventSubscription : public EventSubscription
{
	public:
	FunctionEventSubscription(EventCallbackFunction event) { m_event = event;}
	~FunctionEventSubscription() {};

	virtual void Execute(EventArgs& args) override
	{
		m_event(args);
	}
	EventCallbackFunction m_event = nullptr;
};


template <typename T>
class MethodEventSubscription : public EventSubscription
{
	public:
	typedef void (T::*MethodType)(EventArgs& args);
	MethodEventSubscription(T& object, MethodType method) : m_object(object), m_method(method)
	{

	}
	~MethodEventSubscription() {};
	virtual void Execute(EventArgs& args) override
	{
		(m_object.*m_method)(args);
	}


	T& m_object;
	MethodType m_method;

};


//------------------------EVENT FUNCTIONS CALLBACKS----------------------

template<typename T, typename U>
inline void EventSystem::SubscribeEventCallbackObjectMethod(std::string const& eventName, T& object, U methodType)
{
	std::map<std::string, SubscriptionList>::iterator it = m_subscriptionListsByEventName.find(eventName);
	if (it != m_subscriptionListsByEventName.end())
	{
		SubscriptionList& subscribersForThisEvent = it->second;
		EventSubscription* newSubscription = new MethodEventSubscription<T>(object, methodType);
		subscribersForThisEvent.push_back(newSubscription);
	}
	else
	{
		SubscriptionList list;
		EventSubscription* newSubscription = new MethodEventSubscription<T>(object, methodType);
		list.push_back(newSubscription);
		m_subscriptionListsByEventName[eventName] = list;
	}
}
