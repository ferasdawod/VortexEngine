#pragma once

#include "Utilities/Singleton.h"
#include <unordered_map>
#include <memory>

class ProfilingSample;
using ProfilingSamplePtr = std::shared_ptr<ProfilingSample>;

class ProfilingManager : public Singleton<ProfilingManager>
{
public:
	ProfilingManager() {}
	virtual ~ProfilingManager() {}

	void		StartSample(const char* sampleName);
	void		EndSample(const char* sampleName);

	void		DumpInfo() const;

protected:
	std::unordered_map<const char*, ProfilingSamplePtr>		_profilingSamples;
};