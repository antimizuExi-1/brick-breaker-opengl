#ifndef BRICK_EXPORT_H
#define BRICK_EXPORT_H

#include "Config.h"

#ifdef BRICK_ENABLE_STATIC_EXPORT
#define BrkAPI
#elif defined (BRICK_ENABLE_DYNAMIC_EXPORT)
#define BrkAPI  __attribute__((visibility("default")))
#else
#define BrkAPI
#endif

#endif //BRICK_EXPORT_H
