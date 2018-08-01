# Overview #

This project is a programming tool that will update the I2C address of a ECC608A chip.

## Hardware ##

SAMD20 XPRO Development board - https://www.arrow.com/en/reference-designs/atsamd20-xpro-sam-d20-xplained-pro-evaluation-kit-to-evaluate-the-atsamd20j18a-microcontroller/781059284999af2c89a9c74122d37f6b

CRYPTOAUTH-XPRO-B - https://www.arrow.com/en/products/atcryptoauth-xpro-b/microchip-technology

## Usage ##

* Open the CypherCryptoUpdater.atsln using Atmel Studio 7.0
* Build and program the SAMD20 XPRO board
* Attach the CRYPTAUTH-XPRO board to one of the XPRO headers
* Make sure the 608A chip is selected on the CRYPTAUTH board.
* Connect to the SAMD20 board using Putty over the EDBG serial port at 9600 baud (Default 8-N-1 settings). Here are some instructions: https://developer.arrowconnect.io/page/arrow-connect-microchip-act-2018
* Reset the board and you should see a console menu for programming.
* Use the 'P' command to change the XPRO board I2C address from 0xC0 to 0xCC.  You should see lots of text print out a it dumps the config section of the chip 
* Use the 'T' and 'V' commands to see which address the chip responds to.  It might take a few checks for the device to switch addresses
* Once the chip reports back correctly using the 'V' command it has been correctly updated to address 0xCC

 
