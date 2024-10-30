# Multitasking-with-ESP32

## Project Overview

This project aims to develop a multitasking application using the ESP32 microcontroller and FreeRTOS to read, process anda store data from various sensors concurrently. By leveraging the capabilities of FreeRTOS, we can efficiently handle multiple tasks, enhancing the performance of sensor data acquisition compared to traditional sequential approaches.

## Objectives

- To implement a multitasking system using FreeRTOS on the ESP32.
- To read values from different sensors concurrently.
- To analyze and compare the efficiency of concurrent programming versus sequential programming.
- To gain practical experience in embedded systems programming and real-time operating systems.

## Technologies Used

- **Microcontroller:** ESP32
- **Operating System:** FreeRTOS
- **Programming Language:** C/C++
- **Development Environment:** PlatformIO
- **Sensors:** 

## System Architecture

The system architecture consists of the ESP32 microcontroller handling multiple sensor tasks. Each sensor is assigned a dedicated task in FreeRTOS, allowing them to run concurrently. The following diagram illustrates the architecture:

## Implementation

### Setup Instructions

### Code Structure

- `main.c`: The main entry point where tasks are created.
- `sensor_task.c`: Implementation of the task to read data from the sensors.
- `utils.c`: Utility functions for data processing.

## Efficiency Analysis

To evaluate the performance of the concurrent approach, we will compare: (TODO)

## Results


## Conclusion


## Future Work

- Implement additional sensors and tasks for further testing.
- Explore more advanced scheduling techniques in FreeRTOS.
- Develop a user interface for real-time data visualization.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

---

Feel free to adjust any section or add specific details related to your implementation and findings!
