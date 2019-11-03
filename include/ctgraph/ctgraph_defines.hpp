#pragma once

#include "ctgraph_config.hpp"


#ifdef _DEBUG
#define CTG_DEBUG true
#else
#define CTG_DEBUG false
#endif


#include <string>


namespace ctgraph
{

constexpr static const int version_major = CTG_VERSION_MAJOR;
constexpr static const int version_minor = CTG_VERSION_MINOR;
constexpr static const int version_patch = CTG_VERSION_PATCH;
constexpr static const bool debug_build = CTG_DEBUG;

struct BuildInfo
{
    
    [[nodiscard]] constexpr static int version_major() noexcept
    {
        return ctgraph::version_major;
    }

    [[nodiscard]] constexpr static int version_minor() noexcept
    {
        return ctgraph::version_minor;
    }

    [[nodiscard]] constexpr static int version_patch() noexcept
    {
        return ctgraph::version_patch;
    }

    [[nodiscard]] constexpr static bool debug_build() noexcept
    {
        return ctgraph::debug_build;
    }

    [[nodiscard]] static std::string version()
    {
        return std::to_string(version_major()) + '.' + std::to_string(version_minor()) + '.' + std::to_string(version_patch());
    }
    
};


} // namespace ctgraph
