

#ifndef __CRC_H__
#define __CRC_H__

#include "stm32f10x.h"

#ifndef __CRC_H_INC__
	#define __CRC_H_EXT__ extern
#else
	#define __CRC_H_EXT__
#endif

// º¯Êý //
__CRC_H_EXT__ uint32_t CalculateCRC32(uint32_t addr, uint32_t len);
__CRC_H_EXT__ uint16_t CRC_16_pData(uint8_t *aData, uint16_t len);
__CRC_H_EXT__ uint16_t CRC_16_fData(uint32_t addr, uint16_t len);
#endif /* __CRC_H__ */
