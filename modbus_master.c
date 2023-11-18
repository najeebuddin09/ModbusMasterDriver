/*
 * modbus_master.c
 *
 *  Created on: Dec. 17, 2021
 *      Author: Admin
 */
#include <string.h>
#include <xdc/std.h>
#include "modbus_master.h"
#include "RingBuffer.h"
#include "jh_scia.h"
#include <ti/sysbios/knl/Task.h>

Uint16 ModRTU_CRC(Uint8 * buf, int len){
  Uint16 crc = 0xFFFF;
  int pos = 0;
  int i = 8;

  for (pos = 0; pos < len; pos++) {
    crc ^= ((Uint16)buf[pos] & 0xff);          // XOR byte into least sig. byte of crc

    for (i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;
}

int modbusMakePacket(Uint8 * buf, unsigned int len, unsigned int * aPacketSize, unsigned int slaveId, unsigned int funcCode, unsigned int regNo, unsigned int noOfRegs){
  Uint16 crc = 0;
  unsigned int packetSize = 0;
  if (len <= 7) return -1;  // buffer is small
  buf[packetSize++] = slaveId;
  buf[packetSize++] = funcCode;            // function code: Read holding registers
  buf[packetSize++] = regNo>>8;
  buf[packetSize++] = regNo;
  buf[packetSize++] = noOfRegs>>8;
  buf[packetSize++] = noOfRegs;
  crc = ModRTU_CRC(buf, packetSize);
  buf[packetSize++] = crc;
  buf[packetSize++] = crc>>8;
  *aPacketSize = packetSize;
  return 1;                 // success: packet is stored in buf
}

int modbusSendPacket(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int funcCode, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    unsigned int count = 0;
    unsigned int x = 0;
    if (modbusMakePacket(buf, len, &count, slaveId, funcCode, regNo, noOfRegs) < 1) return -1;  // Error: MakePacket function failed
    sciaWrRaw((unsigned char *)buf, count);
    Task_sleep(timeout);
    if (!RingBufEmpty(&SCIA_RXBUFFER)){
        x=RingBufUsed(&SCIA_RXBUFFER);
        if (x > len) return -2;             // Error: Buffer is too small for response packet
        RingBufRead(&SCIA_RXBUFFER, (unsigned char *)buf, x);
        *respLen = x;
        return 1;       // Success: Packet read from device successfully
    } else {
        return -3;      // Error: No response
    }
}

int modbusReadCoils(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfCoils, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x01, regNo, noOfCoils, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}

int modbusReadDiscreteInputs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x02, regNo, noOfRegs, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}

int modbusReadHoldRegs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x03, regNo, noOfRegs, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}

int modbusReadInputRegs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x04, regNo, noOfRegs, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}

int modbusWriteSingleCoil(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x05, regNo, noOfRegs, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}

int modbusWriteSingleReg(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout){
    if (modbusSendPacket(buf, len, respLen, slaveId, 0x06, regNo, noOfRegs, timeout) < 1) return -1;     // Error: SendPacket function failed
    return 1;   // Success: Execution successful
}
