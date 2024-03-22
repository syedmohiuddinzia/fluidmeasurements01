import serial
import json
from datetime import datetime
import csv
import os

# Configure the serial port
PORT = "COM15'  # Replace with the appropriate serial port on your system
BAUD_RATE = 9600  # Replace with the appropriate baud rate

# Open the serial port
ser = serial.Serial(PORT, BAUD_RATE)

# Get the current date and time
current_datetime = datetime.now()
file_timestamp = current_datetime.strftime('%Y-%m-%d_%H-%M-%S')

# Create a new CSV file with the current date and time as the filename
filename = f'sensor_data_{file_timestamp}.csv'
file_path = os.path.join(os.getcwd(), filename)

# Open the CSV file for writing
with open(file_path, 'w', newline='') as csvfile:
    fieldnames = ['Timestamp', 'Flow Sensor 1', 'Flow Sensor 2', 'Temperature 1', 'Temperature 2', 'Temperature 3', 'Temperature 4', 'Pressure 1', 'Pressure 2']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

    # Write the header row
    writer.writeheader()

    while True:
        # Read a line of data from the serial port
        line = ser.readline().decode().strip()

        # Check if the line is in the expected JSON format
        if line.startswith('{'):
            try:
                # Parse the JSON data
                data = json.loads(line)

                # Extract the relevant values
                flow_sensor_1 = data.get('flowSensor1', '')
                flow_sensor_2 = data.get('flowSensor2', '')
                temperature_1 = data.get('temperature1', '')
                temperature_2 = data.get('temperature2', '')
                temperature_3 = data.get('temperature3', '')
                temperature_4 = data.get('temperature4', '')
                pressure_1 = data.get('pressure1', '')
                pressure_2 = data.get('pressure2', '')

                # Get the current timestamp
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

                # Write the data to the CSV file
                writer.writerow({
                    'Timestamp': timestamp,
                    'Flow Sensor 1': flow_sensor_1,
                    'Flow Sensor 2': flow_sensor_2,
                    'Temperature 1': temperature_1,
                    'Temperature 2': temperature_2,
                    'Temperature 3': temperature_3,
                    'Temperature 4': temperature_4,
                    'Pressure 1': pressure_1,
                    'Pressure 2': pressure_2
                })

            except json.JSONDecodeError:
                print('Invalid JSON data received')

            except Exception as e:
                print(f'Error processing data: {e}')
