/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name        : version.h
 * Description      : Project version information.
 *********************************************************************************************************************/

#ifndef VERSION_H
#define VERSION_H

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define xstr(s) str(s)
#define str(s) #s

#define BOARD_NAME                  "ARIS"
#define PJT_NAME                    "WIFI_AP"

#define FIRMWARE_VERSION            1
#define FIRMWARE_REVISION           1
#define FIRMWARE_PATCH              0

#define FIRMWARE_STRING             xstr(FIRMWARE_VERSION) "." xstr(FIRMWARE_REVISION) "." xstr(FIRMWARE_PATCH)
#define FIRMWARE_NUMBER             ( (FIRMWARE_VERSION << 16) + (FIRMWARE_REVISION << 8) + (FIRMWARE_PATCH << 0) )

#define BOOT_HEADER_STRING          "     =======================================\r\n" \
                                    "\t\t[ " BOARD_NAME " - " PJT_NAME " ]\r\n" \
                                    "\tFirmware Version : " FIRMWARE_STRING "\r\n" \
                                    "\tBuild time : " __DATE__ " "__TIME__ "\r\n" \
                                    "     =======================================\r\n"

#endif  /* VERSION_H */
