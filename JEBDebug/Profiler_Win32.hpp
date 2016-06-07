/* JEBDebug: C++ macros and functions for debugging and profiling
 * Copyright 2014 Jan Erik Breimo
 * All rights reserved.
 *
 * This file is distributed under the BSD License.
 * License text is included with the source distribution.
 */
#ifndef JEBDEBUG_PROFILER_HPP
#define JEBDEBUG_PROFILER_HPP

#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include "Debug.hpp"

namespace JEBDebug {

    namespace Profiler_Internal
    {

        template <typename InpIt1, typename InpIt2>
        std::pair<InpIt1, InpIt2> mismatch(InpIt1 beg, InpIt1 end,
                                           InpIt2 cmpBeg, InpIt2 cmpEnd)
        {
            while (beg != end && cmpBeg != cmpEnd && *beg == *cmpBeg)
            {
                ++beg;
                ++cmpBeg;
            }
            return std::make_pair(beg, cmpBeg);
        }

    }

    class ProfilerSection
    {
    public:
        ProfilerSection(const std::string& fileName,
                        const std::string& funcName,
                        const std::string& name,
                        size_t lineNo)
            : m_FileName(fileName),
              m_FuncName(funcName),
              m_Name(name),
              m_LineNo(lineNo)
        {}

        const std::string& fileName() const {return m_FileName;}
        const std::string& funcName() const {return m_FuncName;}
        const std::string& name() const {return m_Name;}
        size_t lineNo() const {return m_LineNo;}
    private:
        std::string m_FileName;
        std::string m_FuncName;
        std::string m_Name;
        size_t m_LineNo;
    };

    inline bool operator<(const ProfilerSection& a, const ProfilerSection& b)
    {
        if (a.lineNo() != b.lineNo())
            return a.lineNo() < b.lineNo();
        if (a.funcName() != b.funcName())
            return a.funcName() < b.funcName();
        if (a.name() != b.name())
            return a.name() < b.name();
        return a.fileName() < b.fileName();
    }

    class ProfilerData
    {
    public:
        ProfilerData()
         : m_Count(0),
           m_AccTime(0),
           m_MinTime(std::numeric_limits<int64_t>::max()),
           m_MaxTime(std::numeric_limits<int64_t>::min())
        {}

        void addTime(int64_t time)
        {
            ++m_Count;
            m_AccTime += time;
            if (time < m_MinTime)
                m_MinTime = time;
            if (time > m_MaxTime)
                m_MaxTime = time;
        }

        size_t count() const
        {
            return m_Count;
        }

        double accTime() const
        {
            return m_AccTime / frequency();
        }

        double avgTime() const
        {
            return m_Count == 0 ? 0.0 : accTime() / m_Count;
        }

        double minTime() const
        {
            return m_MinTime / frequency();
        }

        double maxTime() const
        {
            return m_MaxTime / frequency();
        }
    private:
        static double frequency()
        {
            LARGE_INTEGER ticksPersSecond;
            QueryPerformanceFrequency(&ticksPersSecond);
            return static_cast<double>(ticksPersSecond.QuadPart);
        }

        size_t m_Count;
        int64_t m_AccTime;
        int64_t m_MinTime;
        int64_t m_MaxTime;
    };

    class Profiler
    {
    public:
        static Profiler& instance()
        {
            static Profiler profiler;
            return profiler;
        }

        void clear()
        {
            m_Profiles.clear();
        }

        void addTime(const std::string& fileName,
                     const std::string& funcName,
                     const std::string& name,
                     size_t lineNo,
                     int64_t time)
        {
            ProfilerSection section(fileName, funcName, name, lineNo);
            auto it = m_Profiles.find(section);
            if (it == m_Profiles.end())
            {
                it = m_Profiles.insert(
                        std::make_pair(section, ProfilerData())).first;
                m_Sequence.push_back(it);
            }
            it->second.addTime(time);
        }

        void write(std::ostream& os) const
        {
            size_t nameWidth = 0;
            for (auto it = m_Sequence.begin(); it != m_Sequence.end(); ++it)
            {
                nameWidth = std::max(nameWidth,
                                     (*it)->first.funcName().size());
            }

            std::string prefix = commonFileNamePrefix();

            nameWidth = std::min(nameWidth, (size_t)30);
            for (auto it = m_Sequence.begin(); it != m_Sequence.end(); ++it)
            {
                os << std::left << std::setw(nameWidth)
                   << (*it)->first.funcName()
                   << std::setprecision(4) << std::fixed
                   << "  " << (*it)->second.count()
                   << " " << (*it)->second.accTime()
                   << " " << (*it)->second.minTime()
                   << " " << (*it)->second.maxTime()
                   << " " << (*it)->second.avgTime()
                   << "  " << (*it)->first.fileName()
                   << "[" << (*it)->first.lineNo() << "]"
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
        Profiler() {}

        std::string commonFileNamePrefix() const
        {
            if (m_Profiles.empty())
              return std::string();

            auto it = m_Profiles.begin();
            std::string prefix = it->first.fileName();
            for (++it; it != m_Profiles.end(); ++it)
            {
                auto newEnd = Profiler_Internal::mismatch(
                        prefix.begin(),
                        prefix.end(),
                        it->first.fileName().begin(),
                        it->first.fileName().end()).first;
                if (newEnd != prefix.end())
                  prefix = std::string(prefix.begin(), newEnd);
            }
            return prefix;
        }

        typedef std::map<ProfilerSection, ProfilerData> ProfileSectionLookup;
        ProfileSectionLookup m_Profiles;

        std::vector<ProfileSectionLookup::const_iterator> m_Sequence;
    };

    class ProfilerTimer
    {
    public:
        ProfilerTimer(const std::string& fileName,
                      const std::string& funcName,
                      size_t lineNo,
                      const std::string& name = std::string())
            : m_FileName(fileName),
              m_FuncName(funcName),
              m_Name(name),
              m_LineNo(lineNo),
              m_StartTime(clock())
        {}

        ~ProfilerTimer()
        {
            auto endTime = clock();
            auto elapsed = endTime - m_StartTime;
            Profiler::instance().addTime(m_FileName, m_FuncName, m_Name,
                                          m_LineNo, elapsed);
        }

    private:
        static int64_t clock()
        {
            LARGE_INTEGER ticks;
            QueryPerformanceCounter(&ticks);
            return ticks.QuadPart;
        }

        std::string m_FileName;
        std::string m_FuncName;
        std::string m_Name;
        size_t m_LineNo;
        int64_t m_StartTime;
    };

}

#define JEB_PROFILE() \
    JEBDebug::ProfilerTimer JEB_DETAIL_UNIQUE_NAME(profile) \
        (__FILE__, __FUNCTION__, __LINE__)

#define JEB_PROFILER_REPORT() \
    JEBDebug::Profiler::instance().write()
