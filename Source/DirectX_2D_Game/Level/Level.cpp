#include "pch.h"

#include "Level.h"

#include "Actors/ActorFactory.h"

Level* Level::_pCurrentLevel = nullptr;

Level::Level(const std::string& levelName, std::shared_ptr<ActorFactory> actorFactory) 
	: _actorFactory(actorFactory), _sLevelName(levelName),  _ambientColor(0.2f, 0.2f, 0.2f, 1.0f), _bWasLoaded(false)
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

void Level::OnUpdate(float deltaTime)
{
	FUNC_PROFILE();

	for (auto it = _actors.cbegin(); it != _actors.cend(); ++it)
	{
		auto actor = *it;
		if (actor->IsEnabled())
			actor->OnUpdate(deltaTime);
	}
}

bool Level::LoadLevel(const std::string& fileName)
{
	// first clean up any actors
	DestroyAllActors();

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
		auto actorPtr = _actorFactory->CreateFromXML(actorE);
		if (actorPtr)
		{
			_actors.push_back(actorPtr);
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
	for (auto it = _actors.begin(); it != _actors.end(); ++it)
	{
		auto actorE = (*it)->ToXML();
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

WeakActorPtr Level::FindActor(ObjectId id) const
{
	auto findit = std::find_if(_actors.begin(), _actors.end(), [&](StrongActorPtr ptr) { return ptr->GetId() == id; });
	if (findit != _actors.end())
	{
		WeakActorPtr actorPtr(*findit);
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
				 [&actorName](StrongActorPtr ptr) 
	{ 
		return (ptr->GetName() == actorName);
	});

	if (findIt != _actors.cend())
		return WeakActorPtr(*findIt);
	else
		return WeakActorPtr();
}

void Level::DestroyActor(const std::string& actorName)
{
	auto findIt = std::find_if(_actors.cbegin(), _actors.cend(),
							   [&actorName](StrongActorPtr ptr)
	{
		return (ptr->GetName() == actorName);
	});

	if (findIt != _actors.cend())
	{
		(*findIt)->Destroy();
		_actors.erase(findIt);
	}
}

void Level::DestroyActor(ObjectId id)
{
	auto findit = std::find_if(_actors.begin(), _actors.end(), [&](StrongActorPtr ptr) { return ptr->GetId() == id; });
	if (findit != _actors.end())
	{
		(*findit)->Destroy();
		_actors.erase(findit);
	}
}

bool Level::AddActor(StrongActorPtr actor)
{
	auto id = actor->GetId();
	auto findit = std::find_if(_actors.begin(), _actors.end(), [&](StrongActorPtr ptr) { return ptr->GetId() == id; });
	// if we couldn't find the actor in our map then this is a new actor and we should add him to our list
	if (findit == _actors.end())
	{
		_actors.push_back(actor);
		return true;
	}

	// if we get here then the actor was already in our map and we should log this mischief
	LOG_M("Attempting to add an actor to the level while he already exists in it");
	return false;
}

void Level::DestroyAllActors()
{
	for (auto it = _actors.begin(); it != _actors.end(); ++it)
	{
		(*it)->Destroy();
	}

	_actors.clear();
}