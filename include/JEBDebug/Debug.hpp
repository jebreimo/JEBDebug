/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <string>

namespace JEBDebug
{
    static std::ostream* STREAM = &std::clog;
}

#define JEB_CHECKPOINT() \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)" << std::endl
#define JEB_MESSAGE(msg) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" << msg << std::endl
#define JEB_SHOW(var) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" #var " = " << (var) << std::endl
#define JEB_SHOW2(var1, var2) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" #var1 " = " << (var1) \
                << "\n\t" #var2 " = " << (var2) << std::endl
#define JEB_SHOW3(var1, var2, var3) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" #var1 " = " << (var1) \
                << "\n\t" #var2 " = " << (var2) \
                << "\n\t" #var3 " = " << (var3) << std::endl
#define JEB_SHOW4(var1, var2, var3, var4) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" #var1 " = " << (var1) \
                << "\n\t" #var2 " = " << (var2) \
                << "\n\t" #var3 " = " << (var3) \
                << "\n\t" #var4 " = " << (var4) << std::endl
#define JEB_SHOW5(var1, var2, var3, var4, var5) \
    *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                << "(...)\n\t" #var1 " = " << (var1) \
                << "\n\t" #var2 " = " << (var2) \
                << "\n\t" #var3 " = " << (var3) \
                << "\n\t" #var4 " = " << (var4) \
                << "\n\t" #var5 " = " << (var5) << std::endl

#define JEBDEBUG_UNIQUE_NAME_EXPANDER2(name, lineno) name##_##lineno
#define JEBDEBUG_UNIQUE_NAME_EXPANDER1(name, lineno) \
    JEBDEBUG_UNIQUE_NAME_EXPANDER2(name, lineno)
#define JEBDEBUG_UNIQUE_NAME(name) \
    JEBDEBUG_UNIQUE_NAME_EXPANDER1(name, __LINE__)

#define JEBDEBUG_AS_STRING_TRICK(a) #a
#define JEBDEBUG_AS_STRING(a) JEBDEBUG_AS_STRING_TRICK(a)

#define JEBDEBUG_CONTEXT() \
    (std::string(__FILE__ "[" JEBDEBUG_AS_STRING(__LINE__) "]: ") + \
    __FUNCTION__)

namespace JEBDebug
{
    using std::chrono::high_resolution_clock;

    class CpuTimer
    {
    public:
        void start()
        {
            m_StartTime = high_resolution_clock::now();
            m_IsStopped = false;
        }

        void stop()
        {
            auto endTime = high_resolution_clock::now();
            m_AccumulatedTime += endTime - m_StartTime;
            m_IsStopped = true;
        }

        void reset()
        {
            m_AccumulatedTime = high_resolution_clock::duration::zero();
            m_IsStopped = true;
        }

        double seconds() const
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

        bool stopped() const
        {
            return m_IsStopped;
        }
    private:
        high_resolution_clock::time_point m_StartTime;
        high_resolution_clock::duration m_AccumulatedTime;
        bool m_IsStopped = true;
    };

    template <typename String>
    class ScopedTimerImpl
    {
    public:
        typedef typename String::value_type CharT;
        typedef std::basic_ostream <CharT, std::char_traits<CharT>> Stream;

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
    std::basic_ostream <Char, Traits>& operator<<(
        std::basic_ostream <Char, Traits>& os,
        const CpuTimer& stopwatch)
    {
        return os << stopwatch.seconds();
    }
}

#define JEB_TIMEIT() \
    ::JEBDebug::ScopedTimer JEBDEBUG_UNIQUE_NAME(JEB_ScopedTimer) \
        (JEBDEBUG_CONTEXT() + "\n\telapsed time = ", *::JEBDebug::STREAM)

namespace JEBDebug { namespace internal {

    template <typename It>
    void write(std::ostream& os, It begin, It end)
    {
        if (begin != end)
            os << *begin++;
        for (; begin != end; begin++)
            os << ", " << *begin;
    }

    template <typename Container>
    void writeContainer(std::ostream& os, const Container& c)
    {
        write(os, std::begin(c), std::end(c));
    }

    template <typename It>
    void writePretty(std::ostream& os, It begin, It end)
    {
        size_t i = 0;
        for (; begin != end; begin++)
            os << std::setw(6) << i++ << ": " << *begin << "\n\t";
    }

    template <typename Container>
    void writeContainerPretty(std::ostream& os, const Container& c)
    {
        writePretty(os, std::begin(c), std::end(c));
    }
}}

#define JEB_SHOW_RANGE(begin, end) \
    do { \
        *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ \
                    << "]:\n\t(" #begin " ... " #end ") = ["; \
        ::JEBDebug::internal::write(*::JEBDebug::STREAM, (begin), (end)); \
        *::JEBDebug::STREAM << "]" << std::endl; \
    } while (false)

#define JEB_SHOW_CONTAINER(c) \
    do { \
        *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                    << "\n\t" #c " = ["; \
        ::JEBDebug::internal::writeContainer(*::JEBDebug::STREAM, (c)); \
        *::JEBDebug::STREAM << "]" << std::endl; \
    } while (false)

#define JEB_PRETTY_RANGE(begin, end) \
    do { \
        *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ \
                    << "]:\n\t(" #begin " ... " #end ") = [\n\t"; \
        ::JEBDebug::internal::writePretty(*::JEBDebug::STREAM, (begin), (end)); \
        *::JEBDebug::STREAM << "]" << std::endl; \
    } while (false)

#define JEB_PRETTY_CONTAINER(c) \
    do { \
        *::JEBDebug::STREAM << __FILE__ "[" << __LINE__ << "]: " << __FUNCTION__ \
                    << "\n\t" #c " = [\n\t"; \
        ::JEBDebug::internal::writeContainerPretty(*::JEBDebug::STREAM, (c)); \
        *::JEBDebug::STREAM << "]" << std::endl; \
    } while (false)
