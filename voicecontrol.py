import serial
import speech_recognition as sr
import time
import os
import warnings

# Suppress ALSA warnings
warnings.filterwarnings("ignore", category=DeprecationWarning)
os.environ["PYTHONWARNINGS"] = "ignore"

# Connect to Arduino Nano 33 IoT
arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
time.sleep(2)  # allow Arduino to reset
print("Arduino connected successfully on /dev/ttyACM0")

# Initialize speech recognizer
recognizer = sr.Recognizer()

# Use your USB mic: card 3, device 0
mic_index = None
for i, name in enumerate(sr.Microphone.list_microphone_names()):
    if "USB Composite Device" in name:
        mic_index = i
        break

if mic_index is None:
    print("USB mic not found! Using default mic.")
    mic_index = 0

mic = sr.Microphone(device_index=mic_index)

print("Voice control ready. Speak a command...")

while True:
    try:
        with mic as source:
            recognizer.adjust_for_ambient_noise(source, duration=1)
            print("Listening...")
            audio = recognizer.listen(source)
        command = recognizer.recognize_google(audio).lower()
        print("You said:", command)

        if "fan on" in command:
            arduino.write(b"fan on\n")
        elif "fan off" in command:
            arduino.write(b"fan off\n")
        elif "light on" in command:
            arduino.write(b"light on\n")
        elif "light off" in command:
            arduino.write(b"light off\n")
        elif "read temp" in command:
            arduino.write(b"read temp\n")
            time.sleep(1)
            temp = arduino.readline().decode().strip()
            print("Temperature:", temp)

    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print(f"Could not request results; {e}")
    except KeyboardInterrupt:
        print("Exiting...")
        break
