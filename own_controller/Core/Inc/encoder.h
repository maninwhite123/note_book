#ifndef __ENCODER_H
#define __ENCODER_H 

#include "rtwtypes.h"

// BiSS-C֡�ṹ (N=32, ��֡��43λ)
#define BISS_C_FRAME_BITS 43
#define BISS_C_AP_BITS    32
#define BISS_C_RX_BYTES   6  // 43λ��Ҫ6�ֽ�(48λ)������

// SPI���ջ�����
extern uint8_T bissc_rx_buf[BISS_C_RX_BYTES];

// ��������ṹ��
typedef struct {
  uint32_T position;  // ����λ��ֵ
  uint8_T ack;        // Ack��־
  uint8_T start;      // Start��־
  uint8_T cds;        // CDS��־
  uint8_T error;      // �����־
  uint8_T warn;       // �����־
  uint8_T crc;        // ���յ���CRCֵ
  uint8_T crc_ok;     // CRCУ����
  uint8_T frame_ok;   // ����֡��Ч��
} BissCFrame;


void SPI_DMA_RX_Complete_IRQHandler(void);
BissCFrame BissC_Parse(uint8_T *rx_buf);
uint8_T BissC_CalculateCRC(uint64_T data);

#endif