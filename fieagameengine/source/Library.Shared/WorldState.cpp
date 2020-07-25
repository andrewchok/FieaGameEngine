#include "pch.h"
#include "WorldState.h"

namespace Library
{
	const GameTime& WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(const GameTime& gameTime)
	{
		mGameTime = gameTime;
	}
	Stack<Scope>& WorldState::GetCallStack()
	{
		return mCallStack;
	}
}

