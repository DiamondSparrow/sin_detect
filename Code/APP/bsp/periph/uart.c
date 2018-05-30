/**
 **********************************************************************************************************************
 * @file        uart.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       UART C source file.
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

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdio.h>
#include <string.h>

#include "bsp/periph/uart.h"

#include "chip.h"
#include "ring_buffer.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
/** UART 0 transmit data buffer. */
static uint8_t uart_0_tx_data[UART_0_TX_DATA_SIZE + 1] = {0};
/** UART 0 receive data buffer. */
uint8_t uart_0_rx_data[UART_0_RX_DATA_SIZE + 1] = {0};
/** UART 0 transmit ring buffer. */
STATIC RINGBUFF_T uart_0_tx_rb = {0};
/** UART 0 receive ring buffer. */
STATIC RINGBUFF_T uart_0_rx_rb = {0};

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
void uart_0_init(void)
{
    uint32_t tmp = 0;

    RingBuffer_Init(&uart_0_tx_rb, uart_0_tx_data, 1, UART_0_TX_DATA_SIZE);
    RingBuffer_Init(&uart_0_rx_rb, uart_0_rx_data, 1, UART_0_RX_DATA_SIZE);

    /* UART signals on pins PIO0_19 (FUNC3, U0_TXD) and PIO0_18 (FUNC2, U0_RXD) */
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18, (IOCON_FUNC1 | IOCON_MODE_PULLUP));
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 19, (IOCON_FUNC1 | IOCON_MODE_PULLUP));

    /* Setup UART for 115.2K8N1 */
    Chip_UART0_Init(LPC_USART0);
    Chip_UART0_SetBaudFDR(LPC_USART0, UART_0_BAUDRATE);

    Chip_UART0_ConfigData(LPC_USART0, (UART0_LCR_WLEN8 | UART0_LCR_SBS_1BIT | UART0_LCR_PARITY_DIS));
    //Chip_UART0_SetupFIFOS(LPC_USART0, (UART0_FCR_FIFO_EN | UART0_FCR_RX_RS | UART0_FCR_TX_RS)); //  | UART0_FCR_TRG_LEV2

    /* Enable Software Flow Control    */
    Chip_UART0_TXEnable(LPC_USART0);
    /* Disable Software Flow Control    */
    //Chip_UART0_TXDisable(LPC_USART0);

    /* Read to clear the line status.   */
    tmp = Chip_UART0_ReadLineStatus(LPC_USART0);

    /* Ensure a clean start, no data in either TX or RX FIFO.   */
    while((LPC_USART0->LSR & (UART0_LSR_THRE | UART0_LSR_TEMT)) != (UART0_LSR_THRE | UART0_LSR_TEMT));
    while( LPC_USART0->LSR & UART0_LSR_RDR)
    {
        tmp = Chip_UART0_ReadByte(LPC_USART0); /* Dump data from RX FIFO */
    }
    tmp |= tmp;

    /* Enable receive data and line status interrupt */
    Chip_UART0_IntEnable(LPC_USART0, (UART0_IER_THREINT | UART0_IER_RBRINT));

    /* Enable UART 0 interrupt */
    NVIC_EnableIRQ(USART0_IRQn);

    return;
}

void uart_0_send_blocking(const uint8_t data[], uint32_t size)
{
    Chip_UART0_SendBlocking(LPC_USART0, data, size);

    return;
}

uint32_t uart_0_send_rb_empty(void)
{
    return (RingBuffer_IsEmpty(&uart_0_tx_rb));
}

uint32_t uart_0_send_rb_irq(uint8_t *data, uint32_t size)
{
    return Chip_UART0_SendRB(LPC_USART0, &uart_0_tx_rb, data, size);
}

uint32_t uart_0_read_rb_irq(uint8_t *data, uint32_t size)
{
    return Chip_UART0_ReadRB(LPC_USART0, &uart_0_rx_rb, data, size);
}

void uart_0_flush_tx_rb(void)
{
    RingBuffer_Flush(&uart_0_tx_rb);

    return;
}

void uart_0_flush_rx_rb(void)
{
    RingBuffer_Flush(&uart_0_rx_rb);

    return;
}

/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
/**
 * @brief   USART 0 IRQ handler.
 */
void USART0_IRQHandler(void)
{
    Chip_UART0_IRQRBHandler(LPC_USART0, &uart_0_rx_rb, &uart_0_tx_rb);

    return;
}

