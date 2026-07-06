import sounddevice as sd
import numpy as np
import requests
import time

# -----------------------------
# SETTINGS
# -----------------------------
SAMPLE_RATE = 44100
CHUNK_SIZE = 4410
THRESHOLD = 0.25

# Replace with your ESP32 IP Address
ESP32_IP = "http://10.126.3.246"

# -----------------------------
# TRACKER VARIABLES
# -----------------------------
switch_on = False
last_clap_time = 0

print("========================================")
print(" IoT ClapSwitch Started")
print(" Microphone is LIVE!")
print(f" ESP32 Address: {ESP32_IP}")
print(" Clap to toggle the LED.")
print("========================================")


def send_command(command):
    """
    Sends ON or OFF command to the ESP32.
    """

    try:
        response = requests.get(f"{ESP32_IP}/{command}", timeout=2)

        if response.status_code == 200:
            print(f" ESP32 Response: {response.text}")
        else:
            print(f" HTTP Error: {response.status_code}")

    except requests.exceptions.RequestException:
        print(" Could not connect to the ESP32.")
        print(" Make sure the ESP32 is powered on and connected to Wi-Fi.")


def audio_callback(indata, frames, time_info, status):

    global switch_on
    global last_clap_time

    current_time = time.time()

    peak_volume = np.max(np.abs(indata))

    # Detect clap
    if peak_volume > THRESHOLD and (current_time - last_clap_time) > 0.5:

        switch_on = not switch_on
        last_clap_time = current_time

        print("\n----------------------------------------")
        print(f" Clap Detected!")
        print(f" Peak Volume: {peak_volume:.2f}")

        if switch_on:

            print(" Sending ON command...")
            send_command("on")

        else:

            print(" Sending OFF command...")
            send_command("off")

        print("----------------------------------------")


try:

    with sd.InputStream(
        callback=audio_callback,
        channels=1,
        samplerate=SAMPLE_RATE,
        blocksize=CHUNK_SIZE
    ):

        while True:
            time.sleep(1)

except KeyboardInterrupt:

    print("\nProgram terminated by user.")