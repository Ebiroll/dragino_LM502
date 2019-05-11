/***************************************************************************

*******************************************************************************/

#include "cytypes.h"
#include "cydevice_trm.h"
#include "CyLFClk.h"

#include <string.h>
#include <limits.h>
#include <ctype.h>


uint8 CyEnterCriticalSection(void) {
    asm (
    "MRS r0, PRIMASK\n"         /* Save and return interrupt state */
    "CPSID I\n"                 /* Disable interrupts */
    "BX lr\n"
    );
}

void  CyExitCriticalSection(uint8 savedIntrStatus) {
    asm (
      "MSR PRIMASK, r0\n"         /* Restore interrupt state */
      "BX lr\n"
    );
}

void  CyDelayCycles(uint32 cycles) {

}

void _init()
{

}
