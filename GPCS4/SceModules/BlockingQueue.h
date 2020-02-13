#pragma once

#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>

template <typename T>
class BlockingQueue
{
public:
	BlockingQueue()
	{
		m_maxElem = std::numeric_limits<size_t>::max();
	}

	BlockingQueue(size_t max)
	{
		m_maxElem = max;
	}

	void push(T const& value)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cvNotFull.wait(lock, [=] { return m_queue.size() < m_maxElem; });
			m_queue.push_front(value);
		}

		this->m_cvNotEmpty.notify_one();
	}

	T pop()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cvNotEmpty.wait(lock, [=] { return !m_queue.empty(); });

		T rc(std::move(m_queue.back()));
		m_queue.pop_back();

		lock.unlock();
		m_cvNotFull.notify_one();

		return rc;
	}

private:
	size_t m_maxElem;
	std::mutex m_mutex;
	std::condition_variable m_cvNotEmpty;
	std::condition_variable m_cvNotFull;
	std::deque<T> m_queue;
};
