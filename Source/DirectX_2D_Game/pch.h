#pragma once

// c++ includes
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <algorithm>
#include <array>
#include <limits>

// TODO remove these if needed
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#include <windows.h>

// 3rd party include files
#include <d3d11.h>
#include <dxgi.h>
#include <3rd Party/TinyXml/tinyxml.h>
#include <DirectXTK/SimpleMath.h>

// engine include files

// utilities
#include <Debugging/Logger.h>
#include <Performance/ScopedProfilerSample.h>
#include <Utilities/XmlHelper.h>
#include <Debugging/Debug.h>
#include <Utilities/IdGenerator.h>
// TODO remove this
#include <Utilities/Macros.h>
// END TODO
#include <Utilities/Singleton.h>
#include "Utilities/Utils.h"

// actors and components
#include <Actors/ActorsDefenitions.h>
#include <Actors/Actor.h>
#include <Components/BaseComponent.h>
#include <Components/Transform.h>

// events
#include <Events/EventManager.h>
#include <Events/EventsTypes.h>

// input
#include <Input/Input.h>
#include <Engine/GameTimer.h>