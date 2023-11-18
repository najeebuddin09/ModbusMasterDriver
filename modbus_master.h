/*
 * modbus_master.h
 *
 *  Created on: Dec. 17, 2021
 *      Author: Admin
 */

#ifndef CUSTOM_LIBS_MODBUS_MODBUS_MASTER_H_
#define CUSTOM_LIBS_MODBUS_MODBUS_MASTER_H_

int modbusReadCoils(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfCoils, unsigned int timeout);
int modbusReadDiscreteInputs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout);
int modbusReadHoldRegs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout);
int modbusReadInputRegs(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout);
int modbusWriteSingleCoil(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout);
int modbusWriteSingleReg(Uint8 * buf, unsigned int len, unsigned int * respLen, unsigned int slaveId, unsigned int regNo, unsigned int noOfRegs, unsigned int timeout);

#endif /* CUSTOM_LIBS_MODBUS_MODBUS_MASTER_H_ */
