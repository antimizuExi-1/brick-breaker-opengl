#include "brick/Shape.h"

// load shape's vertices data and compile shaders
extern void Brk_Rectangle_LoadResource();
extern void Brk_Circle_LoadResource();
// clean up resource
extern void Brk_Rectangle_CleanupResource(void);
extern void Brk_Circle_CleanupResource(void);

void prv_Brk_Shape_Init(void)
{
    Brk_Rectangle_LoadResource();
    Brk_Circle_LoadResource();
}

void prv_Brk_Shape_Cleanup(void)
{
    Brk_Rectangle_CleanupResource();
    Brk_Circle_CleanupResource();
}
