/**
 **********************************************************************************************************************
 * @file        sin_detect.h
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-06-13
 * @brief       Sin signal detection C header file.
 **********************************************************************************************************************
 * @warning     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR \n
 *              IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND\n
 *              FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR\n
 *              CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n
 *              DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n
 *              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN\n
 *              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF\n
 *              THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************
 */

#ifndef SIN_DETECT_H_
#define SIN_DETECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define SIN_DETECT_RATE         5000.0F                 //!< Sin detection rate in Hz.

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of exported functions
 *********************************************************************************************************************/
/**
 * @brief   Initialize sinusoidal signal frequency detection.
 *
 * @return  State of initialization
 * @retval  0   failed
 * @retval  1   success.
 */
bool sin_detect_init(void);

/**
 * @brief   Process sinusoidal signal frequency detection.
 *
 * @note    This function must be called frequently. Frequency is defined by @ref SIN_DETECT_RATE.
 *
 * @param   signal  Signal to process.
 */
void sin_detect_process(uint32_t signal);

/**
 * @brief   Debug sinusoidal signal frequency.
 *
 * @note Call it from the thread.
 */
void sin_detect_debug(void);

#ifdef __cplusplus
}
#endif

#endif /* SIN_DETECT_H_ */
