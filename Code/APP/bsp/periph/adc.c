/**
 **********************************************************************************************************************
 * @file         adc.c
 * @author       Diamond Sparrow
 * @version      1.0.0.0
 * @date         2017-04-11
 * @brief        This is C source file template.
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

#include "bsp/periph/adc.h"

#include "../../sin_detect.h"
#include "chip.h"

/**********************************************************************************************************************
 * Private definitions and macros
 *********************************************************************************************************************/
#define ADC_SEQ_A_CH_COUNT      4           //!< Sequencer A channel count.
#define ADC_SEQ_A_AVG_COUNT     5           //!< Sequencer A average count.

/**********************************************************************************************************************
 * Private typedef
 *********************************************************************************************************************/
typedef struct
{
    uint8_t ch;
    uint32_t value;
    uint32_t acumulator;
    uint8_t counter;
} adc_ch_data_t;

/**********************************************************************************************************************
 * Private constants
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private variables
 *********************************************************************************************************************/
volatile adc_ch_data_t adc_seqa_ch_data[ADC_ID_LAST] =
{
    {.ch = 0, .value = 0, .acumulator = 0, .counter = 0},   // ADC_CH_INA282_1
};

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/
void adc_init(void)
{
    /* Setup ADC for 12-bit mode and normal power */
    Chip_ADC_Init(LPC_ADC, 0);

    /* Use higher voltage trim */
    Chip_ADC_SetTrim(LPC_ADC, ADC_TRIM_VRANGE_LOWV);

    /* Need to do a calibration after initialization and trim. */
    Chip_ADC_StartCalibration(LPC_ADC);
    Chip_ADC_SetClockRate(LPC_ADC, 500000);
    while(!(Chip_ADC_IsCalibrationDone(LPC_ADC)));

    /* Setup ADC clock rate */
    Chip_ADC_SetClockRate(LPC_ADC, ADC_CLK);

    /* Setup sequencer A. */
    Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX,
                            (
                             ADC_SEQ_CTRL_CHANSEL(adc_seqa_ch_data[ADC_ID_SINUS_DETECT].ch) |
                             ADC_SEQ_CTRL_LOWPRIO |
                             ADC_SEQ_CTRL_MODE_EOS
                             ));

    // ADC_ID_SINUS_DETECT
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 9, (IOCON_FUNC3 | IOCON_MODE_INACT | IOCON_ADMODE_EN));

    /* Power up the internal temperature sensor - this also selects the temperature sensor as the input for the ADC0 input */
    Chip_SYSCTL_PowerDown(SYSCTL_POWERDOWN_TS_PD);

    /* Setup threshold 0 low and high values to about 25% and 75% of max */
    Chip_ADC_SetThrLowValue(LPC_ADC, 0, ((1 * 0xFFF) / 4));
    Chip_ADC_SetThrHighValue(LPC_ADC, 0, ((3 * 0xFFF) / 4));
    
    /* Clear all pending interrupts */
    Chip_ADC_ClearFlags(LPC_ADC, Chip_ADC_GetFlags(LPC_ADC));
    /* Enable ADC overrun and sequence A completion interrupts */
    Chip_ADC_EnableInt(LPC_ADC, (ADC_INTEN_SEQA_ENABLE));
    /* Enable ADC NVIC interrupt */
    //NVIC_EnableIRQ(ADC_A_IRQn);

    /* Enable sequencer */
    Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);

    /* Start Burst sequencer. */
    Chip_ADC_StartBurstSequencer(LPC_ADC, ADC_SEQA_IDX);

    return;
}

void adc_a_handler(void)
{
    uint32_t raw = 0;
    adc_id_t id = (adc_id_t)0;
    uint8_t i = 0;

    for(i = 0; i < ADC_SEQ_A_AVG_COUNT; i++)
    {
        for(id = (adc_id_t)0; id < ADC_ID_LAST; id++)
        {
            raw = Chip_ADC_GetDataReg(LPC_ADC, adc_seqa_ch_data[id].ch);
            if(ADC_DR_DATAVALID & raw)
            {
                adc_seqa_ch_data[id].counter++;
                adc_seqa_ch_data[id].acumulator += ADC_DR_RESULT(raw);
            }
            if(i == (ADC_SEQ_A_AVG_COUNT - 1))
            {
                adc_seqa_ch_data[id].value = adc_seqa_ch_data[id].acumulator / adc_seqa_ch_data[id].counter;
                adc_seqa_ch_data[id].acumulator = 0;
                adc_seqa_ch_data[id].counter = 0;
            }
        }
    }
    
    sin_detect_handler(adc_seqa_ch_data[ADC_ID_SINUS_DETECT].value);
    
    return;
}
/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/
