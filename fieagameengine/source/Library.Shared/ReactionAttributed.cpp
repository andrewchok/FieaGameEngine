#include "pch.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed() : 
		ReactionAttributed(std::string())
	{
	}

	ReactionAttributed::ReactionAttributed(const std::string& name) : 
		Reaction(TypeIdClass(), name)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const IEventPublisher& event)
	{
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass()));

		const Event<EventMessageAttributed>& eventMessageAttributed = static_cast<const Event<EventMessageAttributed>&>(event);
		const EventMessageAttributed& payload = eventMessageAttributed.Message();

		if (payload.GetSubType() == mSubtype)
		{
			Scope params;

			for (auto& attribute : payload.GetAuxiliaryAttributes())
			{
				auto& datum = params.Append(attribute->first);
				datum = attribute->second;
			}

			payload.GetWorld()->GetWorldState().GetCallStack().Push(std::move(params));
			ActionList::Update(payload.GetWorld()->GetWorldState());
			payload.GetWorld()->GetWorldState().GetCallStack().Pop();
			
		}
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		Vector<Signature> signatures = Reaction::Signatures();

		signatures.PushBack({ "SubType", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, mSubtype) });
		
		return signatures;
	}

	gsl::owner<Scope*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	void ReactionAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	const std::string& ReactionAttributed::GetSubtype() const
	{
		return mSubtype;
	}
}
