/**
 **********************************************************************************************************************
 * @file        bsp.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       Board Support (BSP) layer C source file.
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
#include <stdint.h>

#include "bsp.h"
#include "chip.h"
#include "flash.h"

#include "periph/adc.h"
#include "periph/gpio.h"
#include "periph/timers.h"
#include "periph/uart.h"
#include "periph/wdt.h"

#include "cmsis_os2.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
/** LPCOpen Configuration Global variables: Oscillator rate in Hz. */
const uint32_t OscRateIn = 12000000UL;
/** LPCOpen Configuration Global variables: RTC oscillator rate in Hz. */
const uint32_t RTCOscRateIn = 32768UL;

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
static uint32_t bsp_rst_status = 0;
/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/
extern uint32_t SystemCoreClock;

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/
static void bsp_read_rst_status(void);

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
void bsp_init(void)
{
    SystemCoreClockUpdate();

    wdt_init();
    adc_init();
    gpio_init();
    uart_0_init();
    timers_32_0_init();

    bsp_read_rst_status();

    return;
}

uint32_t bsp_get_reset_cause(void)
{
    return bsp_rst_status;
}

uint32_t bsp_get_core_clock(void)
{
    return SystemCoreClock;
}

/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
static void bsp_read_rst_status(void)
{
    bsp_rst_status = Chip_SYSCTL_GetSystemRSTStatus();
    Chip_SYSCTL_ClearSystemRSTStatus(bsp_rst_status);

    return;
}
