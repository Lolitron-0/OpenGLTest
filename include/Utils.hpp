#pragma once
#include <glm/glm.hpp>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texCoords;
};

enum class TextureType {
	DiffuseMap,
	SpecularMap
};

struct Texture {
	unsigned int id;
	TextureType type;
};


template<typename T>
class Singleton
{
public:
	static T& getInstance()
	{
		if (!Singleton::instance)
			Singleton::instance = createInstance();
		return *(Singleton::instance);
	}
protected:
	inline explicit Singleton()
	{
		static_assert(!Singleton::instance);
		Singleton::instance = static_cast<T*>(this);
	}

	inline ~Singleton()
	{
		Singleton::instance = 0;
	}

	static T* createInstance() { return new T(); }
private:
	static T* instance;

	inline explicit Singleton(Singleton const&) {}
	inline Singleton& operator=(Singleton const&) { return *this; }
};

template<typename T>
T* Singleton<T>::instance = 0;