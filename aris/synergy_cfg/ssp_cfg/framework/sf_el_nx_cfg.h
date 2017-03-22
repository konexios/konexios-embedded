/* generated configuration header file - do not edit */
#ifndef SF_EL_NX_CFG_H_
#define SF_EL_NX_CFG_H_
#define SF_EL_NX_COMMS_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
/** Specify MAC address. */
#define SF_EL_NX_CFG_ENET0_MAC_H 0x00002E09
#define SF_EL_NX_CFG_ENET0_MAC_L 0x0A0076C7
#define SF_EL_NX_CFG_ENET1_MAC_H 0x00002E09
#define SF_EL_NX_CFG_ENET1_MAC_L 0x0A0076C8
/** Specify reset pin. */
#define SF_EL_NX_CFG_ENET0_RESET_PIN IOPORT_PORT_09_PIN_03
#define SF_EL_NX_CFG_ENET1_RESET_PIN IOPORT_PORT_07_PIN_06
/** Specify number of descriptors. */
#define NUM_RX_DESC               (8)
#define NUM_TX_DESC               (32)
#endif /* SF_EL_NX_CFG_H_ */
