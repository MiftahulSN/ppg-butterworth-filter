import serial
import csv
import time

port = "COM4"                                            # input PORT
baud_rate = 115200                                       # input baudrate
output_file = "ppg_raw_data/raw_ppg_1.csv"               # output file name and directory
duration_time = 60                                       # input duration time (seconds)

ser = serial.Serial(port, baud_rate)
time.sleep(1)

with open(output_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Timestamp", "IR_Value"])

    try:
        print("starting in 10 seconds!")
        time.sleep(10)

        print("begin!")

        start_time = time.time()

        while time.time() - start_time < duration_time:
            line = ser.readline().decode('utf-8').strip()
            print(line)
            data = line.split(",")
            writer.writerow(data)

        print("done!")

    except KeyboardInterrupt:
        print("stop writing!")

    finally:
        ser.close()
