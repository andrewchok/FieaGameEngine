#pragma once
#include "GameClock.h"
#include "GameTime.h"
#include "Stack.h"
#include "Scope.h"

namespace Library
{
	class WorldState final
	{
	public:
		/// <summary>
		/// Get Game Time
		/// </summary>
		/// <returns>GameTime</returns>
		const GameTime& GetGameTime() const;

		/// <summary>
		/// Set Game Time
		/// </summary>
		/// <param name="gameTime">GameTime</param>
		void SetGameTime(const GameTime& gameTime);

		Stack<Scope>& GetCallStack();
		
		class World* world = nullptr;
		class Sector* sector = nullptr;
		class Entity* entity = nullptr;
		class Action* action = nullptr;
	private:
		GameClock mGameClock;
		GameTime mGameTime;

		Stack<Scope> mCallStack;
	};

}