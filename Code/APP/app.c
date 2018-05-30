/**
 **********************************************************************************************************************
 * @file        app.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       Main application C source file.
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
#include <stdbool.h>

#include "chip.h"
#include "cmsis_os2.h"

#include "app.h"
#include "debug.h"
#include "bsp/bsp.h"
#include "sin_detect.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
/** Application thread attributes. */
const osThreadAttr_t app_thread_attr =
{
    .name = "APP",
    .stack_size = 1024,
    .priority = osPriorityNormal,
};


/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
/** Application thread id. */
osThreadId_t app_thread_id = NULL;

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/
/**
 * @brief   Application thread.
 *
 * @param   argument    Pointer to  thread arguments.
 */
static void app_thread(void *argument);

/**
 * @brief   Feed watchdog from application.
 */
static void app_wdt_feed(void);

/**
 * @brief   Application error handler.
 */
static void app_error(void);

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
int main(void)
{
    bsp_init();

    DEBUG_BOOT("");
    DEBUG_BOOT(" # Sinus Frequncy Detector #");
    DEBUG_BOOT(" * Booting.");
    DEBUG_BOOT("%-15.15s 0x%X.",        "Reset:",  bsp_get_reset_cause());
    DEBUG_BOOT("%-15.15s %ld MHz.",     "Core Clock:", (bsp_get_core_clock() / 1000000));
    DEBUG_BOOT("%-15.15s v%d.%d-%c%d.", "Version:", APP_VERSION_0, APP_VERSION_1, APP_VERSION_2, APP_VERSION_3);
    DEBUG_BOOT("%-15.15s %s",           "Author:", APP_AUTHOR);
    DEBUG_BOOT("%-15.15s %s %s",        "Build:", __DATE__, __TIME__);
    DEBUG_BOOT("%-15.15s ok.",          "BSP:");

    // Initialize RTOS kernel.
    if(osKernelInitialize() != osOK)
    {
        DEBUG_BOOT("%-15.15s err.",     "OS Kernel:");
        // Invoke error function.
        app_error();
    }
    DEBUG_BOOT("%-15.15s ok.",      "OS Kernel:");

    // Initialize application thread.
    if((app_thread_id = osThreadNew(app_thread, NULL, &app_thread_attr)) == NULL)
    {
        DEBUG_BOOT("%-15.15s err.",     "APP:");
        // Invoke error function.
        app_error();
    }
    DEBUG_BOOT("%-15.15s ok.",      "APP:");

    // Start kernel, if not ready.
    if(osKernelGetState() == osKernelReady)
    {
        if(osKernelStart() != osOK)
        {
            DEBUG_BOOT("%-15.15s err.",      "OS Start:");
            // Invoke error function.
            app_error();
        }
    }

    while(1);
}


/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
static void app_thread(void *argument)
{
    bool ret = false;

    debug_init();

    DEBUG_INIT(" * Initializing.");

    ret = sin_detect_init();
    DEBUG_BOOT("%-15.15s %s.",      "Sin detect:", ret ? "ok" : "err");

    DEBUG_INIT(" * Running.");

    while(1)
    {
        osDelay(100);
        app_wdt_feed();
        sin_detect_debug();
    }
}

static void app_wdt_feed(void)
{
    static uint32_t c = 10;

    if(!c--)
    {
        wdt_feed_soft();
        c = 10;
    }

    return;
}

static void app_error(void)
{
    while(1)
    {
        __nop();
    }
}
