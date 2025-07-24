#ifndef __ENCODER_H
#define __ENCODER_H 

#include "rtwtypes.h"

// BiSS-C帧结构 (N=32, 总帧长43位)
#define BISS_C_FRAME_BITS 43
#define BISS_C_AP_BITS    32
#define BISS_C_RX_BYTES   6  // 43位需要6字节(48位)缓冲区

// SPI接收缓冲区
extern uint8_T bissc_rx_buf[BISS_C_RX_BYTES];

// 解析结果结构体
typedef struct {
  uint32_T position;  // 绝对位置值
  uint8_T ack;        // Ack标志
  uint8_T start;      // Start标志
  uint8_T cds;        // CDS标志
  uint8_T error;      // 错误标志
  uint8_T warn;       // 警告标志
  uint8_T crc;        // 接收到的CRC值
  uint8_T crc_ok;     // CRC校验结果
  uint8_T frame_ok;   // 整体帧有效性
} BissCFrame;


void SPI_DMA_RX_Complete_IRQHandler(void);
BissCFrame BissC_Parse(uint8_T *rx_buf);
uint8_T BissC_CalculateCRC(uint64_T data);

#endif