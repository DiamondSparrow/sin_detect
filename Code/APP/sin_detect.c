/**
 **********************************************************************************************************************
 * @file        sinus_detect.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-06-13
 * @brief       Sinusoidal signal frequency detection C source file.
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
#define SIN_DETECT_CYCLES       32                      //!< Cycles count after witch is reached will calculate frequency.
#define SIN_DETECT_LP_CUTOFF    0.5F                    //!< Sin detection low pass filter cutoff.
#define SIN_DETECT_ZERO         (ADC_RESOLUTION / 2)    //!< Zero level.
#define SIN_DETECT_FREQ_LOW     100.0F                  //!< Sin detection low frequency in Hz.
#define SIN_DETECT_FREQ_HIGH    300.0F                  //!< Sin detection low frequency in Hz.
#define SIN_DETECT_FREQ_HYS     2.0F                    //!< Sin detection hysteresis level

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
/**
 * @brief   Sinusoidal signal frequency detection data structure.
 */
typedef struct
{
    uint32_t accumulator;       /**< Counter accumulator. */
    uint32_t counter;           /**< Counter for time. */
    uint32_t cycles;            /**< Cycles counter after which is reached will calculate frequency. */
    uint32_t last_signal;       /**< Last signal value. */
    uint32_t current_signal;    /**< Current signal value. */
    float frequncy;             /**< Measured sinusoidal signal frequency, */
    bool state;                 /**< Flag that show if frequency is between range @ref SIN_DETECT_FREQ_LOW
                                     and @ref SIN_DETECT_FREQ_HIGH. true - yes, false - no. */
} sin_detect_data_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
/** Sinusoidal signal frequency detection data. See @ref sin_detect_data_t. */
volatile sin_detect_data_t sin_detect_data = {0};
/** Son detection low pass filter data. See @ref filters_low_pass_t. */
volatile filters_low_pass_t sin_detect_lp_filter = {0};

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/
/**
 * @brief   Calculate sinusoidal signal frequency.
 *
 * @param   data    Pointer to Sinusoidal signal frequency detection data. See @ref sin_detect_data_t.
 * @param   signal  Sinusoidal signal.
 */
static void sin_detect_frquency(sin_detect_data_t *data, uint32_t signal);

/**
 * @brief   Control led by sinusoidal signal frequency.
 *
 * @param   freq    Frequency in Hz by which led will be controlled.
 * @param   state   Current state of whether frequency was in a range.
 *
 * @return  State of whether frequency is in the range.
 * @retval  0   frequency is not in the range.
 * @retval  1   frequency is in the range.
 */
static bool sin_detect_led_control(float freq, bool state);

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
bool sin_detect_init(void)
{
    timers_32_0_start(SIN_DETECT_RATE);

    return true;
}

void sin_detect_process(uint32_t signal)
{
    // Calculate frequency.
    sin_detect_frquency((sin_detect_data_t *)&sin_detect_data, signal);

    // Control led.
    sin_detect_data.state = sin_detect_led_control(sin_detect_data.frequncy, sin_detect_data.state);

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
static void sin_detect_frquency(sin_detect_data_t *data, uint32_t signal)
{
    float freq = 0;

    // Pass signal to low pass filter.
    data->current_signal = (uint32_t) filters_low_pass(
            (filters_low_pass_t *) &sin_detect_lp_filter, (float) signal,
            SIN_DETECT_LP_CUTOFF);

    // Increment time measurement counter.
    data->counter++;

    // Check if sin signal is at zero level.
    if((data->last_signal > SIN_DETECT_ZERO && data->current_signal <= SIN_DETECT_ZERO)
        || (data->last_signal < SIN_DETECT_ZERO && data->current_signal >= SIN_DETECT_ZERO))
    {
        data->cycles++;
        data->accumulator += data->counter;
        if(data->cycles > SIN_DETECT_CYCLES)
        {
            // Calculate frequency.
            freq = 1.0 / (((float)(float)data->accumulator / (float)data->cycles) * 2.0 * (1.0 / SIN_DETECT_RATE));
            // Save frequency.
            data->frequncy = freq;
            // Clear cycles counter.
            data->cycles = 0;
            // Clear accumulator.
            data->accumulator = 0;
        }
        // Clear time counter.
        data->counter = 0;
    }

    // Save last signal.
    data->last_signal = signal;

    return;
}

static bool sin_detect_led_control(float freq, bool state)
{
    bool ret = false;

    if(state)
    {
        // Previous frequency was in range
        if(freq >= (SIN_DETECT_FREQ_LOW - SIN_DETECT_FREQ_HYS)
           && freq <= (SIN_DETECT_FREQ_HIGH + SIN_DETECT_FREQ_HYS))
        {
            // In rage [98:302], turn on LED.
            gpio_output_low(GPIO_ID_LED_BLUE);
            ret = true;
        }
        else
        {
            // Out of range, turn off LED
            gpio_output_high(GPIO_ID_LED_BLUE);
            ret = false;
        }
    }
    else
    {
        // Previous frequency was not in a range.
        if(freq >= (SIN_DETECT_FREQ_LOW + SIN_DETECT_FREQ_HYS)
           && freq <= (SIN_DETECT_FREQ_HIGH - SIN_DETECT_FREQ_HYS))
        {
            // In range [102:298], turn on LED.
            gpio_output_low(GPIO_ID_LED_BLUE);
            ret = true;
        }
        else
        {
            // Out of range, turn off LED
            gpio_output_high(GPIO_ID_LED_BLUE);
            ret = false;
        }
    }

    return ret;
}
