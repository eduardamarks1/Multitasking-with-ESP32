# Multitasking with ESP32

## Project Overview

This project demonstrates multitasking with the ESP32 microcontroller, using FreeRTOS to read, process, and store data from multiple sensors concurrently. The goal is to compare the efficiency of concurrent versus sequential data acquisition and storage, leveraging FreeRTOS for enhanced task management. The system utilizes two sensors, the BMP280 for environmental data (pressure, temperature, altitude) and the MPU6050 for motion data (acceleration, gyroscope).

## Objectives

- Implement multitasking using FreeRTOS on the ESP32.
- Concurrently read data from the BMP280 and MPU6050 sensors.
- Compare the performance of concurrent versus sequential programming.
- Gain practical experience in embedded systems programming, sensor integration, and real-time operating systems.

## Technologies Used

- **Microcontroller:** ESP32
- **Operating System:** FreeRTOS
- **Programming Language:** C/C++
- **Development Environment:** PlatformIO
- **Sensors:** BMP280 (pressure, temperature, altitude) and MPU6050 (acceleration, gyroscope)

## System Architecture

The system uses the ESP32 microcontroller to handle multiple tasks concurrently. Four tasks are implemented:

1. **Reading Data from the BMP280 Sensor:** This task reads pressure, altitude, and temperature data from the BMP280 sensor.
2. **Reading Data from the MPU6050 Sensor:** This task reads acceleration, gyroscope, and temperature data from the MPU6050 sensor.
3. **Storing BMP280 Data:** This task stores the BMP280 sensor data in a text file in the internal flash memory (SPIFFS).
4. **Storing MPU6050 Data:** This task stores the MPU6050 sensor data in a separate text file in SPIFFS.

A mutex is used to synchronize access to the files, and queues are used to communicate data between the reading tasks and the storage tasks. 

The system runs for a predefined duration (e.g., 30 seconds), which can be changed as needed, reading sensor values at specified intervals. During this period, data is saved to text files. Once the execution time ends, the stored data is printed to the serial console for review. 

### Setup Instructions

The system uses the ESP32 microcontroller to handle multiple tasks concurrently. Two main tasks are implemented: one for reading and processing data from the BMP280 sensor, and another for the MPU6050 sensor. Data from both sensors is saved in the internal flash memory (SPIFFS) in separate files. A mutex and queues are used for synchronization and task communication.

The system runs for a specified duration (e.g., 30 seconds), during which data is read at defined intervals and stored in text files. At the end of the execution, the saved data is printed to the serial console for review.

### Setup and Instructions:

1. **Install PlatformIO**: Make sure you have PlatformIO installed and set up for ESP32 development correctly.
2. **Upload the Code**: Upload the provided code to your ESP32 device.
3. **Connect the Sensors**: Ensure the BMP280 and MPU6050 sensors are properly connected to the ESP32 via I2C.
4. **Monitor Serial Output**: Use the serial monitor to view debug messages and results.
5. **Run Python Script**: After uploading the code to the ESP32, open the terminal on your computer. Run the Python script that reads the serial data and saves it to a text file. This script should be placed in the same folder as your PlatformIO project.

If you encounter any issues or have doubts, just copy the `platform.ini` configuration we used and follow the steps above to ensure everything is set up properly.

Let us know if you'd like any further adjustments!

### Code Structure

- `main_conc.cpp`: The main file implementing the multitasking system with FreeRTOS. It initializes the sensors, creates tasks for reading and saving data, and manages synchronization with mutexes and queues.
- `main_seq.cpp`: A sequential version of the same system for comparison. It reads and saves sensor data sequentially instead of concurrently.
- `salvar_serial.py`: A Python script to read the sensor data from the ESP32’s serial output and save it to a text file for post-processing.

### Key Features

- **Multitasking with FreeRTOS**: Tasks for reading and saving sensor data are managed concurrently.
- **Data Synchronization**: Mutexes are used to prevent race conditions when accessing files for saving data.
- **Data Storage**: Sensor data is saved to internal flash (SPIFFS) in two separate text files: one for BMP280 data and one for MPU6050 data.
- **Data Review**: At the end of the run, the data saved in the flash is printed to the serial monitor for review.

## Efficiency Analysis

To evaluate the performance of the concurrent approach, we compare the multitasking approach (using FreeRTOS) with a sequential version of the same system. This will help assess the efficiency and responsiveness of the system, particularly in terms of how well it handles multiple sensor inputs simultaneously.

### Key Metric:

The impact of concurrency on the overall numbers of readings and data stored, especially with a larger discrepancy between the reading intervals of the two sensors.

## Results

At the conclusion of the execution, the system prints the number of reads and saves for each sensor, along with the data stored in the respective files. The output will allow you to compare the results between the two implementations and assess the performance improvements achieved by FreeRTOS multitasking. The performance improvement is most noticeable when the intervals of the two sensors differ significantly, where the concurrent approach leads to more efficient handling of data collection and storage.

## Conclusion

This project demonstrates how FreeRTOS on the ESP32 can efficiently handle concurrent tasks, such as reading from multiple sensors and saving the data simultaneously. The system provides a robust framework for managing real-time sensor data acquisition in embedded systems.

## Future Work

- Add additional sensors (e.g., temperature, humidity, etc.) for further testing.
- Explore more advanced scheduling techniques and task synchronization in FreeRTOS.
