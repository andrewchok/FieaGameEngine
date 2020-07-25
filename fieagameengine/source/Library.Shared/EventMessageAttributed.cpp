#include "pch.h"
#include "EventMessageAttributed.h"


namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed() : 
		EventMessageAttributed(std::string())
	{
	}

	EventMessageAttributed::EventMessageAttributed(const std::string& name) : 
		Attributed(TypeIdClass()), mSubtype(name)
	{
	}

	const std::string& EventMessageAttributed::GetSubType() const
	{
		return mSubtype;
	}

	World* EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}

	void EventMessageAttributed::SetSubType(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	void EventMessageAttributed::SetWorld(World& world)
	{
		mWorld = &world;
	}

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(EventMessageAttributed, mSubtype) }
		};
	}

	gsl::owner<Scope*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}

}