#include "m7.h"

#include <gba_interrupt.h>
#include <gba_video.h>

Mode7Context m7Context = {
	.d = 80,
	.w = ((240 - 72 + 8) >> 1)
};

// From Tonc
IWRAM_CODE static void m7() {
	int vcount = REG_VCOUNT;
	if (vcount >= 152 || vcount < 16) {
		return;
	}
	s32 lcf, lxr, lyr;

	s16 fade = m7Context.bgFade[vcount] + m7Context.fadeOffset;
	if (fade > 0xF) {
		REG_BLDY = 0xF;
	} else {
		REG_BLDY = fade;
	}

	lcf = m7Context.y * m7Context.div16[vcount] >> 12;
	
	REG_BG2PA = lcf >> 4;

	// Horizontal offset
	lxr = m7Context.w * (lcf >> 4);
	REG_BG2X = m7Context.x - lxr;

	// Vertical offset
	lyr = (m7Context.d * lcf) >> 4; 
	REG_BG2Y = m7Context.z - lyr;

	REG_IF |= IRQ_HBLANK;
}

void enableMode7(int enable) {
	if (enable) {
		irqSet(IRQ_HBLANK, m7);
		irqEnable(IRQ_HBLANK);
	} else {
		irqDisable(IRQ_HBLANK);
	}
}