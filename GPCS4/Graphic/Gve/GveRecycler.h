#pragma once

#include "GveCommon.h"

#include <mutex>
#include <array>

namespace gve
{;

/**
   * \brief Object recycler
   * 
   * Implements a thread-safe buffer that can store up to
   * a given number of objects of a certain type. This way,
   * Gve can efficiently reuse and reset objects instead
   * of destroying them and creating them anew.
   * \tparam T Type of the objects to store
   * \tparam N Maximum number of objects to store
   */

template <typename T, size_t N>
class GveRecycler
{

public:
	/**
     * \brief Retrieves an object if possible
     * 
     * Returns an object that was returned to the recycler
     * earier. In case no objects are available, this will
     * return \c nullptr and a new object has to be created.
     * \return An object, or \c nullptr
     */
	RcPtr<T> retrieveObject()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_objectId == 0)
		{
			return nullptr;
		}
			
		return m_objects.at(--m_objectId);
	}

	/**
     * \brief Returns an object to the recycler
     * 
     * If the buffer is full, the object will be destroyed
     * once the last reference runs out of scope. No further
     * action needs to be taken in this case.
     * \param [in] object The object to return
     */
	void returnObject(const RcPtr<T>& object)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_objectId < N)
		{
			m_objects.at(m_objectId++) = object;
		}
	}

private:
	std::mutex m_mutex;
	std::array<RcPtr<T>, N> m_objects;
	size_t m_objectId = 0;
};

}  // namespace gve