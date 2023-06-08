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
#include <iterator>
#include <ostream>
#include <string>
#if defined(_WIN32) && defined(JEBDEBUG_STREAM_TO_DEBUGGER)
    #include <sstream>
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

namespace JEBDebug
{
#if defined(_WIN32) && defined(JEBDEBUG_STREAM_TO_DEBUGGER)
    class DebugStreamBuffer : public std::stringbuf
    {
    public:
        ~DebugStreamBuffer() override
        {
            DebugStreamBuffer::sync();
        }

    private:
        int sync() override
        {
            OutputDebugStringA(str().c_str());
            str({});
            return std::stringbuf::sync();
        }
    };

    class DebugStream : public std::ostream
    {
    public:
        DebugStream()
            : std::ostream(new DebugStreamBuffer)
        {}

        ~DebugStream() override
        {
            delete std::ostream::rdbuf();
        }
    };
#endif

    class Stream
    {
    public:
        Stream()
        {
#if defined(_WIN32) && defined(JEBDEBUG_STREAM_TO_DEBUGGER)
            if (IsDebuggerPresent())
                set_stream(debug_stream_);
#endif
        }

        std::ostream& operator()()
        {
            if (!stream_)
                stream_ = &std::clog;
            return *stream_;
        }

        void set_stream(std::ostream& stream)
        {
            stream_ = &stream;
        }
    private:
#if defined(_WIN32) && defined(JEBDEBUG_STREAM_TO_DEBUGGER)
        DebugStream debug_stream_;
#endif
        std::ostream* stream_ = nullptr;
    };

    static Stream STREAM;
}

#ifdef _MSC_VER
    #define _JEBDEBUG_STREAM_LOCATION_2(file, line) \
        file "(" #line "): " << __FUNCSIG__
#else
    #define _JEBDEBUG_STREAM_LOCATION_2(file, line) \
        file ":" #line ": " << __PRETTY_FUNCTION__
#endif

#define _JEBDEBUG_STREAM_LOCATION_1(file, line) \
    _JEBDEBUG_STREAM_LOCATION_2(file, line)

#define _JEBDEBUG_STREAM_LOCATION() \
    _JEBDEBUG_STREAM_LOCATION_1(__FILE__, __LINE__)


#define JEB_CHECKPOINT() \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() << std::endl; \
    } while (false)

#define JEB_MESSAGE(msg) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
            << ":\n\t" << msg << std::endl; \
    } while (false)


// This "recursive" implementation of JEB_SHOW is inspired by the following
// reply on stackoverflow: https://stackoverflow.com/a/5048661
// To make it work in Visual Studio I had to use this
// one as well: https://stackoverflow.com/a/24837037
#define _JEBDEBUG_GLUE(x, y) x y

#define _JEBDEBUG_RETURN_ARG_COUNT(_1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, count, ...) count
#define _JEBDEBUG_EXPAND_ARGS(args) _JEBDEBUG_RETURN_ARG_COUNT args
#define _JEBDEBUG_COUNT_ARGS_MAX10(...) _JEBDEBUG_EXPAND_ARGS((__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define _JEBDEBUG_OVERLOAD_MACRO2(name, count) name##count
#define _JEBDEBUG_OVERLOAD_MACRO1(name, count) _JEBDEBUG_OVERLOAD_MACRO2(name, count)
#define _JEBDEBUG_OVERLOAD_MACRO(name, count) _JEBDEBUG_OVERLOAD_MACRO1(name, count)

#define _JEBDEBUG_CALL_OVERLOAD(name, ...) _JEBDEBUG_GLUE(_JEBDEBUG_OVERLOAD_MACRO(name, _JEBDEBUG_COUNT_ARGS_MAX10(__VA_ARGS__)), (__VA_ARGS__))

#define _JEBDEBUG_SHOW_1(var) \
    << "\n\t" #var " = " << (var)

#define _JEBDEBUG_SHOW_2(var1, var2) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_1(var2)

#define _JEBDEBUG_SHOW_3(var1, var2, var3) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_2(var2, var3)

#define _JEBDEBUG_SHOW_4(var1, var2, var3, var4) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_3(var2, var3, var4)

#define _JEBDEBUG_SHOW_5(var1, var2, var3, var4, var5) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_4(var2, var3, var4, var5)

#define _JEBDEBUG_SHOW_6(var1, var2, var3, var4, var5, var6) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_5(var2, var3, var4, var5, var6)

#define _JEBDEBUG_SHOW_7(var1, var2, var3, var4, var5, var6, var7) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_6(var2, var3, var4, var5, var6, var7)

#define _JEBDEBUG_SHOW_8(var1, var2, var3, var4, var5, var6, var7, var8) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_7(var2, var3, var4, var5, var6, var7, var8)

#define _JEBDEBUG_SHOW_9(var1, var2, var3, var4, var5, var6, var7, var8, var9) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_8(var2, var3, var4, var5, var6, var7, var8, var9)

#define _JEBDEBUG_SHOW_10(var1, var2, var3, var4, var5, var6, var7, var8, var9, var10) \
    << "\n\t" #var1 " = " << (var1) \
    _JEBDEBUG_SHOW_9(var2, var3, var4, var5, var6, var7, var8, var9, var10)

#define JEB_SHOW(...) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() << ":" \
            _JEBDEBUG_CALL_OVERLOAD(_JEBDEBUG_SHOW_, __VA_ARGS__) \
            << std::endl; \
    } while (false)

#define _JEBDEBUG_UNIQUE_NAME_EXPANDER2(name, lineno) name##_##lineno
#define _JEBDEBUG_UNIQUE_NAME_EXPANDER1(name, lineno) \
    _JEBDEBUG_UNIQUE_NAME_EXPANDER2(name, lineno)
#define _JEBDEBUG_UNIQUE_NAME(name) \
    _JEBDEBUG_UNIQUE_NAME_EXPANDER1(name, __LINE__)

#define _JEBDEBUG_AS_STRING_TRICK(a) #a
#define _JEBDEBUG_AS_STRING(a) _JEBDEBUG_AS_STRING_TRICK(a)

#ifdef _MSC_VER
    #define _JEBDEBUG_CONTEXT() \
        (std::string(__FILE__ "(" _JEBDEBUG_AS_STRING(__LINE__) "): ") + \
        __FUNCSIG__)
#else
    #define _JEBDEBUG_CONTEXT() \
        (std::string(__FILE__ ":" _JEBDEBUG_AS_STRING(__LINE__) ": ") + \
        __PRETTY_FUNCTION__)
#endif

namespace JEBDebug
{
    class CpuTimer
    {
    public:
        using Clock = std::chrono::high_resolution_clock;

        void start()
        {
            start_time_ = Clock::now();
            is_stopped_ = false;
        }

        void stop()
        {
            auto end_time = Clock::now();
            accumulated_time_ += end_time - start_time_;
            is_stopped_ = true;
        }

        [[nodiscard]] double seconds() const
        {
            using namespace std::chrono;
            auto tmp = accumulated_time_;
            if (!stopped())
            {
                auto end_time = Clock::now();
                tmp += end_time - start_time_;
            }
            return duration<double>(tmp).count();
        }

        [[nodiscard]] bool stopped() const
        {
            return is_stopped_;
        }

    private:
        Clock::time_point start_time_ = {};
        Clock::duration accumulated_time_ = {};
        bool is_stopped_ = true;
    };

    template <typename String>
    class ScopedTimerImpl
    {
    public:
        typedef typename String::value_type CharT;
        typedef std::basic_ostream<CharT, std::char_traits<CharT>> Stream;

        ScopedTimerImpl(const String& label, Stream& stream)
            : label_(label),
              stream_(stream)
        {
            timer_.start();
        }

        ~ScopedTimerImpl()
        {
            timer_.stop();
            stream_ << label_ << timer_ << std::endl;
        }

    private:
        CpuTimer timer_;
        String label_;
        Stream& stream_;
    };

    typedef ScopedTimerImpl<std::string> ScopedTimer;

    template <typename Char, typename Traits>
    std::basic_ostream<Char, Traits>& operator<<(
        std::basic_ostream<Char, Traits>& os,
        const CpuTimer& stopwatch)
    {
        return os << stopwatch.seconds();
    }
}

#define JEB_TIMEIT() \
    ::JEBDebug::ScopedTimer _JEBDEBUG_UNIQUE_NAME(JEB_ScopedTimer) \
        (_JEBDEBUG_CONTEXT() + ":\n\telapsed time = ", ::JEBDebug::STREAM())

namespace JEBDebug { namespace internal
{
    template <typename It>
    void write(std::ostream& os, It begin, It end)
    {
        if (begin != end)
            os << *begin++;
        for (; begin != end; begin++)
            os << ", " << *begin;
    }

    template <typename Container>
    void write_container(std::ostream& os, const Container& c)
    {
        write(os, std::begin(c), std::end(c));
    }

    template <typename It>
    void write_pretty(std::ostream& os, It begin, It end)
    {
        size_t i = 0;
        for (; begin != end; begin++)
            os << std::setw(6) << i++ << ": " << *begin << "\n\t";
    }

    template <typename Container>
    void write_containerPretty(std::ostream& os, const Container& c)
    {
        write_pretty(os, std::begin(c), std::end(c));
    }
}}

#define JEB_SHOW_RANGE_FLAT(begin, end) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
            << ":\n\t" #begin " ... " #end " = ["; \
        ::JEBDebug::internal::write(::JEBDebug::STREAM(), (begin), (end)); \
        ::JEBDebug::STREAM() << "]" << std::endl; \
    } while (false)

#define JEB_SHOW_CONTAINER_FLAT(c) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
            << ":\n\t" #c " = ["; \
        ::JEBDebug::internal::write_container(::JEBDebug::STREAM(), (c)); \
        ::JEBDebug::STREAM() << "]" << std::endl; \
    } while (false)

#define JEB_SHOW_RANGE(begin, end) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
                    << ":\n\t" #begin " ... " #end " = [\n\t"; \
        ::JEBDebug::internal::write_pretty(::JEBDebug::STREAM(), (begin), (end)); \
        ::JEBDebug::STREAM() << "]" << std::endl; \
    } while (false)

#define JEB_SHOW_CONTAINER(c) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
            << ":\n\t" #c " = [\n\t"; \
        ::JEBDebug::internal::write_containerPretty(::JEBDebug::STREAM(), (c)); \
        ::JEBDebug::STREAM() << "]" << std::endl; \
    } while (false)

namespace JEBDebug
{
    namespace internal
    {
        inline size_t print_hex_numbers(std::ostream& stream,
                                        const void* data, size_t num_bytes,
                                        size_t num_numbers)
        {
            auto flags = stream.setf(std::ios::hex, std::ios::basefield);
            auto fill = stream.fill('0');
            size_t i = 0;
            auto n = std::min(num_bytes, num_numbers);
            const auto* cdata = static_cast<const unsigned char*>(data);
            for (; i < n; ++i)
                stream << ' ' << std::setw(2) << unsigned(cdata[i]);
            for (; i < num_numbers; ++i)
                stream << "   ";
            stream.fill(fill);
            stream.flags(flags);
            return n;
        }

        inline size_t print_characters(std::ostream& stream,
                                       const void* data, size_t num_bytes,
                                       size_t num_chars)
        {
            size_t i = 0;
            auto n = std::min(num_bytes, num_chars);
            const auto* cdata = static_cast<const unsigned char*>(data);
            for (; i < n; ++i)
            {
                if (32 <= cdata[i] && cdata[i] < 127)
                    stream.put(char(cdata[i]));
                else
                    stream.put('.');
            }
            for (; i < num_chars; ++i)
                stream.put(' ');
            return n;
        }
    }

    inline void hexdump(std::ostream& stream, const void* data, size_t size,
                        size_t columns = 16)
    {
        auto flags = stream.setf(std::ios::hex, std::ios::basefield);
        auto fill = stream.fill('0');
        const auto* cdata = static_cast<const unsigned char*>(data);
        const auto* end = cdata + size;
        auto digits = [](size_t n)
        {
            size_t i = 1;
            while (n >>= 4u)
                ++i;
            return i;
        }(size);
        while (cdata != end)
        {
            stream << std::setw(std::streamsize(digits))
                   << (intptr_t(cdata) - intptr_t(data));
            const auto* tmp_cdata = cdata;
            for (int i = 0; i < 2; ++i)
            {
                stream.put(' ');
                tmp_cdata += internal::print_hex_numbers(stream, tmp_cdata,
                                                        end - tmp_cdata,
                                                        columns / 2);
            }
            stream << "  ";
            cdata += internal::print_characters(stream, cdata,
                                                end - cdata,
                                                columns);
            stream.put('\n');
        }
        stream.fill(fill);
        stream.flags(flags);
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    void hexdump(std::ostream& stream, const T& value)
    {
        hexdump(stream, &value, sizeof(value));
    }

    template <typename T, std::enable_if_t<!std::is_arithmetic_v<T>, int> = 0>
    void hexdump(std::ostream& stream, const T& value)
    {
        using std::data, std::size;
        hexdump(stream, data(value), size(value) * sizeof(decltype(*data(value))));
    }
}

/**
 * @brief Display a nice hexdump of the input variable.
 *
 * The arguments can be
 * - a single variable that supports data(v) and size(v), for instance a
 *   std::vector.
 * - two values, data and size, where data is a pointer to the data to be
 *   displayed, and size is the number of bytes to display.
 */
#define JEB_HEXDUMP(...) \
    do { \
        ::JEBDebug::STREAM() << _JEBDEBUG_STREAM_LOCATION() \
                    << ":\n" #__VA_ARGS__ ":\n"; \
        ::JEBDebug::hexdump(::JEBDebug::STREAM(), __VA_ARGS__); \
        ::JEBDebug::STREAM() << std::endl; \
    } while (false)
