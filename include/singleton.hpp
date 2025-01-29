/*
 * Singleton.hpp
 *
 */

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T>
class TSingleton
{
protected:
	TSingleton(void)
	{
	}

	~TSingleton()
	{
	}

public:
	static T *getInstance(void)
	{
		static T _singleton;

		return &_singleton;
	}
};

#endif // SINGLETON_HPP
