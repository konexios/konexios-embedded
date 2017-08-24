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

#ifndef SF_ADXL362_PRIVATE_API_H
#define SF_ADXL362_PRIVATE_API_H

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_ADXL362_Open(sf_adxl362_ctrl_t * const p_ctrl, sf_adxl362_cfg_t const * const p_cfg);
ssp_err_t SF_ADXL362_ReadAccelerations (sf_adxl362_ctrl_t * const p_ctrl,
                                        float * x_value,
                                        float * y_value,
                                        float * z_value);
ssp_err_t SF_ADXL362_Reset(sf_adxl362_ctrl_t * const p_ctrl);
ssp_err_t SF_ADXL362_Close(sf_adxl362_ctrl_t * const p_ctrl);
ssp_err_t SF_ADXL362_VersionGet(ssp_version_t * const p_version);

#endif /* SF_ADXL362_PRIVATE_API_H */
