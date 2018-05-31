# sin_detect
Sinusoidal signal frequency detection on NXP LPC11U68Xpresso board.

This program was designed for NXP MCU LPC11U68 (Cortex M0+) running at 48 MHz. 
to measure sinusoidal signal frequency and to indicate whether frequency is in range (from 100 to 300 Hz.) on LED. 
It will measure sinusoidal signal (from 0 to VDD - 3.3 V.) on ADC channel 0 (PIO1.9) at defined frequency (5 kHz.) using timer.

Frequency detection consists of:
1. Measuring sinusoidal signal on ADC and averaging measured value for better accuracy.
2. Algorithm will detect sinusoidal signal zero point and will count time between points.
3. If zero point is detected it will accumulate time for more than several times (more than one sinusoid) to get better accuracy.
4. When there are enough sinusoid measurements it will calculate sinusoidal signal frequency.
5. Calculated sinusoidal signal frequency will be passed to low pass filter for better accuracy.
6. Using hysteresis loop LED will be controlled: will turn on if frequency is in defined range, otherwise it will be turned off.

This application also uses CMSIS-RTOS with RTX kernel, windowed watchdog is enabled.
For drivers lpcopen (manufacturer provided drivers) was used.
On UART-0 (TX pin PIO0_19, 115200, 8N1) system will print debug information, like measured sinusoidal signal frequency:
```
Sin detect: 0, 72.096 Hz;
Sin detect: 1, 128.064 Hz;
Sin detect: 1, 256.128 Hz;
Sin detect: 0, 384.512 Hz;
```
First number indicates whether frequency is in the range (0 – no, 1 – yes), second shows measured frequency in hertz.

There are two projects: one for Eclipse CDT (because it's way better IDE for editing), and other – Keil 5 project for compiling and debugging.

Code is written in C and for commenting doxygen style was used.