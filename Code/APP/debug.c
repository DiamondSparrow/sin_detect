/**
 **********************************************************************************************************************
 * @file        debug.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2016-04-10
 * @brief       Debuging C source file.
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
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "bsp/bsp.h"
#include "cmsis_os2.h"

#include "debug.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define DEBUG_BUFFER_SIZE   1024    //!< Debug buffer size in bytes used for message forming.
#define DEBUG_LOCK_TIMEOUT  1000    //!< Debug semaphore lock timeout in milliseconds.

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
/** Debug lock semaphore ID. */
osSemaphoreId_t debug_lock_id;
/** Debug buffer used for message forming. */
uint8_t debug_buffer[DEBUG_BUFFER_SIZE + 1] = {0};

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
bool debug_init(void)
{
    if((debug_lock_id = osSemaphoreNew(1, 1, NULL)) == NULL)
    {
        return false;
    }

    return true;
}

void debug_send(const char *fmt, ...)
{
    uint16_t i = 0;
    va_list args;

    va_start(args, fmt);
    i = vsnprintf((char*)debug_buffer, DEBUG_BUFFER_SIZE, fmt, args);
    // Make sure its zero terminated
    debug_buffer[i] = 0x00;
    uart_0_send_rb_irq(debug_buffer, i);
    va_end(args);

    return;
}

void debug_send_os(const char *fmt, ...)
{
    uint16_t i = 0;
    va_list args;

    if (osSemaphoreAcquire(debug_lock_id, DEBUG_LOCK_TIMEOUT) == osOK)
    {
        va_start(args, fmt);
        i = vsnprintf((char*)debug_buffer, DEBUG_BUFFER_SIZE, fmt, args);
        // Make sure its zero terminated
        debug_buffer[i] = 0x00;
        uart_0_send_rb_irq(debug_buffer, i);
        va_end(args);
        osSemaphoreRelease(debug_lock_id);
    }

    return;
}

void debug_send_blocking(uint8_t *data, uint32_t size)
{
    uart_0_send_blocking(data, size);

    return;
}

void debug_hex_os(uint8_t *buffer, uint16_t size)
{
    uint16_t i = 0;
    uint16_t c = 0;

    if (osSemaphoreAcquire(debug_lock_id, DEBUG_LOCK_TIMEOUT) == osOK)
    {
        for(i = 0; i < size; i++)
        {
            if((c + 8) >= DEBUG_BUFFER_SIZE)
            {
                break;
            }
            if(i > 0)
            {
                c += snprintf((char*)&debug_buffer[c], (DEBUG_BUFFER_SIZE - c), ",%02X", buffer[i]);
            }
            else
            {
                c += snprintf((char*)&debug_buffer[c], (DEBUG_BUFFER_SIZE - c), "[%02X", buffer[i]);
            }
        }
        c += snprintf((char*)&debug_buffer[c], (DEBUG_BUFFER_SIZE - c), "]\r\n");
        uart_0_send_rb_irq(debug_buffer, c);
        osSemaphoreRelease(debug_lock_id);
    }
}

/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/

