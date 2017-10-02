#pragma once

#include <memory>
#include <string>
#include <map>

#include "AbstractFactory.h"

// Allows deffered creating of resources
class ResourceManager
{
public:
	// Do nothing
	static void init() {}
		
	// Loads information about all assets
	static void init(const std::string& path);

	// Clears up all resources
	static void close();

	// Attaches resource factory to specified name
	template <class T, class... Args>
	static void bind(const std::string& name, Args&&... args)
	{
		static_assert(std::is_base_of<AbstractFactory, T>::value,
			"Template parameter of function ResourceManager::bind must be a child class of ResourceFactory class");

		std::unique_ptr<AbstractFactory> factory = std::make_unique<T>(std::forward<Args>(args)...);

		auto key = std::make_pair(name, factory->getStoredTypeIndex());

		auto it = m_factories.find(key);
		if (it == m_factories.end()) {
			m_factories[key] = std::move(factory);
		}
	}

	// Detaches resource factory from specified name
	template <class T>
	static void unbind(const std::string& name)
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));

		auto it = m_factories.find(key);
		if (it != m_factories.end()) {
			m_factories.erase(it);
		}
	}

	// Returns resource of specified name and type
	// If there is no such resource nullptr will be returned
	template <class T>
	static T* get(const std::string& name)
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));

		auto it = m_factories.find(key);
		if (it == m_factories.end()) {
			return nullptr;
		}
		else {
			return reinterpret_cast<T*>(it->second->load());
		}
	}

	// Clear specified resource, but don't delete it from map
	template <class T>
	static void clear(const std::string& name)
	{
		auto key = std::make_pair(name, std::type_index(typeid(T)));

		auto it = m_factories.find(key);
		if (it != m_factories.end()) {
			return it->second->clear();
		}
	}
private:
	static std::map<std::pair<std::string, std::type_index>, std::unique_ptr<AbstractFactory>> m_factories;
};