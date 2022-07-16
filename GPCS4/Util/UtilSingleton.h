#ifndef SINGLETON_H
#define SINGLETON_H

#ifndef NULL
#define NULL 0
#endif

namespace util
{
	template <typename T>
	class Singleton
	{
	public:
		static T* instance()
		{
			if (mInstance == NULL)  //TODO: Double-Check
			{
				mInstance = new T();
			}

			return mInstance;
		}

	protected:
		Singleton()
		{
		}

		virtual ~Singleton()
		{
			mInstance = NULL;
		}

	private:
		Singleton(const Singleton& source);
		Singleton& operator=(const Singleton& source);

	private:
		class Deleter
		{
		public:
			~Deleter();
		};

		static T*      mInstance;
		static Deleter mDeleter;
	};

	template <typename T>
	T* Singleton<T>::mInstance = NULL;

	template <typename T>
	Singleton<T>::Deleter::~Deleter()
	{
		if (Singleton<T>::mInstance)
		{
			delete Singleton<T>::mInstance;
			Singleton<T>::mInstance = NULL;
		}
	}

}  // namespace util




#endif // ! defined SINGLETON_H
