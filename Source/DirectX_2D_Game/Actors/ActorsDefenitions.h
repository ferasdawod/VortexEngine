#pragma once

#include <memory>

class IComponent;
class Actor;

using ActorID = unsigned long;
using ComponentID = unsigned long;

using StrongActorPtr = std::shared_ptr<Actor>;
using WeakActorPtr = std::weak_ptr<Actor>;

using StrongComponentPtr = std::shared_ptr<IComponent>;
using WeakComponentPtr = std::weak_ptr<IComponent>;
