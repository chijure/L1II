/*
 *  Copyright (C) 2010,Imagis Technology Co. Ltd. All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include "ist30xx.h"
#include "ist30xx_update.h"
#include "ist30xx_misc.h"
#include "ist30xx_tracking.h"


IST30XX_RING_BUF TrackBuf;
IST30XX_RING_BUF *pTrackBuf;

void ist30xx_tracking_init(void)
{
	pTrackBuf = &TrackBuf;

	pTrackBuf->RingBufCtr = 0;
	pTrackBuf->RingBufInIdx = 0;
	pTrackBuf->RingBufOutIdx = 0;
}

void ist30xx_tracking_deinit(void)
{
}

int ist30xx_put_track(u32 ms, u32 status)
{
	unsigned long flags;
	//spinlock_t mr_lock = SPIN_LOCK_UNLOCKED;
	spinlock_t mr_lock = __SPIN_LOCK_UNLOCKED();

	spin_lock_irqsave(&mr_lock, flags);

	pTrackBuf->RingBufCtr++;

	if (pTrackBuf->RingBufCtr > IST30XX_RINGBUF_SIZE) {
		pTrackBuf->RingBufCtr = IST30XX_RINGBUF_SIZE;
		pTrackBuf->RingBufOutIdx++;
	}

	if (pTrackBuf->RingBufInIdx == IST30XX_RINGBUF_SIZE)
		pTrackBuf->RingBufInIdx = 0;
	if (pTrackBuf->RingBufOutIdx == IST30XX_RINGBUF_SIZE)
		pTrackBuf->RingBufOutIdx = 0;

	pTrackBuf->TimeBuf[pTrackBuf->RingBufInIdx] = ms;
	pTrackBuf->StatusBuf[pTrackBuf->RingBufInIdx] = status;

	pTrackBuf->RingBufInIdx++;

	spin_unlock_irqrestore(&mr_lock, flags);

	return IST30XX_RINGBUF_NO_ERR;
}

int ist30xx_get_track(u32 *ms, u32 *status)
{
	unsigned long flags;
	//spinlock_t mr_lock = SPIN_LOCK_UNLOCKED;
	spinlock_t mr_lock = __SPIN_LOCK_UNLOCKED();

	if (!pTrackBuf->RingBufCtr)
		return IST30XX_RINGBUF_EMPTY;

	spin_lock_irqsave(&mr_lock, flags);

	if (pTrackBuf->RingBufOutIdx == IST30XX_RINGBUF_SIZE)
		pTrackBuf->RingBufOutIdx = 0;
	*ms = pTrackBuf->TimeBuf[pTrackBuf->RingBufOutIdx];
	*status = pTrackBuf->StatusBuf[pTrackBuf->RingBufOutIdx];

	pTrackBuf->RingBufOutIdx++;
	pTrackBuf->RingBufCtr--;

	spin_unlock_irqrestore(&mr_lock, flags);

	return IST30XX_RINGBUF_NO_ERR;
}

int ist30xx_get_track_cnt(void)
{
	return pTrackBuf->RingBufCtr;
}
