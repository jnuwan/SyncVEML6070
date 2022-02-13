#include "SyncVEML6070.h"

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  VEML6070 RELATED FUNCTIONS
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
SyncVEML6070::SyncVEML6070(){
	
}

/*!
 *	@brief Initialize communication with the VEML607
 *	@return void
 */
void SyncVEML6070::begin(veml6070RefreshTime refreshTime){
	Wire.begin();
  Wire.beginTransmission(VEML6070_ADDR_LSB);
  uint8_t error = Wire.endTransmission();

	if(error == 0){
    _veml6070I2CAddress = VEML6070_ADDR_LSB;
  }
  else{
    Wire.beginTransmission(VEML6070_ADDR_MSB);
    error = Wire.endTransmission();
    if(error == 0){
      _veml6070I2CAddress = VEML6070_ADDR_MSB;
    }
  }
	_commandReg.IT = refreshTime;
	writeWithoutRegister(_veml6070I2CAddress, _commandReg.get());
}

/*!
 *	@brief Read UV sensor value
 *	@return UV index
 */
uint16_t SyncVEML6070::getUV(){
	sleep(false);
	waitForNext();
	if (Wire.requestFrom(VEML6070_ADDR_MSB, 1) != 1)
    return -1;
  uint16_t uvi = Wire.read();
  uvi <<= 8;
  if (Wire.requestFrom(VEML6070_ADDR_LSB, 1) != 1)
    return -1;
  uvi |= Wire.read();
	sleep(true);

  return uvi;
}

/*!
 *  @brief Map the integration time code to the correct multiple. Wait for next reading
 *		Ref. Datasheet rev 1.6 (Jul 2015), page 08 [table 4]
 *		Depends on RSET value. 62.5ms for RSET = 300K in datasheet table
 *  @return void
 */
void SyncVEML6070::waitForNext(){
  uint8_t itCount = 1;
  for (uint8_t i = _commandReg.IT; i > 0; i--) {
    itCount *= 2;
  }
	delay(62.5 * itCount);
}

/*!
 *  @brief Enter or exit sleep mode. While in sleep mode the chip draws ~1uA
 *  @param mode -true: sleep mode, false: wake mode
 *  @return void
 */
void SyncVEML6070::sleep(bool state){
	_commandReg.SD = state;
	writeWithoutRegister(_veml6070I2CAddress, _commandReg.get());
}

/** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  BASIC FUNCTIONS
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
/*!
 *	@brief Writes an 8 bit value over I2C
 *	@param I2CAddress -Slave device address
 *		data -Data
 *	@return value from selected register (one byte)
 */
 void SyncVEML6070::writeWithoutRegister(uint8_t I2CAddress, uint8_t data){
	Wire.beginTransmission(I2CAddress);
	Wire.write(data);
	Wire.endTransmission();
}
