/**
 **********************************************************************************************************************
 * @file        debug.h
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2016-04-10
 * @brief       Debugging C header file.
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

#ifndef DEBUG_H_
#define DEBUG_H_

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
#define DEBUG_BOOT_ENABLE       1   //!< Boot debug enable - 1., disable - 0;
#define DEBUG_INIT_ENABLE       1   //!< Initialization debug enable - 1., disable - 0;
#define DEBUG_ENABLE            1   //!< Debug enable - 1., disable - 0;


#if DEBUG_BOOT_ENABLE
#define DEBUG_BOOT(F, ...)      debug_send(F "\r\n", ##__VA_ARGS__)
#else
#define DEBUG_BOOT(F, ...)      __nop()
#endif // DEBUG_BOOT_ENABLE

#if DEBUG_INIT_ENABLE
#define DEBUG_INIT(F, ...)      debug_send_os(F "\r\n", ##__VA_ARGS__)
#else
#define DEBUG_INIT(F, ...)      __nop()
#endif // DEBUG_INIT_ENABLE

#if DEBUG_ENABLE
#define DEBUG(F, ...)           debug_send_os(F "\r\n", ##__VA_ARGS__)
#else
#define DEBUG(F, ...)           __nop()
#endif // DEBUG_ENABLE


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
 * @brief   Initialize debug.
 *
 * @note    Will initialize necessary OS elements.
 *
 * @return  State of initialization.
 * @retval  0   failed.
 * @retval  1   success.
 */
bool debug_init(void);

/**
 * @brief   Send debug message using stdarg (similar to printf).
 *
 * @param   fmt     Message format.
 *
 * @note    USe only when no OS running.
 */
void debug_send(const char *fmt, ...);

/**
 * @brief   Send debug message using stdarg (similar to printf).
 *
 * @param   fmt     Message format.
 *
 * @note    Use this function when OS running. @ref debug_init should be called before.
 */
void debug_send_os(const char *fmt, ...);

/**
 * @brief   Send debug massage in blocking mode.
 *
 * @param   data    Pointer to debug message.
 * @param   size    Debug message size in bytes.
 *
 * @note    Only for faults and something like that.
 */
void debug_send_blocking(uint8_t *data, uint32_t size);

/**
 * @brief   Debug buffer in hex's
 *
 * @param   buffer  Pointer to data buffer to debug.
 * @param   size    Size of data buffer in bytes.
 */
void debug_hex_os(uint8_t *buffer, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H_ */
