/**
 **********************************************************************************************************************
 * @file        pwm.c
 * @author      Diamond Sparrow
 * @version     1.0.0.0
 * @date        2018-05-28
 * @brief       Timers C source file.s
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
#include <stdio.h>

#include "bsp/periph/timers.h"
#include "bsp/periph/adc.h"

#include "chip.h"

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

/**********************************************************************************************************************
 * Exported variables
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Prototypes of local functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported functions
 *********************************************************************************************************************/

void timers_32_0_init(void)
{
    uint32_t freq = 0;

    /* Initialize 32-bit timer 0 clock */
    Chip_TIMER_Init(LPC_TIMER32_0);

    /* Timer rate is system clock rate */
    freq = Chip_Clock_GetSystemClockRate();

    /* Timer setup for match and interrupt at TICKRATE_HZ */
    Chip_TIMER_Reset(LPC_TIMER32_0);

    /* Enable both timers to generate interrupts when time matches */
    Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 0);

    /* Setup prescale value on 32-bit timer to extend range */
    //Chip_TIMER_PrescaleSet(LPC_TIMER32_0, 0xFFFFFFFF);

    /* Setup 16-bit timer's duration (32-bit match time) */
    Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, (freq / 1000));

    /* Setup both timers to restart when match occurs */
    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 0);

    /* Sets external match control (MATn.matchnum) pin control */
    //Chip_TIMER_ExtMatchControlSet(LPC_TIMER32_0, 0, TIMER_EXTMATCH_TOGGLE, 0);

    /* Enable timer. */
    //Chip_TIMER_Enable(LPC_TIMER32_0);

    /* Clear both timers of any pending interrupts */
    NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);

    /* Enable both timer interrupts */
    NVIC_EnableIRQ(TIMER_32_0_IRQn);

    return;
}

void timers_32_0_deinit(void)
{
    /* Start timer */
    Chip_TIMER_Disable(LPC_TIMER32_0);

    /* Shutdown a timer */
    Chip_TIMER_DeInit(LPC_TIMER32_0);

#if TIMER_32_0_IRQ_ENABLE
    /* Clear timer of any pending interrupts */
    NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);

    /* Enable timer interrupts */
    NVIC_DisableIRQ(TIMER_32_0_IRQn);
#endif // TIMER_32_0_IRQ_ENABLE

    return;
}

void timers_32_0_start(uint32_t rate)
{
    uint32_t freq = 0;

    /* Timer rate is system clock rate */
    freq = Chip_Clock_GetSystemClockRate();

    /* Setup 16-bit timer's duration (32-bit match time) */
    Chip_TIMER_SetMatch(LPC_TIMER32_0, 0, (freq / rate));

    /* Start both timers */
    Chip_TIMER_Enable(LPC_TIMER32_0);

    return;
}

void timers_32_0_stop(void)
{
    /* Disable timer. */
    Chip_TIMER_Disable(LPC_TIMER32_0);

    return;
}


/**********************************************************************************************************************
 * Private functions
 *********************************************************************************************************************/

void CT32B0_IRQHandler(void)
{
    if(Chip_TIMER_MatchPending(LPC_TIMER32_0, 0))
    {
        adc_a_handler();
        Chip_TIMER_ClearMatch(LPC_TIMER32_0, 0);
    }

    return;
}
