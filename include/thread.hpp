#ifndef THREAD_HPP
#define THREAD_HPP

/*****************************************************************************/
/*** HEADER INCLUDES *********************************************************/

#if defined(__linux__)
    #include <pthread.h>
    #include <sched.h>
#endif  // __linux__

/// \cond
#include <string_view>
#include <thread>
#include <utility>

/// \endcond

/*****************************************************************************/
/*** CLASSES *****************************************************************/

class thread_t
{
public:
    thread_t() = default;

    template <typename F, typename... Args>
    explicit thread_t(F&& fn, Args&&... args)
        : m_thread(std::forward<F>(fn), std::forward<Args>(args)...)
    {}

    thread_t(const thread_t& /* that */) = delete;
    thread_t(thread_t&& /* that */) = default;

    ~thread_t()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    thread_t& operator=(const thread_t& /* that */) = delete;
    thread_t& operator=(thread_t&& /* that */) = default;

    [[nodiscard]] bool joinable() const noexcept
    {
        return m_thread.joinable();
    }

    void join()
    {
        m_thread.join();
    }

    void set_name(std::string_view name)
    {
        if (m_thread.get_id() == std::thread::id())
        {
            return;
        }

#if defined(__linux__)
        pthread_setname_np(m_thread.native_handle(), name.data());
#else
        (void)name;
#endif
    }

    void set_affinity(int core)
    {
        if (m_thread.get_id() == std::thread::id() || core == -1)
        {
            return;
        }

#if defined(__linux__)
        cpu_set_t cpuset{};

        CPU_ZERO(&cpuset);
        CPU_SET(core, &cpuset);

        pthread_setaffinity_np(m_thread.native_handle(), sizeof(cpuset), &cpuset);
#endif
    }

private:
    std::thread m_thread;
};

#endif  // THREAD_HPP
