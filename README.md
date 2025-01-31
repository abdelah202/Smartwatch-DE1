# DE1 Smartwatch Project

This project implements a digital watch and stopwatch system on the Altera DE1 development board, featuring the NIOS II processor. The system utilizes various components such as SDRAM, UART, and multiple I/O devices (LEDs, buttons, switches, 7-segment displays, and a timer).

## System Overview

### Key Components:

![Watch Project Diagram](images/Diagram.png)


### Description of Inputs and Outputs:

#### Push Buttons:

* **KEY0**: Reset the system.
* **KEY1-3**:
  * **KEY1**: Start the stopwatch.
  * **KEY2**: Pause the stopwatch.
  * **KEY3**: Stop the stopwatch.

#### Switches:

* **Switches 1-9**: Control various operations, such as updating the time on the watch and toggling between the watch and stopwatch displays.

#### 7-Segment Displays:

* **Watch Mode**: Displays the time (hour on 2 displays, minute on 2 displays).
* **Stopwatch Mode**: Displays the elapsed time (minute on 2 displays, second on 2 displays).

### System Features:

* **Time Management**: The system keeps track of time in hours and minutes and displays it on the 7-segment displays.
* **Stopwatch Functionality**: Users can start, pause, and stop a stopwatch to measure elapsed time in minutes and seconds.
* **Display Switching**: The system allows toggling between the current time and the stopwatch view using switches.

### Hardware Requirements:

* **Altera DE1 Board** (with NIOS II processor)
* **8 MB SDRAM** for storing variables and system data
* **JTAG interface** for programming and debugging
* **UART interface** for communication (if needed)

### Software Overview:

The project uses the NIOS II processor and integrates with the peripherals (LEDs, buttons, switches, 7-segment displays, and timers) to implement the digital watch and stopwatch functionality.

### How to Use:

1. **Reset the system**: Press **KEY0** to reset the watch and stopwatch.
2. **Start Stopwatch**: Press **KEY1** to start the stopwatch.
3. **Pause Stopwatch**: Press **KEY2** to pause the stopwatch.
4. **Stop Stopwatch**: Press **KEY3** to stop the stopwatch.
5. **Toggle Display**: Use switch 1 to switch from watch mode to stopwatch mode and vice-versa, and switch 2 to adjust time.
6. **Display the Time**: The current time and stopwatch time will be displayed on the 7-segment displays.

### Future Improvements:

* Adding more functionalities such as alarm settings or countdown timers.
