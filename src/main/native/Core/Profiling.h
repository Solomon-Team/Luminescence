#pragma once

#ifdef TRACY_ENABLE
    #include <tracy/Tracy.hpp>
#else
    #define ZoneScoped
#endif