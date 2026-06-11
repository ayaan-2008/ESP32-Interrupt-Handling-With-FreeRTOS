# ESP32 FreeRTOS Dual Interrupt Manager

## Overview

This project demonstrates how multiple hardware interrupts can be handled efficiently on the ESP32 using FreeRTOS queues.

Two push buttons generate interrupts of equal priority. The Interrupt Service Routines (ISRs) remain lightweight by simply sending events to a FreeRTOS queue. A dedicated interrupt-processing task receives and handles the events.

This architecture follows industry best practices for real-time embedded systems.

## Features

- Dual Hardware Interrupts
- FreeRTOS Multitasking
- ISR-to-Task Communication
- Queue-Based Event Handling
- LED Status Indicators
- Sensor Simulation Task
- Concurrent Task Execution

## Hardware

- ESP32
- 2 Push Buttons
- 3 LEDs

## Tasks

### LED Task
Blinks the system status LED every 500ms.

### Sensor Task
Generates simulated sensor values every second.

### Interrupt Task
Processes interrupt events received from the FreeRTOS queue.

## Interrupt Sources

### Button 1
Triggers Interrupt Source 1.

### Button 2
Triggers Interrupt Source 2.

## FreeRTOS Concepts

- Task Scheduling
- Queues
- Inter-Task Communication
- ISR Safe APIs
- Priority-Based Scheduling

## Interrupt Concepts

- Hardware Interrupts
- ISR Design
- Deferred Interrupt Processing
- Event Queuing

## Technologies

- ESP32
- Arduino Framework
- FreeRTOS
