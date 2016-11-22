#pragma once

#include "Actors/ActorsDefenitions.h"
#include "Graphics/Colors.h"

#include <unordered_map>
#include <string>

using ActorsMap = std::unordered_map<ActorID, StrongActorPtr>;
class GameTimer;

class Level
{
public:
	Level(const std::string& levelName);
	virtual ~Level();
	
	void				OnUpdate(const GameTimer& gameTimer);
						
	// creates a new level and calls load level on it and then returns it if loading was good
	// returns null pointer if the loading failed
	static std::shared_ptr<Level>	CreateFromFile(const std::string& fileName);
						
	// loads the level from a file on the disk and return true if loading was good
	bool				LoadLevel(const std::string& fileName);
	bool				ReloadLevel();
	// saves the level to the disk and overwrites the old level file
	bool				SaveLevel();
	bool				SaveLevel(const std::string& fileName);
						
	WeakActorPtr		FindActor(ActorID id) const;
	WeakActorPtr		FindActor(const std::string& actorName) const;
						
	void				DestroyActor(ActorID id);
	void				DestroyActor(const std::string& actorName);
						
	bool				AddActor(StrongActorPtr actor);
						
	void				DestroyAllActors();
						
	size_t				ActorsCount() const { return _actors.size(); }
	Color				AmbientColor() const { return _ambientColor; }

public:
	static Level*		GetCurrent() { return _pCurrentLevel; }
protected:
	static Level*		_pCurrentLevel;

protected:
	const std::string	GetSavePath() const
	{
		return _sLevelSavePath.empty() ? "Assets\\Levels\\" + _sLevelName + ".xml" : _sLevelSavePath;
	}

protected:
	ActorsMap			_actors;
	std::string			_sLevelName;
	
	std::string			_sLevelSavePath;
	bool				_bWasLoaded;

	Color				_ambientColor;
};