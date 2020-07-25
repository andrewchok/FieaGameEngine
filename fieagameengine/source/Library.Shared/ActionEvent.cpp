#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "World.h"

namespace Library
{
	using MilliSeconds = std::chrono::milliseconds;

	RTTI_DEFINITIONS(ActionEvent)

	ActionEvent::ActionEvent() : 
		ActionEvent(std::string())
	{
	}

	ActionEvent::ActionEvent(const std::string& name) : 
		Action(TypeIdClass(), name)
	{
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		worldState.action = this;

		EventMessageAttributed event;
		event.SetWorld(*worldState.world);
		event.SetSubType(mSubtype);
		//const auto& attributes = GetAttributes();

		//for (auto& attribute : attributes)
		//{
		//	if (!IsPrescribedAttribute(attribute->first))
		//	{
		//		event.AppendAuxiliaryAttribute(attribute->first) = attribute->second;
		//	}
		//}
		ForEachAuxiliaryAttribute([&event](const Scope::HashTablePair& attribute) 
		{
			event.AppendAuxiliaryAttribute(attribute); 
		});

		std::shared_ptr<Event<EventMessageAttributed>> eventPtr = std::make_shared<Event<EventMessageAttributed>>(std::move(event));
		worldState.world->GetEventQueue().Enqueue(eventPtr, worldState.GetGameTime(), MilliSeconds(mDelay));
	}

	gsl::owner<Scope*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		Vector<Signature> signatures = Action::Signatures();

		signatures.PushBack({ "SubType", Datum::DatumTypes::String, 1, offsetof(ActionEvent, mSubtype) });
		signatures.PushBack({ "Delay", Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, mDelay) });
		
		return signatures;
	}

	void ActionEvent::SetDelay(const MilliSeconds& milliseconds)
	{
		mDelay = static_cast<std::size_t>(milliseconds.count());
	}

	void ActionEvent::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	const MilliSeconds ActionEvent::GetDelay() const
	{
		return MilliSeconds(mDelay);
	}

	const std::string& ActionEvent::GetSubtype() const
	{
		return mSubtype;
	}
}