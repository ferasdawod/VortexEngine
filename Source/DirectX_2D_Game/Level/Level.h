#pragma once

#include "Actors/ActorsDefenitions.h"
#include "Graphics/Colors.h"

#include <unordered_map>
#include <string>

class GameTimer;
class ActorFactory;

class Level
{
public:
	Level(const std::string& levelName, std::shared_ptr<ActorFactory> actorFactory);
	virtual ~Level();
	
	void				OnUpdate(const GameTimer& gameTimer);
						
	// loads the level from a file on the disk and return true if loading was good
	bool				LoadLevel(const std::string& fileName);
	bool				ReloadLevel();
	// saves the level to the disk and overwrites the old level file
	bool				SaveLevel();
	bool				SaveLevel(const std::string& fileName);
						
	WeakActorPtr		FindActor(ObjectId id) const;
	WeakActorPtr		FindActor(const std::string& actorName) const;
						
	void				DestroyActor(ObjectId id);
	void				DestroyActor(const std::string& actorName);
						
	bool				AddActor(StrongActorPtr actor);
						
	void				DestroyAllActors();
						
	size_t				ActorsCount() const { return _actors.size(); }
	Color				AmbientColor() const { return _ambientColor; }

	const std::vector<StrongActorPtr>& GetActors() const { return _actors; }

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
	std::shared_ptr<ActorFactory> _actorFactory;

	std::vector<StrongActorPtr> _actors;
	std::string _sLevelName;
	
	std::string	_sLevelSavePath;
	bool _bWasLoaded;

	Color _ambientColor;
};