# IoT-Smart-Energy-Meter-System

The IoT-Based Smart Energy Monitoring System uses IoT technology to trackelectricity usage in real time.
Using sensors to measure voltage and current, thesystem determines power consumption in kWh.An LCD screen shows
the totalnumber of units used and the estimated cost, which is then sent to a cloudplatform for remote monitoring. This
system encourages effective energymanagement and assists users in monitoring their electricity consumption.
Itsupports intelligent energy monitoring systems and lessens the need for manual meter reading.

# Schematic

<img width="601" height="425" alt="k" src="https://github.com/user-attachments/assets/e440ce78-d5da-47a5-8431-d71eb6f084ef" />

# PCB

<img width="482" height="467" alt="Screenshot 2026-05-22 103205" src="https://github.com/user-attachments/assets/d7370000-71db-4a1a-8cdb-3617b116bae1" />

# Working Algorithm

1. Start the system.
2. Initialize all hardware components:
• ESP32 microcontroller
• LCD display
• Voltage sensor
• Current sensor
• Ultrasonic sensor
• Wi-Fi module
3. Establish Wi-Fi connection and connect to the cloud platform/server.
4. Begin continuous monitoring loop.
5. Read electrical parameters from sensors:
• Voltage (V)
• Current (I)
• Power Factor (PF)
6. Calculate power and unit consumption based on measured values.
7. Display the measured parameters (Voltage, Current, Power Factor) on the
LCD.
8. Upload the electrical data to the cloud platform for remote monitoring.
9. Check ultrasonic sensor to detect any physical interaction or movement near
the energy meter box.
10. If movement/object is detected, then:
• Mark condition as Theft/Intrusion Detected.
• Display “THEFT ALERT” message on LCD.
• Send alert notification to the consumer and distributor through cloud
Platform.
11. Else:
• Continue normal monitoring and data updating process.
12. Stop the process only when the system is powered OFF.
