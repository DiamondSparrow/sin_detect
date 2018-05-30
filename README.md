# sin_detect
Sinusoidal signal detection on NXP LPC11U68Xpresso board.

This program was designed for NXP MCU LPX11U68 (Cortex M0+) running at 48 MHz. 
to measure sinusoidal signal frequency and to indicate whether frequency is in range (from 100 to 300 Hz.) on LED. 

It will measure sinusoidal signal (from 0 to VDD - 3.3 V.) on ADC channel 0 (P1.9) at defined frequency (5 kHz) using timer.

Frequency detection consists of:
1. Measuring sinusoidal signal on ADC and averaging measured value for better accuracy.
2. Measured ADC value will be also passed to low pass filter for better accuracy.
3. Algorithm will detect sinusoidal signal zero point and will count time between points.
4. If zero point is detected it will accumulate time for more than several times (more than one sinusoid) to get better accuracy.
6. When there are enough sinusoid measurements it will calculate sinusoidal signal frequency.
7. Using hysteresis loop it will control LED. Will turn LED if frequency is in defined range, otherwise it will be turned off.

This application also uses CMSIS-RTOS with RTX kernel, windowed watchdog is enabled.
