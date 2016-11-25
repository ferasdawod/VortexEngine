#include "Level.h"

#include "Actors/Actor.h"
#include "Actors/ActorFactory.h"
#include "Debugging/Debug.h"
#include "Input/Input.h"
#include "Utilities/XmlHelper.h"
#include "Application/GameTimer.h"

#include "3rd Party/TinyXml/tinyxml.h"
#include <algorithm>

Level* Level::_pCurrentLevel = nullptr;

Level::Level(const std::string& levelName) : _sLevelName(levelName), _ambientColor(0.2f, 0.2f, 0.2f, 1.0f)
{
	assert(_pCurrentLevel == nullptr && "There can only be one level at any time");
	_pCurrentLevel = this;
}

Level::~Level()
{
	DestroyAllActors();
	assert(_pCurrentLevel == this && "How did this happen dude");
	_pCurrentLevel = nullptr;
}

void Level::OnUpdate(const GameTimer& gameTimer)
{
	FUNC_PROFILE();

	for (auto it = _actors.cbegin(); it != _actors.cend(); it++)
	{
		if (it->second->GetEnabled())
			it->second->OnUpdate(gameTimer);
	}
}

std::shared_ptr<Level> Level::CreateFromFile(const std::string& fileName)
{
	std::shared_ptr<Level> lvl(DBG_NEW Level("Blah Blah Blah"));
	if (lvl->LoadLevel(fileName))
	{
		return lvl;
	}
	
	return std::shared_ptr<Level>();
}

bool Level::LoadLevel(const std::string& fileName)
{
	// first clean up any actors
	DestroyAllActors();

	ActorFactory actorFactory;

	std::shared_ptr<TiXmlDocument> lvlD(DBG_NEW TiXmlDocument(fileName));
	if (!lvlD->LoadFile())
	{
		auto msg = "Failed to load the level from the file " + fileName;
		LOG_E(msg.c_str(), 0);
		return false;
	}

	auto lvlE = lvlD->FirstChildElement("Level");
	if (!lvlE)
	{
		LOG_E("the level format is wrong, no Level element was found at the root level", 0);
		return false;
	}

	lvlE->QueryStringAttribute("Name", &_sLevelName);

	_ambientColor = Color(0.25f, 0.25f, 0.25f, 1.0f);
	XmlHelper::FromXml(lvlE, "AmbientColor", _ambientColor);

	auto actorsE = lvlE->FirstChildElement("Actors");
	int actorsCount;
	actorsE->QueryIntAttribute("Count", &actorsCount);

	size_t actorIndex = 0;
	for (TiXmlElement* actorE = actorsE->FirstChildElement(); actorE;
		 actorE = actorE->NextSiblingElement(), ++actorIndex)
	{
		auto actorPtr = actorFactory.CreateFromXML(actorE);
		if (actorPtr)
		{
			_actors.emplace(actorPtr->GetUniqueID(), actorPtr);
		}
		else
		{
			bool isActorE = actorE->ValueStr() == std::string("Actor");
			if (!isActorE)
			{
				std::string msg = "an actor at index " + std::to_string(actorIndex) +
					"could not be loaded in the level file because it is not an actor element";
				LOG_M(msg);
			}
			else
			{
				std::string actorName;
				actorE->QueryStringAttribute("Name", &actorName);
				std::string msg = "the actor at index " + std::to_string(actorIndex) +
					"could not be loaded, actor name = " + actorName;
				LOG_M(msg);
			}

			return false;
		}
	}

	assert(_actors.size() == actorsCount);
	_sLevelSavePath = fileName;
	_bWasLoaded = true;
	return true;
}

bool Level::ReloadLevel()
{
	if (_bWasLoaded)
		return LoadLevel(_sLevelSavePath);
	else
		return false;
}

bool Level::SaveLevel()
{
	return SaveLevel(GetSavePath());
}

bool Level::SaveLevel(const std::string& fileName)
{
	TiXmlElement* lvlE = DBG_NEW TiXmlElement("Level");
	lvlE->SetAttribute("Name", _sLevelName);

	TiXmlElement* actorsE = DBG_NEW TiXmlElement("Actors");
	actorsE->SetDoubleAttribute("Count", ActorsCount());
	for (auto it = _actors.begin(); it != _actors.end(); it++)
	{
		auto actorE = it->second->ToXML();
		actorsE->LinkEndChild(actorE);
	}
	auto ambientColorE = XmlHelper::ToXml("AmbientColor", _ambientColor);

	lvlE->LinkEndChild(ambientColorE);
	lvlE->LinkEndChild(actorsE);

	std::shared_ptr<TiXmlDocument> lvlD(DBG_NEW TiXmlDocument());
	lvlD->LinkEndChild(lvlE);
	if (lvlD->SaveFile(fileName))
		return true;
	else
	{
		LOG_M("Failed to save the level" + _sLevelName + "To disk, " + std::string(lvlD->ErrorDesc()));
		return false;
	}
}

WeakActorPtr Level::FindActor(ActorID id) const
{
	auto findit = _actors.find(id);
	if (findit != _actors.end())
	{
		WeakActorPtr actorPtr(findit->second);
		return actorPtr;
	}
	else
	{
		return WeakActorPtr();
	}
}

WeakActorPtr Level::FindActor(const std::string& actorName) const
{
	auto findIt = std::find_if(_actors.cbegin(), _actors.cend(), 
				 [&actorName](const std::pair<ActorID, StrongActorPtr> pair) 
	{ 
		return (pair.second->GetName() == actorName);
	});

	if (findIt != _actors.cend())
		return WeakActorPtr(findIt->second);
	else
		return WeakActorPtr();
}

void Level::DestroyActor(const std::string& actorName)
{
	auto findIt = std::find_if(_actors.cbegin(), _actors.cend(),
							   [&actorName](const std::pair<ActorID, StrongActorPtr> pair)
	{
		return (pair.second->GetName() == actorName);
	});

	if (findIt != _actors.cend())
	{
		findIt->second->Destroy();
		_actors.erase(findIt);
	}
}

void Level::DestroyActor(ActorID id)
{
	auto findit = _actors.find(id);
	if (findit != _actors.end())
	{
		findit->second->Destroy();
		_actors.erase(findit);
	}
}

bool Level::AddActor(StrongActorPtr actor)
{
	auto id = actor->GetUniqueID();
	auto findit = _actors.find(id);
	// if we couldn't find the actor in our map then this is a new actor and we should add him to our list
	if (findit == _actors.end())
	{
		_actors.emplace(id, actor);
		return true;
	}

	// if we get here then the actor was already in our map and we should log this mischief
	LOG_M("Attempting to add an actor to the level while he already exists in it");
	return false;
}

void Level::DestroyAllActors()
{
	for (auto it = _actors.begin(); it != _actors.end(); it++)
	{
		it->second->Destroy();
	}

	_actors.clear();
}