/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#pragma once

#include <algorithm>
#include <chrono>
#include <limits>
#include <map>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>

#ifndef JEB_MULTI_UNIT_PROFILER
    #define JEB_INSTANTIATE_PROFILER
#endif

namespace JEBDebug
{
    class ProfilerSection
    {
    public:
        ProfilerSection(std::string_view file_name,
                        std::string_view func_name,
                        size_t line_no)
            : file_name(file_name),
              func_name(func_name),
              line_no(line_no)
        {}

        std::string_view file_name;
        std::string_view func_name;
        std::string_view name;
        size_t line_no;
    };

    inline bool operator<(const ProfilerSection& a, const ProfilerSection& b)
    {
        if (a.line_no != b.line_no)
            return a.line_no < b.line_no;
        if (a.func_name != b.func_name)
            return a.func_name < b.func_name;
        return a.file_name < b.file_name;
    }

    class ProfilerData
    {
    public:
        using Duration = std::chrono::high_resolution_clock::duration;

        ProfilerData()
         : count_(0),
           acc_time_(0),
           min_time_(std::numeric_limits<Duration>::max()),
           max_time_(-std::numeric_limits<Duration>::min())
        {}

        void add_time(Duration total_time, Duration time)
        {
            ++count_;
            acc_time_ += time;
            if (total_time < min_time_)
                min_time_ = total_time;
            if (total_time > max_time_)
                max_time_ = total_time;
        }

        [[nodiscard]] size_t count() const
        {
            return count_;
        }

        [[nodiscard]] double acc_time() const
        {
            using namespace std::chrono;
            return duration_cast<duration<double>>(acc_time_).count();
        }

        [[nodiscard]] double min_time() const
        {
            using namespace std::chrono;
            return duration_cast<duration<double>>(min_time_).count();
        }

        [[nodiscard]] double max_time() const
        {
            using namespace std::chrono;
            return duration_cast<duration<double>>(max_time_).count();
        }
    private:
        size_t count_;
        Duration acc_time_;
        Duration min_time_;
        Duration max_time_;
    };

    class Profiler
    {
    public:
        static Profiler& instance()
        {
            return instance_;
        }

        void clear()
        {
            profiles_.clear();
        }

        void start_timer(std::string_view file_name,
                         std::string_view func_name,
                         size_t line_no)
        {
            call_stack_.emplace_back(
                ProfilerSection(file_name, func_name, line_no),
                Clock::now(),
                Duration());
        }

        void end_timer()
        {
            auto end_time = Clock::now();
            auto& [key, start_time, sub_duration] = call_stack_.back();
            auto elapsed = end_time - start_time;
            auto it = profiles_.find(key);
            if (it == profiles_.end())
            {
                it = profiles_.emplace(key, ProfilerData()).first;
                sequence_.push_back(it);
            }
            it->second.add_time(elapsed, elapsed - sub_duration);
            call_stack_.pop_back();
            if (!call_stack_.empty())
                std::get<2>(call_stack_.back()) += elapsed;
        }

        void write(std::ostream& os) const
        {
            size_t nameWidth = 0;
            for (auto it = sequence_.begin(); it != sequence_.end(); ++it)
            {
                nameWidth = std::max(nameWidth,
                                     (*it)->first.func_name.size());
            }

            nameWidth = std::min(nameWidth, size_t(30));
            for (auto it = sequence_.begin(); it != sequence_.end(); ++it)
            {
                os << std::left << std::setw(std::streamsize(nameWidth))
                   << (*it)->first.func_name
                   << std::setprecision(4) << std::fixed
                   << "  " << (*it)->second.count()
                   << " " << (*it)->second.acc_time()
                   << " " << (*it)->second.min_time()
                   << " " << (*it)->second.max_time()
                   << "  " << (*it)->first.file_name
                   #ifdef _MSC_VER
                   << "(" << (*it)->first.line_no << ")"
                   #else
                   << ":" << (*it)->first.line_no()
                   #endif
                   << std::endl;
            }
        }

        void write() const
        {
            write(std::cout);
        }

        void write(const std::string& filePath) const
        {
            std::ofstream file(filePath);
            write(file);
        }
    private:
        Profiler() = default;

        static Profiler instance_;

        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using Duration = Clock::duration;
        std::vector<std::tuple<ProfilerSection, TimePoint, Duration>> call_stack_;

        using ProfileSectionLookup = std::map<ProfilerSection, ProfilerData>;
        ProfileSectionLookup profiles_;

        std::vector<ProfileSectionLookup::const_iterator> sequence_;
    };

#ifdef JEB_INSTANTIATE_PROFILER
    Profiler Profiler::instance_;
#endif

    class ProfilerTimer
    {
    public:
        ProfilerTimer(std::string_view file_name,
                      std::string_view func_name,
                      size_t line_no)
        {
            Profiler::instance().start_timer(file_name, func_name, line_no);
        }

        ~ProfilerTimer()
        {
            Profiler::instance().end_timer();
        }
    };
}

#define JEBPROFILER_UNIQUE_NAME_EXPANDER2(name, lineno) name##_##lineno
#define JEBPROFILER_UNIQUE_NAME_EXPANDER1(name, lineno) \
    JEBPROFILER_UNIQUE_NAME_EXPANDER2(name, lineno)
#define JEBPROFILER_UNIQUE_NAME(name) \
    JEBPROFILER_UNIQUE_NAME_EXPANDER1(name, __LINE__)

#ifdef _MSC_VER
    #define JEB_PROFILE() \
        ::JEBDebug::ProfilerTimer JEBPROFILER_UNIQUE_NAME(profile) \
            (__FILE__, __FUNCSIG__, __LINE__)
#else
    #define JEB_PROFILE() \
        ::JEBDebug::ProfilerTimer JEBPROFILER_UNIQUE_NAME(profile) \
            (__FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

#define JEB_PROFILER_REPORT() \
    ::JEBDebug::Profiler::instance().write()
