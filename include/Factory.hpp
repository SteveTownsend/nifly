/*
nifly
C++ NIF library for the Gamebryo/NetImmerse File Format
See the included GPLv3 LICENSE file
*/

#pragma once

#include "BasicTypes.hpp"

#include <unordered_map>

namespace nifly {

class NiFactory {
public:
	virtual NiObject* Create() = 0;
	virtual NiObject* Load(NiIStream& stream) = 0;

	virtual ~NiFactory() = default;
};

template<typename T>
class NiFactoryType FINALCLASS : public NiFactory {
public:
	// Create new NiObject
	NiObject* Create() override { return new T; }

	// Load new NiObject from file
	NiObject* Load(NiIStream& stream) override {
		T* nio = new T;
		nio->Get(stream);
		return nio;
	}
};

class NiFactoryRegister {
public:
	// Constructor registers the block types
	NiFactoryRegister();

	template<typename T>
	void RegisterFactory() {
		// Any NiObject can be registered together with its block name
		m_registrations.emplace(T::BlockName, std::make_unique<NiFactoryType<T>>());
	}

	// Get block factory via header std::string
	NiFactory* GetFactoryByName(const std::string& name) {
		auto it = m_registrations.find(name);
		if (it != m_registrations.end())
			return it->second.get();

		return nullptr;
	}

	// Get static instance of factory register
	static NiFactoryRegister& Get();

protected:
	std::unordered_map<std::string, std::unique_ptr<NiFactory>> m_registrations;
};
} // namespace nifly
