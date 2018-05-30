/**
 **********************************************************************************************************************
 * @file        adc.h
 * @author      Diamond Sparrow
 * @version     1.0.0.0
 * @date        2017-04-11
 * @brief       ADC C header file.
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

#ifndef ADC_H_
#define ADC_H_

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
#define ADC_CLK                 4400000     //!< 1000000, set to 4.4Mhz
#define ADC_VREF                2500.0F     //!< Reference voltage in mV.
#define ADC_RESOLUTION          4096.0F     //!< ADC resolution 12 bit.


#define ADC_CONVERT_MV(ADC_VALUE)   (ADC_VALUE * (ADC_VREF / ADC_RESOLUTION)) //!< Convert ADC value to millivolts.

/**********************************************************************************************************************
 * Exported types
 *********************************************************************************************************************/
/**
 * @brief ADC channel list.
 */
typedef enum
{
    ADC_ID_SINUS_DETECT = 0,    //!< Sinus detection PIN.
    ADC_ID_LAST,                //!< Last should stay last.
} adc_id_t;

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
 * @brief Initialize ADC.
 */
void adc_init(void);
/**
 * @brief   ADC sequencer A handler for timer.
 */
void adc_a_handler(void);

#ifdef __cplusplus
}
#endif

#endif /* ADC_H_ */
