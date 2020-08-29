/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#pragma once

#include <chrono>
#include <ostream>
#include <string>

namespace JEBDebug
{
    using std::chrono::high_resolution_clock;

    class CpuTimer
    {
    public:
        CpuTimer();
        void start();
        void stop();
        void reset();
        double seconds() const;
        bool stopped() const;
    private:
        high_resolution_clock::time_point m_StartTime;
        high_resolution_clock::duration m_AccumulatedTime;
        bool m_IsStopped;
    };

    template <typename String>
    class ScopedTimerImpl
    {
    public:
        typedef typename String::value_type CharT;
        typedef std::basic_ostream<CharT, std::char_traits<CharT>> Stream;

        ScopedTimerImpl(const String& label, Stream& stream)
            : m_Label(label),
              m_Stream(stream)
        {
            m_Timer.start();
        }

        ~ScopedTimerImpl()
        {
            m_Timer.stop();
            m_Stream << m_Label << m_Timer << "\n";
        }
    private:
        CpuTimer m_Timer;
        String m_Label;
        Stream& m_Stream;
    };

    typedef ScopedTimerImpl<std::string> ScopedTimer;
    typedef ScopedTimerImpl<std::wstring> ScopedTimerW;

    template <typename Char, typename Traits>
    std::basic_ostream<Char, Traits>& operator<<(
            std::basic_ostream<Char, Traits>& os,
            const CpuTimer& stopwatch)
    {
      return os << stopwatch.seconds();
    }

    inline CpuTimer::CpuTimer()
        : m_IsStopped(true)
    {}

    inline void CpuTimer::start()
    {
        m_StartTime = high_resolution_clock::now();
        m_IsStopped = false;
    }

    inline void CpuTimer::stop()
    {
        auto endTime = high_resolution_clock::now();
        m_AccumulatedTime += endTime - m_StartTime;
        m_IsStopped = true;
    }

    inline void CpuTimer::reset()
    {
        m_AccumulatedTime = high_resolution_clock::duration::zero();
        m_IsStopped = true;
    }

    inline double CpuTimer::seconds() const
    {
        using namespace std::chrono;
        auto tmp = m_AccumulatedTime;
        if (!stopped())
        {
            auto endTime = high_resolution_clock::now();
            tmp += endTime - m_StartTime;
        }
        return duration<double>(tmp).count();
    }

    inline bool CpuTimer::stopped() const
    {
        return m_IsStopped;
    }
}
