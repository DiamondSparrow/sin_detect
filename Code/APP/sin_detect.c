/**
 **********************************************************************************************************************
 * @file        sinus_detect.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-06-13
 * @brief       Sinus signal detection C source file.
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
#include <string.h>

#include "bsp/periph/adc.h"
#include "bsp/periph/gpio.h"
#include "bsp/periph/timers.h"

#include "debug.h"
#include "sin_detect.h"
#include "filters.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define SIN_DETECT_RATE         5000.0F                 //!< Sin detection rate in Hz.
#define SIN_DETECT_CYCLES       32                      //!< Cycles count after witch is reached will calculate frequency.
#define SIN_DETECT_LP_CUTOFF    0.5F                    //!< Sin detection low pass filter cutoff.
#define SIN_DETECT_ZERO         (ADC_RESOLUTION / 2)    //!< Zero level.
#define SIN_DETECT_FREQ_LOW     100.0F                  //!< Sin detection low frequency in Hz.
#define SIN_DETECT_FREQ_HIGH    300.0F                  //!< Sin detection low frequency in Hz.
#define SIN_DETECT_FREQ_HYS     2.0F                    //!< Sin detection hysteresis level

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct
{
    uint32_t accumulator;
    uint32_t counter;
    uint32_t cycles;
    uint32_t last_signal;
    uint32_t current_signal;
    float frequncy;
    bool state;
} sin_detect_data_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
/** Sin detection measurements data. See @ref sin_detect_data_t. */
volatile sin_detect_data_t sin_detect_data = {0};
/** Son detection low pass filter data. See @ref filters_low_pass_t. */
volatile filters_low_pass_t sin_detect_lp_filter = {0};

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
bool sin_detect_init(void)
{
    timers_32_0_start(SIN_DETECT_RATE);

    return true;
}

void sin_detect_handler(uint32_t signal)
{
    float freq = 0;

    // Save current signal.
    sin_detect_data.current_signal = signal;
    // Increment time measurement counter.
    sin_detect_data.counter++;

    // Check if sin signal is at zero level.
    if((sin_detect_data.last_signal > SIN_DETECT_ZERO && sin_detect_data.current_signal <= SIN_DETECT_ZERO)
        || (sin_detect_data.last_signal < SIN_DETECT_ZERO && sin_detect_data.current_signal >= SIN_DETECT_ZERO))
    {
        sin_detect_data.cycles++;
        sin_detect_data.accumulator += sin_detect_data.counter;
        if(sin_detect_data.cycles > SIN_DETECT_CYCLES)
        {
            freq = 1.0 / (((float)(float)sin_detect_data.accumulator / (float)sin_detect_data.cycles) * 2.0 * (1.0 / SIN_DETECT_RATE));
            freq = filters_low_pass((filters_low_pass_t *)&sin_detect_lp_filter, freq, SIN_DETECT_LP_CUTOFF);
            sin_detect_data.frequncy = freq;
            sin_detect_data.cycles = 0;
            sin_detect_data.accumulator = 0;

            if(sin_detect_data.state)
            {
                if(freq >= (SIN_DETECT_FREQ_LOW - SIN_DETECT_FREQ_HYS) && freq <= (SIN_DETECT_FREQ_HIGH + SIN_DETECT_FREQ_HYS))
                {
                    // In rage [98:302], turn on LED.
                    gpio_output_low(GPIO_ID_LED_BLUE);
                    sin_detect_data.state = true;
                }
                else
                {
                    // Out of range, turn off LED
                    gpio_output_high(GPIO_ID_LED_BLUE);
                    sin_detect_data.state = false;
                }
            }
            else
            {
                if(freq >= (SIN_DETECT_FREQ_LOW + SIN_DETECT_FREQ_HYS) && freq <= (SIN_DETECT_FREQ_HIGH - SIN_DETECT_FREQ_HYS))
                {
                    // In range [102:298], turn on LED.
                    gpio_output_low(GPIO_ID_LED_BLUE);
                    sin_detect_data.state = true;
                }
                else
                {
                    // Out of range, turn off LED
                    gpio_output_high(GPIO_ID_LED_BLUE);
                    sin_detect_data.state = false;
                }
            }
        }
        sin_detect_data.counter = 0;
    }

    // Save last signal.
    sin_detect_data.last_signal = signal;
    
    return;
}

void sin_detect_debug(void)
{
    DEBUG("Sin detect: %d, %.03f Hz;",  sin_detect_data.state, sin_detect_data.frequncy);

    return;
}

/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
