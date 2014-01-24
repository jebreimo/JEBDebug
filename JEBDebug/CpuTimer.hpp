/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#ifndef JEBDEBUG_CPUTIMER_HPP
#define JEBDEBUG_CPUTIMER_HPP

#include <ctime>
#include <ostream>
#include <string>

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
    clock_t m_StartTime;
    clock_t m_AccumulatedTime;
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
    m_StartTime = clock();
}

inline void CpuTimer::stop()
{
    clock_t endTime = std::clock();
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
        return m_AccumulatedTime / (double)CLOCKS_PER_SEC;
    else
        return m_AccumulatedTime + (clock() - m_StartTime) / (double)CLOCKS_PER_SEC;
}

inline bool CpuTimer::stopped() const
{
    return m_StartTime == 0;
}

}

#endif
