/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#pragma once

#include <ostream>
#include <string>
#ifdef WIN32
    #define NOMINMAX
    #include <Windows.h>
#else
    #include <ctime>
#endif

namespace JEBDebug
{
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
        double getClock() const;
        double getClockFrequency() const;

        double m_StartTime;
        double m_AccumulatedTime;
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
      os << stopwatch.seconds();
      return os;
    }

    inline CpuTimer::CpuTimer()
        : m_StartTime(0),
          m_AccumulatedTime(0)
    {
    }

    inline void CpuTimer::start()
    {
        m_StartTime = getClock();
    }

    inline void CpuTimer::stop()
    {
        auto endTime = getClock();
        m_AccumulatedTime += endTime - m_StartTime;
        m_StartTime = 0;
    }

    inline void CpuTimer::reset()
    {
        m_AccumulatedTime = m_StartTime = 0;
    }

    inline double CpuTimer::seconds() const
    {
        if (stopped())
            return m_AccumulatedTime / getClockFrequency();
        else
            return m_AccumulatedTime
                   + (getClock() - m_StartTime) / getClockFrequency();
    }

    inline bool CpuTimer::stopped() const
    {
        return m_StartTime == 0;
    }

#ifdef WIN32

    inline double CpuTimer::getClock() const
    {
        LARGE_INTEGER ticks;
        QueryPerformanceCounter(&ticks);
        return static_cast<double>(ticks.QuadPart);
    }

    inline double CpuTimer::getClockFrequency() const
    {
        LARGE_INTEGER ticksPersSecond;
        QueryPerformanceFrequency(&ticksPersSecond);
        return static_cast<double>(ticksPersSecond.QuadPart);
    }

#else

    inline double CpuTimer::getClock() const
    {
        return static_cast<double>(clock());
    }

    inline double CpuTimer::getClockFrequency() const
    {
        return static_cast<double>(CLOCKS_PER_SEC);
    }

#endif

}
