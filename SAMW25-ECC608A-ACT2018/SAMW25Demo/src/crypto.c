/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"
#include <stdlib.h>

#include "atmel_start_pins.h"
#include "driver_init.h"
#include "system.h"

#include "crypto.h"
#include "atca_host.h"

#define CRYPTO_SERIAL_NUM_SIZE			ATCA_SERIAL_NUM_SIZE
#define CRYPTO_REVISION_SIZE			4
#define CRYPTO_ECC_CONFIG_SIZE			ATCA_ECC_CONFIG_SIZE
#define DISCOVER_DEVICE_COUNT_MAX       (4)

//! enumeration for device types
typedef enum {
	DEVICE_TYPE_UNKNOWN,   //!< unknown device
	DEVICE_TYPE_CM,        //!< CM, currently not supported
	DEVICE_TYPE_CRF,       //!< CRF, currently not supported
	DEVICE_TYPE_CMC,       //!< CMC, currently not supported
	DEVICE_TYPE_SA100S,    //!< SA100S, can be discovered, but is currently not supported
	DEVICE_TYPE_SA102S,    //!< SA102S, can be discovered, but is currently not supported
	DEVICE_TYPE_SA10HS,    //!< SA10HS, can be discovered, but is currently not supported
	DEVICE_TYPE_SHA204,    //!< SHA204 device
	DEVICE_TYPE_AES132,    //!< AES132 device
	DEVICE_TYPE_ECC108,    //!< ECC108 device
	DEVICE_TYPE_ECC108A,   //!< ECC108A device
	DEVICE_TYPE_ECC508A,   //!< ECC508A device
	DEVICE_TYPE_ECC608A    //!< ECC608A device
} device_type_t;

//! enumeration for interface types
typedef enum {
	DEVKIT_IF_UNKNOWN,
	DEVKIT_IF_SPI,
	DEVKIT_IF_I2C,
	DEVKIT_IF_SWI,
	DEVKIT_IF_UART
} interface_id_t;

//! information about a discovered device
typedef struct {
	//! interface type (SWI, I2C, SPI)
	interface_id_t bus_type;
	//! device type
	device_type_t device_type;
	//! I2C address or selector byte
	uint8_t address;
	//! array index into GPIO structure for SWI and SPI (Microbase does not support this for SPI.)
	uint8_t device_index;
	//! revision bytes (four bytes for SHA204 and ECC108, first two bytes for AES132)
	uint8_t dev_rev[4];
} device_info_t;


#define ATECC_I2C_ADDR_INDEX	16
#define ECC_SLOT_LENGTH			32
//#define AC_SLOT_LENGTH			64



// Types and definitions
// ---------------------------------------------------------------------------

// Variables
// ---------------------------------------------------------------------------

ATCAIfaceCfg ATECC508desc = {
    .iface_type        = ATCA_I2C_IFACE,
    .devtype           = ATECC508A,
    .atcai2c           = {
        .slave_address = 0xC0,
        .bus           = 2,
        .baud          = 400000,
    },
    .wake_delay        = 800,
    .rx_retries        = 20,
	.cfg_data          = &I2C_0
};

ATCAIfaceCfg ATECC608desc = {
	.iface_type        = ATCA_I2C_IFACE,
	.devtype           = ATECC608A,
	.atcai2c           = {
		.slave_address = 0xCC,
		.bus           = 2,
		.baud          = 400000,
	},
	.wake_delay        = 800,
	.rx_retries        = 20,
	.cfg_data          = &I2C_0
};

const uint8_t g_slot4_key[] = {
	0x37, 0x80, 0xe6, 0x3d, 0x49, 0x68, 0xad, 0xe5,
	0xd8, 0x22, 0xc0, 0x13, 0xfc, 0xc3, 0x23, 0x84,
	0x5d, 0x1b, 0x56, 0x9f, 0xe7, 0x05, 0xb6, 0x00,
	0x06, 0xfe, 0xec, 0x14, 0x5a, 0x0d, 0xb1, 0xe3
};

uint8_t test_ecc608_configdata[ATCA_ECC_CONFIG_SIZE] = {
	0x01, 0x23, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x04, 0x05, 0x06, 0x07, 0xEE, 0x01, 0x01, 0x00,  //15
	0xCC, 0x00, 0xA1, 0x00, 0xAF, 0x2F, 0xC4, 0x44, 0x87, 0x20, 0xC4, 0xF4, 0x8F, 0x0F, 0x0F, 0x0F,  //31, 5
	0x9F, 0x8F, 0x83, 0x64, 0xC4, 0x44, 0xC4, 0x64, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,  //47
	0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,  //63
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x84, 0x03, 0xBC, 0x09, 0x69, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00,  //79
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0E, 0x40, 0x00, 0x00, 0x00, 0x00,  //95
	0x33, 0x00, 0x1C, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0x3E, 0x00, 0x1C, 0x00, 0x33, 0x00,  //111
	0x1C, 0x00, 0x1C, 0x00, 0x38, 0x10, 0x30, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x32, 0x00, 0x30, 0x00   //127
};

const uint8_t test_ecc_configdata[ATCA_ECC_CONFIG_SIZE] = {
	0x01, 0x23, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x04, 0x05, 0x06, 0x07, 0xEE, 0x00, 0x01, 0x00, //15
	0xC0, 0x00, 0x55, 0x00, 0x8F, 0x2F, 0xC4, 0x44, 0x87, 0x20, 0xC4, 0xF4, 0x8F, 0x0F, 0x8F, 0x8F, //31, 5
	0x9F, 0x8F, 0x83, 0x64, 0xC4, 0x44, 0xC4, 0x64, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, //47
	0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, //63
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //79
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //95
	0x33, 0x00, 0x1C, 0x00, 0x13, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x33, 0x00, //111
	0x1C, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0x30, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x32, 0x00, 0x30, 0x00  //127
};

const uint8_t sha204_default_config[ATCA_SHA_CONFIG_SIZE] = {
	// block 0
	// Not Written: First 16 bytes are not written
	0x01, 0x23, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xEE, 0x55, 0x00, 0x00,
	// I2C, CheckMacConfig, OtpMode, SelectorMode
	0xC8, 0x00, 0x55, 0x00,
	// SlotConfig
	0x8F, 0x80, 0x80, 0xA1,
	0x82, 0xE0, 0xC4, 0xF4,
	0x84, 0x00, 0xA0, 0x85,
	// block 1
	0x86, 0x40, 0x87, 0x07,
	0x0F, 0x00, 0xC4, 0x64,
	0x8A, 0x7A, 0x0B, 0x8B,
	0x0C, 0x4C, 0xDD, 0x4D,
	0xC2, 0x42, 0xAF, 0x8F,
	// Use Flags
	0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0x00, 0xFF, 0x00,
	// block 2
	0xFF, 0x00, 0xFF, 0x00,
	// Last Key Use
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF,
	// Not Written: UserExtra, Selector, LockData, LockConfig (word offset = 5)
	0x00, 0x00, 0x55, 0x55,
};

static uint8_t serial[9];

static ATCADeviceType m_my_device;

static bool m_init_complete = false;

static uint8_t slot_buffer[ECC_SLOT_LENGTH];


// Local prototypes
// ---------------------------------------------------------------------------
static bool _crypto_test_hmac(ecc_data_slots_t slot, uint8_t* digest, uint8_t * buf, int buf_len, uint8_t * key, int key_len);
static void _crypto_data_slot_dump(ecc_data_slots_t slot);
static bool _crypto_slot_write(ecc_data_slots_t slot, uint8_t * pub_key, int len);
static ATCA_STATUS _crypto_revision_get(uint8_t * revision);
static ATCA_STATUS _crypto_serial_get(uint8_t * serial );

// Public functions
// ---------------------------------------------------------------------------
void crypto_init(ATCADeviceType device_type)
{
	ATCA_STATUS status;
	ATCAIfaceCfg * cfg_ptr;
	
	m_my_device = device_type;
	
	// Load the device specific data
	switch(m_my_device) {
		default:
		case ATECC608A:
		cfg_ptr = &ATECC608desc;
		break;
		case ATECC508A:
		cfg_ptr = &ATECC508desc;
		break;
	}
	
	// Initialize the driver
	status = atcab_init(cfg_ptr);
	if (status != ATCA_SUCCESS)
	{
		printf("atcab_init() failed: %02x\r\n", status);
		return;
	}
	m_init_complete = true;
		
	// Make sure the locks are correct
	crypto_configure_locks();
	
	
	return;
}

bool crypto_is_attached()
{
	return (m_init_complete==true);
	//return (atcab_wakeup()==ATCA_SUCCESS);
}

bool crypto_configure_locks(void)
{
	ATCA_STATUS status;
	uint8_t * config_data_ptr = NULL;
	bool is_locked = false;
	
	
	// Load the device specific data
	switch(m_my_device) {
		default:
		case ATECC608A:
		config_data_ptr = (uint8_t*)test_ecc608_configdata;
		break;
		case ATECC508A:
		config_data_ptr = (uint8_t*)test_ecc_configdata;
		break;
	}
	if(!config_data_ptr){
		printf("Can't configure the device\n");
		return false;
	}

	// If the device config zone is not locked, then lock it
	status = atcab_is_locked(LOCK_ZONE_CONFIG, &is_locked);
	if(status != ATCA_SUCCESS)
	{
		printf("Error Reading Config Zone Lock State with Code: 0x%X", status);
		return false;
	}
	if(!is_locked)
	{
		printf("Config zone is NOT locked, attempting to lock...\n");
		status = atcab_write_config_zone(config_data_ptr);
		if(status == ATCA_SUCCESS)
		{
			status = atcab_lock_config_zone();
			if(status != ATCA_SUCCESS)
			{
				printf("Error locking config zone with code: 0x%X", status);
				return false;
			}
			
		}
	}
	printf("Config zone is locked.\n");
			
	// If the device data zone is not locked, then lock it
	status = atcab_is_locked(LOCK_ZONE_DATA, &is_locked);
	if(status != ATCA_SUCCESS)
	{
		printf("Error reading data zone lock state\n");
		return false;
	}
	if(!is_locked)
	{
		printf("Data zone is not locked, attempting to lock...\n");
		status = atcab_lock_data_zone();
		if(status != ATCA_SUCCESS)
		{
			printf("Error locking data zone\n");
			return false;
		}
	}
	printf("Data zone is locked\n");
	
	// At this point both the config zone and the data zone
	// are locked.
	
	return true;
}

void crypto_info(void)
{
	ATCA_STATUS status;
	uint8_t revision[CRYPTO_REVISION_SIZE];
	uint8_t serial[CRYPTO_SERIAL_NUM_SIZE];

	printf("Crypto chip info:\n");

	// Print chip info	
	status = _crypto_revision_get(revision);
	if(status!=ATCA_SUCCESS)
	{
		printf(" Error with communication L%d, status=%d\n",__LINE__,status);
		return;
	}
	printf(" Device Revision: 0x");
	for(int i = 0; i < CRYPTO_REVISION_SIZE; i++)
	{
		printf("%X", revision[i]);
	}
	printf("\n");

	// Device type	
	printf(" Device Type: ");
	switch(revision[2]){
	case 0x50: printf("ECC508A\n"); break;
	case 0x60: printf("ECC608A\n"); break;
	}

	// Serial number	
	printf(" Serial Number: 0x");
	status = _crypto_serial_get(serial);
	if(status!=ATCA_SUCCESS)
	{
		printf(" Error with communication L%d, status=%d\n",__LINE__,status);
		return;
	}
	for(int i = 0; i < CRYPTO_SERIAL_NUM_SIZE; i++)
	{
		printf("%X", serial[i]);
	}
	printf("\n");
	
	return;
}

void crypto_config_dump(void)
{
	ATCA_STATUS status;
	uint8_t config_buffer[ATCA_ECC_CONFIG_SIZE];
	
	memset(config_buffer,0,sizeof(config_buffer));
	status = atcab_read_config_zone(config_buffer);
	if(status!=ATCA_SUCCESS){
		printf("Error with communication L%d, status=%d\n",__LINE__,status);
		return;
	}
	
	// Dump memory
	system_printmem(config_buffer,sizeof(config_buffer));
	
	return;
}

void crypto_slots_dump(void)
{
	int x;
	for(x=0;x<DATA_SLOT_MAX;x++)
	{
		_crypto_data_slot_dump(x);
	}
	return;
}

bool crypto_write_slot(ecc_data_slots_t slot, uint8_t * buf, int len)
{
	ATCA_STATUS status;
	bool is_locked = false;

	memset(slot_buffer,0,sizeof(slot_buffer));

	// Slot sanity check
	if(slot<0 || slot > DATA_SLOT_MAX)
	{
		printf("Invalid slot %d. Acceptable values are 0 - 15.\n",slot);
		return false;
	}

	// Check length
	if(len>ECC_SLOT_LENGTH)
	{
		printf("Data too long %d>%d, generating SHA256 hash\n",len,ECC_SLOT_LENGTH);
		
		// Data longer than the slot needs to be sha256 encoded first
		memset(slot_buffer,0,sizeof(slot_buffer));
		crypto_sha256(slot_buffer,buf,len);
		
		printf("Hash: ");
		int x;
		for(x=0;x<ECC_SLOT_LENGTH;x++) printf("%02x",slot_buffer[x]);
		printf("\n");
		
	}else{
		// Copy the buffer over to our slot buffer (which is 0 padded)
		memcpy(slot_buffer,buf,len);	
	}
		
	// Check config zone == locked
	status = atcab_is_locked(LOCK_ZONE_CONFIG, &is_locked);
	if(status != ATCA_SUCCESS)
	{
		printf("Error reading config zone lock state. Error Code: %X\n", status);
		return false;
	}
	if(!is_locked)
	{
		printf("Config zone is not locked! Lock the config zone before attempting to write to the data zone\n");
		return false;
	}
	//printf("Config zone is locked\n");
	
	// Check slot locked == false
	status = atcab_is_slot_locked(slot, &is_locked);
	if(status != ATCA_SUCCESS)
	{
		printf("Error getting slot lock status with code: 0x%X\n", status);
		return false;
	}
	if(is_locked)
	{
		printf("Data slot is locked. No new data can be written to this slot.\nTry writing the data to a different slot.\n");
		return false;
	}
	//printf("Slot is unlocked\n");
	
	// Write the data, block 0
	status = atcab_write_zone(ATCA_ZONE_DATA, slot, 0, 0, slot_buffer, 32);
	if(status!=ATCA_SUCCESS){
		printf("Write error %X\n",status);
		return false;
	}
	#if 0
	// Write the data, block 1
	status = atcab_write_zone(ATCA_ZONE_DATA, slot, 1, 0, &(slot_buffer[32]), 32);
	if(status!=ATCA_SUCCESS){
		printf("Write error %X\n",status);
		return false;
	}
	#endif

	
	return true;
}

bool crypto_sha256(uint8_t *shasum, uint8_t *buf, uint16_t size)
{
	ATCA_STATUS status;
	status = atcab_sha(size, buf, shasum);
	return (status==ATCA_SUCCESS);
}

bool crypto_hmac256(uint8_t * hmacdig, ecc_data_slots_t slot, uint8_t * buf, int buf_size)
{
	ATCA_STATUS status;

	// NOTE: Digest must be 32 bytes

	if(slot<0 || slot > DATA_SLOT_MAX)
	{
		printf("Invalid slot %d. Acceptable values are 0 - 15.\n",slot);
		return false;
	}
	status = atcab_sha_hmac(buf, buf_size, slot, hmacdig, SHA_MODE_TARGET_TEMPKEY);
	if(status != ATCA_SUCCESS)
	{
		printf("Error Creating HMAC/SHA-256 with code: 0x%x\n", status);
		return false;
	}

	return true;
}

char test_GBA[] = "Four score and seven years ago our fathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal.";
char test_key[] = "c445596cfd3fd6bc40b4e78691674361ab2450c9a03c55603e32dbdb334d8480";
char test_key2[] = "c445596cfd3fd6bc40b4e78691674361ab2450c9a03c55603e32dbdb334d84800";
void crypto_test()
{
	ecc_data_slots_t slot = SECRET_KEY_DATA_SLOT;
	uint8_t digest[32];
	int x;
	char key[] = "SomeKey";
	char message[] = "Testing";
	
	printf("-- Crypto chip test\n");
	
	_crypto_test_hmac(slot,digest,(uint8_t*)message,strlen(message),(uint8_t*)key,strlen(key));

	_crypto_test_hmac(slot,digest,(uint8_t*)message,strlen(message),(uint8_t*)test_key,strlen(test_key));

	_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)key,strlen(key));

	_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)test_key,strlen(test_key));
	
	_crypto_test_hmac(slot,digest,(uint8_t*)message,strlen(message),(uint8_t*)test_key2,strlen(test_key2));

	_crypto_test_hmac(slot,digest,(uint8_t*)test_GBA,strlen(test_GBA),(uint8_t*)test_key2,strlen(test_key2));

	return;
}

// Private functions
// ---------------------------------------------------------------------------
static bool _crypto_test_hmac(ecc_data_slots_t slot, uint8_t* digest, uint8_t * buf, int buf_len, uint8_t * key, int key_len)
{
	int x;
	memset(digest,0,32);
	crypto_write_slot(slot,key,key_len);
	_crypto_data_slot_dump(slot);
	crypto_hmac256(digest,slot, buf, buf_len);
	printf("Key  : '%s'\n",(char*)key);
	printf("Input: '%s'\n",(char*)buf);
	printf("HMAC : ");
	for(x=0;x<32;x++) printf("%02X",digest[x]);
	printf("\n");
	return true;
}
static void _crypto_data_slot_dump(ecc_data_slots_t slot)
{
	ATCA_STATUS status;
	uint8_t slot_data_block0[32];
	uint8_t slot_data_block1[32];
	
	if(slot<0 || slot > DATA_SLOT_MAX)
	{
		printf("Invalid data slot. Acceptable values are 0 - 15.\n");
		return;
	}
	
	status = atcab_read_zone(ATCA_ZONE_DATA, slot, 0, 0, slot_data_block0, sizeof(slot_data_block0));
	if(status!=ATCA_SUCCESS){
		printf("Data slot-block0 read error %X\n",status);
		return;
	}
	
	status = atcab_read_zone(ATCA_ZONE_DATA, slot, 1, 0, slot_data_block1, sizeof(slot_data_block1));
	if(status!=ATCA_SUCCESS){
		printf("Data slot-block1 read error %X\n",status);
		return;
	}
	
	printf("Slot %2d, Block 0 Data: 0x", slot);
	for(int j = 0; j < sizeof(slot_data_block0); j ++)
	{
		printf("%02X", slot_data_block0[j]);
	}
	printf("\n");
	
	printf("Slot %2d, Block 1 Data: 0x", slot);
	for(int j = 0; j < sizeof(slot_data_block1); j ++)
	{
		printf("%02X", slot_data_block1[j]);
	}
	printf("\n");
	
	return;
}

static ATCA_STATUS _crypto_revision_get(uint8_t * revision)
{
	ATCA_STATUS status;
	
	status = atcab_info(revision);
	if(status!=ATCA_SUCCESS){
		printf("Read error on config zone %X\n",status);
		return status;
	}
	
	return ATCA_SUCCESS;
}

static ATCA_STATUS _crypto_serial_get(uint8_t * serial)
{
	ATCA_STATUS status;
	
	status = atcab_read_serial_number(serial);
	if(status!=ATCA_SUCCESS){
		printf("Read error on config zone %X\n",status);
		return status;
	}
	
	return ATCA_SUCCESS;
}

// EOF
