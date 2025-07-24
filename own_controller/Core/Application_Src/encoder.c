#include "encoder.h"
#include "main.h"

extern 	uint32_T encoder_position;

uint8_T bissc_rx_buf[BISS_C_RX_BYTES];


// DMA接收完成中断处理
/**
  * @brief  SPI DMA接收完成中断处理函数
  * @note   在DMA传输完成中断中调用
  *         检查DMA标志位并解析接收到的BiSS-C数据
  * @param  None
  * @return None
  */
void SPI_DMA_RX_Complete_IRQHandler(void)
{
  // 检查DMA传输完成标志
//  if (__HAL_DMA_GET_FLAG(hdma_spi_rx, DMA_FLAG_TC)) {
//    __HAL_DMA_CLEAR_FLAG(hdma_spi_rx, DMA_FLAG_TC);
    
    // 解析接收到的数据
    BissCFrame frame = BissC_Parse(bissc_rx_buf);
    
    // 如果数据有效，处理位置值
    if (frame.frame_ok) {
      // 使用绝对位置值 (示例: 更新编码器计数器)
      encoder_position = frame.position;
    }
//  }
}

// BiSS-C数据解析函数
/**
  * @brief  BiSS-C数据帧解析函数
  * @param  rx_buf: 指向SPI接收缓冲区的指针(6字节)
  * @return BissCFrame结构体，包含解析结果和校验状态
  * @note   解析顺序遵循BiSS-C协议规范(MSB first):
  *         [Ack][Start][CDS][AP(32位)][Error][Warn][CRC6]
  */
BissCFrame BissC_Parse(uint8_T *rx_buf)
{
  BissCFrame result = {0};
  uint64_T raw_data = 0;

  result.frame_ok = 0;  // 初始化帧有效性为无效
  
  // 组合6字节数据到64位变量 (MSB first)
  for (int i = 0; i < BISS_C_RX_BYTES; i++) {
    raw_data = (raw_data << 8) | rx_buf[i];
  }
  
  // 丢弃多余的5位 (43位有效数据)
  raw_data >>= (64 - BISS_C_FRAME_BITS);  // 右移21位使有效位对齐
  
  // 解析各字段 (参考BiSS-C帧结构)
  result.ack    = (raw_data >> (BISS_C_FRAME_BITS - 1)) & 0x01;  // 位42
  result.start  = (raw_data >> (BISS_C_FRAME_BITS - 2)) & 0x01;  // 位41
  result.cds    = (raw_data >> (BISS_C_FRAME_BITS - 3)) & 0x01;  // 位40
  result.error  = (raw_data >> 7) & 0x01;  // 位7
  result.warn   = (raw_data >> 6) & 0x01;  // 位6
  result.crc    = raw_data & 0x3F;         // 位0-5
  
  // 提取绝对位置 (位8-39)
  result.position = (raw_data >> 8) & 0xFFFFFFFF;
  
  // CRC校验 (多项式 x⁶ + x¹ + x⁰ = 0x43)
  result.crc_ok = (BissC_CalculateCRC(raw_data >> 6) == result.crc);
  
  // 整体帧有效性检查 (CRC + 协议标志)
  // result.frame_ok = result.crc_ok && result.ack && result.start && (result.cds == 0);
  //为了方便直接验证CRC校验是否正确
  result.frame_ok = result.crc_ok;
  
  return result;
}

// BiSS-C CRC6计算函数 (多项式 x⁶ + x¹ + x⁰ = 0x43)
/**
  * @brief  BiSS-C CRC6计算函数
  * @param  data: 包含37位输入数据(Ack到Warn)的64位变量
  * @return 计算得到的6位CRC值(取反后)
  * @note   - 多项式: x⁶ + x¹ + x⁰ (0x43)
  *         - 初始值: 0x3F(全1)
  *         - 最终结果取反
  *         - 计算过程保留6位有效数据
  */
uint8_T BissC_CalculateCRC(uint64_T data)
{
  // CRC多项式: x⁶ + x¹ + x⁰ (0b1000011 = 0x43)
  const uint8_T poly = 0x43;  // 7位多项式
  uint8_T crc = 0x3F;         // 初始值全1 (6位)
  
  // 处理37位数据 (Ack + Start + CDS + AP + Error + Warn)
  for (int i = 36; i >= 0; i--) {
    uint8_T bit = (data >> i) & 0x01;
    uint8_T msb = (crc >> 5) & 0x01;
    
    crc <<= 1;  // 左移1位
    
    if (bit ^ msb) {
      crc ^= poly;
    }
    
    // 保持6位长度 (最高位在计算中被丢弃)
    crc &= 0x3F;
  }
  
  return (~crc) & 0x3F;  // 取反并返回6位结果
}