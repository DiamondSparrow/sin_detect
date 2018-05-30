/**
 **********************************************************************************************************************
 * @file        uart.h
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       UART C header file.
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

#ifndef UART_H_
#define UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/
#define UART_0_BAUDRATE             115200  //!< UART 0 baudrate in bps.
#define UART_0_TX_DATA_SIZE         512     //!< UART 0 transmit data buffer size in bytes.
#define UART_0_RX_DATA_SIZE         512     //!< UART 0 receive data buffer size in bytes.

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
 * @brief Initialize UART 0.
 */
void uart_0_init(void);

/**
 * @brief   Send data to UART 0 using blocking mode.
 *
 * @param   data    Data to send.
 * @param   size    Size of data to send.
 */
void uart_0_send_blocking(const uint8_t data[], uint32_t size);

/**
 * @brief   Is send ring buffers of UART0 empty?
 *
 * @return  State of ring buffers.
 * @retval  0   Send ring buffer is empty.
 * @retval  1   Send ring buffer is not empty.
 */
uint32_t uart_0_is_send_rb_empty(void);

/**
 * @brief   Send data to UART 0 using ring buffer via interrupt.
 *
 * @param   data    Pointer to data that will be sent.
 * @param   size    Size of data to send in bytes.
 *
 * @return  Size of data in bytes that will by sent.
 */
uint32_t uart_0_send_rb_irq(uint8_t *data, uint32_t size);

/**
 * @brief   Read data from UART 0 using ting buffer via interrupt.
 *
 * @param   data    Pointer to data buffer where received data will be stored.
 * @param   size    Size of data in bytes.
 *
 * @return  Size of received data in bytes.
 */
uint32_t uart_0_read_rb_irq(uint8_t *data, uint32_t size);

/**
 * @brief   Flush UART 0 transmit ring buffer.
 */
void uart_0_flush_tx_rb(void);

/**
 * @brief   Flush UART 0 receive ring buffer.
 */
void uart_0_flush_rx_rb(void);

#ifdef __cplusplus
}
#endif

#endif /* UART_H_ */
