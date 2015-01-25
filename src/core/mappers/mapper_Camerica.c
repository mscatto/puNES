/*
 * mapper_Camerica.c
 *
 *  Created on: 12/lug/2011
 *      Author: fhorse
 */

#include "mappers.h"
#include "info.h"
#include "mem_map.h"

void map_init_Camerica(void) {
	switch (info.mapper.submapper) {
		case BF9096:
			EXTCL_CPU_WR_MEM(Camerica_BF9096);
			break;
		case BF9097:
			EXTCL_CPU_WR_MEM(Camerica_BF9097);
			break;
		case GOLDENFIVE:
			EXTCL_CPU_WR_MEM(Camerica_GoldenFive);
			if (info.reset >= HARD) {
				map_prg_rom_8k(2, 2, 0x0F);
			}
			break;
		default:
			EXTCL_CPU_WR_MEM(Camerica_BF9093);
			break;
	}
}
void extcl_cpu_wr_mem_Camerica_BF9093(WORD address, BYTE value) {
	control_bank_with_AND(0x0F, info.prg.rom.max.banks_16k)
	map_prg_rom_8k(2, 0, value);
	map_prg_rom_8k_update();
}
void extcl_cpu_wr_mem_Camerica_BF9096(WORD address, BYTE value) {
	BYTE base;

	switch ((address >> 12) & 0x0C) {
		case 0x08: {
			BYTE low = (mapper.rom_map_to[0] >> 1);

			if (info.id == PEGASUS4IN1) {
				base = ((value & 0x10) >> 2) | (value & 0x08);
				map_prg_rom_8k(2, 0, base | ((low & 0x07) >> 1));
			} else {
				base = (value & 0x18) >> 1;
				map_prg_rom_8k(2, 0, base | ((low & 0x07) >> 1));
			}
			map_prg_rom_8k(2, 2, base | 0x03);
			break;
		}
		default:
			map_prg_rom_8k(2, 0, ((mapper.rom_map_to[0] & 0x18) >> 1) | (value & 0x03));
			break;
	}
	map_prg_rom_8k_update();
}
void extcl_cpu_wr_mem_Camerica_BF9097(WORD address, BYTE value) {
	switch ((address >> 12) & 0x0C) {
		case 0x08:
			if (value & 0x10) {
				mirroring_SCR0();
			} else {
				mirroring_SCR1();
			}
			return;
		default:
			control_bank_with_AND(0x07, info.prg.rom.max.banks_16k)
			map_prg_rom_8k(2, 0, value);
			map_prg_rom_8k_update();
			return;
	}
}
void extcl_cpu_wr_mem_Camerica_GoldenFive(WORD address, BYTE value) {
	BYTE base;

	switch ((address >> 12) & 0x0C) {
		case 0x08:
			if (value & 0x08) {
				base = (value << 4) & 0x70;
				map_prg_rom_8k(2, 0, base | (mapper.rom_map_to[0] & 0x1E) >> 1);
				map_prg_rom_8k(2, 2, base | 0x0F);
			}
			break;
		default:
			map_prg_rom_8k(2, 0, ((mapper.rom_map_to[0] & 0xE0) >> 1) | (value & 0x0F));
			break;
	}
	map_prg_rom_8k_update();
}