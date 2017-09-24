#pragma once
#ifndef BE_BELUA_VERSION_HPP_
#define BE_BELUA_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_BELUA_VERSION_MAJOR 0
#define BE_BELUA_VERSION_MINOR 1
#define BE_BELUA_VERSION_REV 8

/*!! include('common/version', 'be::belua') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_BELUA_VERSION (BE_BELUA_VERSION_MAJOR * 100000 + BE_BELUA_VERSION_MINOR * 1000 + BE_BELUA_VERSION_REV)
#define BE_BELUA_VERSION_STRING "be::belua " BE_STRINGIFY(BE_BELUA_VERSION_MAJOR) "." BE_STRINGIFY(BE_BELUA_VERSION_MINOR) "." BE_STRINGIFY(BE_BELUA_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
