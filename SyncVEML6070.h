#ifndef SyncVEML6070_H
#define	SyncVEML6070_H

//Include libraries
#include "Wire.h"
#include "Arduino.h"

/** VEML6070 Addresses
 *	Ref. Datasheet rev 1.6 (Jul 2015), page 06 Slave Address and Function Description
*/
#define VEML6070_ADDR_MSB (0x39)
#define VEML6070_ADDR_LSB (0x38)

class SyncVEML6070 {

	private:
		void waitForNext(void);
		void writeWithoutRegister(uint8_t I2CAddress, uint8_t data);
    /** Command register bits description
     *  Ref. Datasheet rev 1.6 (Jul 2015), page 06, 07 [table 1, table 2]
    */
    struct command_reg {
      /** Command register format */
      command_reg() : SD(0), RES1(1), IT(VEML6070_HALF_T){}
      unsigned int SD : 1;//< shutdown mode setting (1 bit)
      unsigned int RES1 : 1;//< Reserved (1 bit) always 1
      unsigned int IT : 2;//< Integrated time setting (2 bits)
      unsigned int ACK_THD : 1;//< Acknowledge threshold window setting (1 bit)
      unsigned int ACK : 1;//< Acknowledge activity setting (1 bit)
      //unsigned int RES2 : 2;//< Reserved (2 bits)
      /** Used to retrieve the assembled command register's byte value. */
      unsigned int get(){ return  (ACK << 5) | (ACK_THD << 4) | (IT << 2) | (RES1 << 1) | SD;}
    };

    command_reg _commandReg;
    uint8_t _veml6070I2CAddress;


	public:
		/** Refresh time determination
		 * Ref. Datasheet rev 1.6 (Jul 2015), page 08 [table 4]
    */
		enum veml6070RefreshTime {
			VEML6070_HALF_T = 0x00,//< 1/2T 62.5 ms(RSET = 300 kΩ), 125 ms(RSET = 600 kΩ)
			VEML6070_1_T = 0x01,//< 1T 125 ms(RSET = 300 kΩ), 250 ms(RSET = 600 kΩ)
			VEML6070_2_T = 0x02,//< 2T 250 ms(RSET = 300 kΩ), 500 ms(RSET = 600 kΩ)
			VEML6070_4_T = 0x03,//< 4T 500 ms(RSET = 300 kΩ), 1000 ms(RSET = 600 kΩ)
		};
		SyncVEML6070();
		void begin(veml6070RefreshTime refreshTime);
		uint16_t getUV();
		void sleep(bool state);

};

#endif