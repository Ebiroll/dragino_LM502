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

/*
x/10i CyDelayCycles
   0x750 <CyDelayCycles>:       adds    r0, #2
   0x752 <CyDelayCycles+2>:     lsrs    r0, r0, #2
   0x754 <CyDelayCycles+4>:     beq.n   0x75e <CyDelayCycles_done>
   0x756 <CyDelayCycles_loop>:  adds    r0, #1
   0x758 <CyDelayCycles_loop+2>:        subs    r0, #2
   0x75a <CyDelayCycles_loop+4>:        bne.n   0x756 <CyDelayCycles_loop>
   0x75c <CyDelayCycles_loop+6>:        nop                     ; (mov r8, r8)
   0x75e <CyDelayCycles_done>:  nop                     ; (mov r8, r8)
   0x760 <CyDelayCycles_done+2>:        bx      lr

New compiler
 x/12i CyDelayCycles
   0x310 <CyDelayCycles>:	adds	r0, #2
   0x312 <CyDelayCycles+2>:	lsrs	r0, r0, #2
   0x314 <CyDelayCycles+4>:	beq.n	0x31e <CyDelayCycles+14>
   0x316 <CyDelayCycles+6>:	adds	r0, #1
   0x318 <CyDelayCycles+8>:	subs	r0, #2
   0x31a <CyDelayCycles+10>:	bne.n	0x316 <CyDelayCycles+6>
   0x31c <CyDelayCycles+12>:	nop			; (mov r8, r8)
   0x31e <CyDelayCycles+14>:	nop			; (mov r8, r8)
   0x320 <CyDelayCycles+16>:	bx	lr
   0x322 <CyDelayCycles+18>:	bx	lr
   0x324 <_init>:	bx	lr



   __attribute__((target("thumb"))) 
*/

void  CyDelayCycles(uint32 cycles) {          /* cycles bytes */
      asm ( 	
        ".align  4\n"
        ".code   16\n"
        ".syntax unified\n"        
        "ADDS r0,r0, #2\n"          /*    1    2    Round to nearest multiple of 4 */
        "LSRS r0, r0, #2\n"         /*    1    2    Divide by 4 and set flags */
        "BEQ CyDelayCycles_done\n"  /*    2    2    Skip if 0 */
        "CyDelayCycles_loop:\n"     /* For CM0+ branch instruction takes 2 CPU cycles */
        "ADDS r0,  #1\n"            /*    1    2    Increment counter */
        "SUBS r0,  #2\n"            /*    1    2    Decrement counter by 2 */
        "BNE CyDelayCycles_loop\n"  /*    2    2    2 CPU cycles (if branche is taken)*/
        "NOP\n"                     /*    1    2 */
        "CyDelayCycles_done:\n"
        "NOP\n"                     /*    1    2    Loop alignment padding */
        "BX lr\n"                   /*    3    2 */
      );
}

/* 
 New compiler
  0x324 <_init>:	bx	lr
 5.4.1
   0x83c <_init>:       push    {r3, r4, r5, r6, r7, lr}
   0x83e <_init+2>:     nop                     ; (mov r8, r8)
   0x840 <_init+4>:     pop     {r3, r4, r5, r6, r7}
   0x842 <_init+6>:     pop     {r3}
   0x844 <_init+8>:     mov     lr, r3
   0x846 <_init+10>:    bx      lr
*/
void _init()
{

}
