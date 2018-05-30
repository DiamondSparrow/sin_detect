/**
 **********************************************************************************************************************
 * @file        gpio.h
 * @author      Deimantas Zvirblis
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       GPIO C header file.
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

#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdbool.h>

/**********************************************************************************************************************
 * Exported definitions and macros
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
/**
 * @brief   GPIO list with defined pin value.
 */
typedef enum
{
    GPIO_ID_LED_RED     = 0,    //!< RGB LED red.
    GPIO_ID_LED_GREEN   = 1,    //!< RGB LED green.
    GPIO_ID_LED_BLUE    = 2,    //!< RGB LED blue.
    GPIO_ID_LAST,               //!< Last should stay last.
} gpio_id_t;

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
 * @brief   Initialize GPIO's.
 */
void gpio_init(void);

/**
 * @brief   Set pin as output.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 */
void gpio_output(gpio_id_t id);

/**
 * @brief   Set output level.
 *
 * @param   pin     Pin number. See @ref gpio_id_t.
 * @param   value   Pin level: true - high, false - low.
 */
void gpio_output_set(gpio_id_t id, bool value);

/**
 * @brief   Set GPIO to low level.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 */
void gpio_output_low(gpio_id_t id);

/**
 * @brief   Set GPIO to high level.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 */
void gpio_output_high(gpio_id_t id);

/**
 * @brief   Toggle output pin.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 */
void gpio_output_toggle(gpio_id_t id);

/**
 * @brief   Set pin as input.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 */
void gpio_input(gpio_id_t id);

/**
 * @brief   Read input pin value.
 *
 * @param   pin Pin number. See @ref gpio_id_t.
 *
 * @return  State of pin.
 * @retval  0 - low level.
 * @retval  1 - high level.
 */
bool gpio_input_read(gpio_id_t id);

#ifdef __cplusplus
}
#endif

#endif /* GPIO_H_ */
