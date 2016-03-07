#include "ThreadPool.h"

ThreadPool::ThreadPool(unsigned int nb_Thread)
	:m_closing{}
{
	for (decltype(nb_Thread) i = 0; i < nb_Thread; ++i)
		m_pool.emplace_back([&]() {
		while (!stop_working())
		{
			auto quoi = next_task();
			if (quoi)
				quoi->execute();
		}
	});
}

ThreadPool::~ThreadPool()
{
	halt();
	wait_end();
}

unique_ptr<ThreadPool::Base_task> ThreadPool::next_task()
{
	if (empty())
	{
		std::unique_lock<std::mutex> verrou{ m_lockNewTask };
		new_task.wait(verrou, [&]() { return closing() || !empty(); });
	}
	std::lock_guard<std::mutex> verrou{ m_lockQueue };
	if (m_Tasks.empty())
		return nullptr;
	auto p = move(m_Tasks.front());
	m_Tasks.pop_front();
	return p;
}