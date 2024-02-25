#ifndef __DRIVER_W25Q_H
#define __DRIVER_W25Q_H

void W25QInit(void);
int W25QPageWrite(uint32_t addr,unsigned char *buf,unsigned int length);
void W25QEraseSector(uint32_t addr);
int W25QRead(uint32_t addr,unsigned char *buf,unsigned int length);
#endif //__DRIVER_W25Q_H
