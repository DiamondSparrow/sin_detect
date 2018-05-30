/**
 **********************************************************************************************************************
 * @file        gpio.c
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       GPIO C source file.
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

#include "bsp/periph/gpio.h"

#include "chip.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
/**
 * @brief   GPIO data structure.
 */
typedef struct
{
    uint8_t port;   //!< GPIO port.
    uint8_t pin;    //!< GPIO pin.
    uint8_t dir;    //!< GPIO direction: 0 - input, 1 - output.
    uint8_t state;  //!< GPIO state: 0 - low, 1 high.
    uint32_t mode;  //!< GPIO function / mode.
} gpio_data_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/
const gpio_data_t gpio_list[GPIO_ID_LAST] =
{
    [GPIO_ID_LED_RED] =
    {
        .port = 2, .pin = 17, .dir = 1, .state = 1,
        .mode = (IOCON_FUNC0 | IOCON_MODE_INACT),
    },
    [GPIO_ID_LED_GREEN] =
    {
        .port = 2, .pin = 16, .dir = 1, .state = 1,
        .mode = (IOCON_FUNC0 | IOCON_MODE_INACT),
    },
    [GPIO_ID_LED_BLUE] =
    {
        .port = 2, .pin = 18, .dir = 1, .state = 1,
        .mode = (IOCON_FUNC0 | IOCON_MODE_INACT),
    },
};

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
void gpio_init(void)
{
    uint8_t i = 0;

    Chip_GPIO_Init(LPC_GPIO);
    
    Chip_Clock_SetIOCONFiltClockDiv(0, 255);

    for(i = 0; i < GPIO_ID_LAST; i++)
    {
        Chip_IOCON_PinMuxSet(LPC_IOCON, gpio_list[i].port, gpio_list[i].pin, gpio_list[i].mode);
        if(gpio_list[i].dir)
        {
            Chip_GPIO_SetPinDIROutput(LPC_GPIO, gpio_list[i].port, gpio_list[i].pin);
            Chip_GPIO_SetPinState(LPC_GPIO, gpio_list[i].port, gpio_list[i].pin, gpio_list[i].state);
        }
        else
        {
            Chip_GPIO_SetPinDIRInput(LPC_GPIO, gpio_list[i].port, gpio_list[i].pin);
        }
    }

    return;
}

void gpio_output(gpio_id_t id)
{
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin);

    return;
}

void gpio_output_set(gpio_id_t id, bool value)
{
    Chip_GPIO_SetPinState(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin, value);

    return;
}

void gpio_output_low(gpio_id_t id)
{
    Chip_GPIO_SetPinOutLow(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin);

    return;
}

void gpio_output_high(gpio_id_t id)
{
    Chip_GPIO_SetPinOutHigh(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin);

    return;
}

void gpio_output_toggle(gpio_id_t id)
{
    Chip_GPIO_SetPinToggle(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin);

    return;
}

void gpio_input(gpio_id_t id)
{
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, gpio_list[id].port, gpio_list[id].pin);

    return;
}

bool gpio_input_read(gpio_id_t id)
{
    return (bool)(Chip_GPIO_ReadValue(LPC_GPIO, gpio_list[id].port) & (1 << gpio_list[id].pin));
}

/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
