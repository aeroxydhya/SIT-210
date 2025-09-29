import tkinter as tk
import RPi.GPIO as GPIO

# GPIO Setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# LED pins - USING YOUR EXISTING CONNECTIONS
LED_RED = 17      # Your red LED
LED_GREEN = 27    # Your green LED  
LED_YELLOW = 22   # Your yellow LED (instead of blue)

# Setup GPIO pins as output
GPIO.setup(LED_RED, GPIO.OUT)
GPIO.setup(LED_GREEN, GPIO.OUT)
GPIO.setup(LED_YELLOW, GPIO.OUT)

# Turn off all LEDs initially
GPIO.output(LED_RED, GPIO.LOW)
GPIO.output(LED_GREEN, GPIO.LOW)
GPIO.output(LED_YELLOW, GPIO.LOW)

class LEDControlGUI:
    def __init__(self, master):
        self.master = master
        master.title("LED Controller")
        master.geometry("300x200")
        
        # Variable to track selected LED
        self.selected_led = tk.StringVar()
        self.selected_led.set("none")
        
        self.create_widgets()
        
    def create_widgets(self):
        # Title
        title_label = tk.Label(self.master, text="LED Control Panel", 
                              font=("Arial", 14, "bold"))
        title_label.pack(pady=15)
        
        # Radio buttons - updated for yellow instead of blue
        red_radio = tk.Radiobutton(self.master, text="Red LED", 
                                  variable=self.selected_led, value="red",
                                  command=self.led_changed, font=("Arial", 12))
        red_radio.pack(anchor=tk.W, padx=30, pady=5)
        
        green_radio = tk.Radiobutton(self.master, text="Green LED", 
                                    variable=self.selected_led, value="green",
                                    command=self.led_changed, font=("Arial", 12))
        green_radio.pack(anchor=tk.W, padx=30, pady=5)
        
        yellow_radio = tk.Radiobutton(self.master, text="Yellow LED", 
                                   variable=self.selected_led, value="yellow",
                                   command=self.led_changed, font=("Arial", 12))
        yellow_radio.pack(anchor=tk.W, padx=30, pady=5)
        
        # Status label
        self.status_label = tk.Label(self.master, text="No LED selected", 
                                    font=("Arial", 10), fg="blue")
        self.status_label.pack(pady=10)
        
        # Exit button
        exit_button = tk.Button(self.master, text="Exit Program", 
                               command=self.exit_app, bg="red", fg="white",
                               font=("Arial", 10, "bold"), width=15)
        exit_button.pack(pady=15)
        
    def led_changed(self):
        # Turn off all LEDs first
        GPIO.output(LED_RED, GPIO.LOW)
        GPIO.output(LED_GREEN, GPIO.LOW)
        GPIO.output(LED_YELLOW, GPIO.LOW)
        
        # Turn on selected LED and update status
        selected = self.selected_led.get()
        if selected == "red":
            GPIO.output(LED_RED, GPIO.HIGH)
            self.status_label.config(text="Red LED is ON", fg="red")
        elif selected == "green":
            GPIO.output(LED_GREEN, GPIO.HIGH)
            self.status_label.config(text="Green LED is ON", fg="green")
        elif selected == "yellow":
            GPIO.output(LED_YELLOW, GPIO.HIGH)
            self.status_label.config(text="Yellow LED is ON", fg="orange")
    
    def exit_app(self):
        GPIO.output(LED_RED, GPIO.LOW)
        GPIO.output(LED_GREEN, GPIO.LOW)
        GPIO.output(LED_YELLOW, GPIO.LOW)
        GPIO.cleanup()
        self.master.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = LEDControlGUI(root)
    root.mainloop()