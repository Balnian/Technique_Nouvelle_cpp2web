#pragma once
#include <functional>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

using namespace std;

class ThreadPool
{
	struct Base_task
	{
		virtual void execute() = 0;
		virtual ~Base_task() = default;
	};

	template<class F>
	class Task : public Base_task
	{
		F func;
	public:
		Task(F && func) :func{ move(func) }
		{}

		void execute()
		{
			func();
		}
	};

	mutex m_lockNewTask;
	condition_variable new_task;
	mutable mutex m_lockQueue;
	mutable vector<thread> m_pool;

	deque<unique_ptr<Base_task>> m_Tasks;
	atomic<bool> m_closing;



public:
	ThreadPool(unsigned int nb_Thread = thread::hardware_concurrency());
	~ThreadPool();
	template<class F>
	void addTask(F && func)
	{
		std::lock(m_lockQueue, m_lockNewTask);
		std::unique_lock<std::mutex> verrou_ntm{ m_lockNewTask, std::adopt_lock };
		std::lock_guard<std::mutex> verrou_m{ m_lockQueue, std::adopt_lock };
		//auto movFunc = bind([](F && func, Args && ... args) {func(forward<Args>(args)...); },func, forward<Args>(args)...);
		m_Tasks.emplace_back(make_task(func));
		new_task.notify_one();
	}

	void halt()
	{
		m_closing = true;
		std::unique_lock<std::mutex> verrou{ m_lockNewTask };
		new_task.notify_all();
	}

	void wait_end() const
	{
		for (auto & t : m_pool)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
	}

private:
	template<class F>
	static std::unique_ptr<Task<F>> make_task(F && func)
	{
		return std::make_unique<Task<F>>(Task<F>(func));
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> verrou{ m_lockQueue };
		return m_Tasks.empty();
	}

	bool closing()const { return m_closing; }

	unique_ptr<Base_task> next_task();

	bool stop_working() const
	{
		return closing() && empty();
	}

	


};

