#include "pch.h"

#include "ProfilingManager.h"

#include "ProfilingSample.h"
#include <sstream>
#include <iomanip>

void ProfilingManager::StartSample(const char* sampleName)
{
	auto findIt = _profilingSamples.find(sampleName);
	if (findIt == _profilingSamples.end())
	{
		ProfilingSamplePtr sample(DBG_NEW ProfilingSample);
		_profilingSamples.emplace(std::make_pair(sampleName, sample));
		sample->Start();
	}
	else
	{
		findIt->second->Start();
	}
}

void ProfilingManager::EndSample(const char* sampleName)
{
	auto findIt = _profilingSamples.find(sampleName);
	assert(findIt != _profilingSamples.end());

	findIt->second->Stop();
}

void ProfilingManager::DumpInfo() const
{
	using namespace std;

	stringstream stream;
	stream << "Profiling Results : " << endl;
	stream << setw(50) << "Sample Name" << setw(20) << "Average" << setw(20) << "Min Time" << setw(20) << "Max Time" << endl;
	
	for (auto itr = _profilingSamples.cbegin(); itr != _profilingSamples.cend(); ++itr)
	{
		ProfilingSamplePtr sample = itr->second;

		stream << setw(50) << itr->first << setw(20) << sample->GetAvarageTime() << setw(20) << sample->GetMinTime() << setw(20) << sample->GetMaxTime() << endl;
	}

	LOG_M(stream.str());
}