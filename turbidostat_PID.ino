#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensor_pin = A0;
const int pump_pin = 5;
const float pump_speed_factor = 0.016;

// Variables for the PID controller
float setpoint = 2.2; // Desired sensor voltage
float kp = 1.0; // Proportional gain
float ki = 0.0; // Integral gain
float kd = 0.0; // Derivative gain
float last_error = 0.0;
float integral = 0.0;
float output = 0.0;

// Variables for controlling the pump
unsigned long last_pump_update = 0;
unsigned long pump_update_interval = 30000; // Update pump every 30 seconds
float pump_speed = 0.0;

void setup() {
  Serial.begin(9600);

  pinMode(pump_pin, OUTPUT);
  digitalWrite(pump_pin, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  //delay(2000);
  display.clearDisplay();
}

void loop() {
  float sensor_voltage = analogRead(sensor_pin) * (5.0 / 1023.0);
  Serial.print("Sensor voltage: ");
  Serial.println(sensor_voltage);

  // Calculate the error
  float error = setpoint - sensor_voltage;

  // Calculate the PID output
  unsigned long current_time = millis();
  float dt = (current_time - last_pump_update) / 1000.0;
  integral += error * dt;
  float derivative = (error - last_error) / dt;
  output = kp * error + ki * integral + kd * derivative;

  // Update the pump speed
  pump_speed = max(0.0, pump_speed + output);
  pump_speed = min(1.0, pump_speed);
  digitalWrite(pump_pin, HIGH);
  delay(pump_speed * pump_speed_factor * 60000.0);
  digitalWrite(pump_pin, LOW);
  delay(1000);

  // Update the last pump update time
  last_pump_update = current_time;

  // Display the sensor voltage and pump speed on the OLED screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Time  Voltage  Speed");
  display.println("--------------------");
  display.print(millis() / 1000);
  display.print("    ");
  display.print(sensor_voltage, 2);
  display.print("   ");
  display.print(pump_speed * 100, 2);
  display.println("%");
  display.display();

  // Check if the sensor voltage is above the setpoint and turn off the PID controller
  if (sensor_voltage > 2.2) {
    last_error = 0.0;
    integral = 0.0;
    output = 0.0;
    pump_speed = 0.0;
    digitalWrite(pump_pin, LOW);
  }

  // Delay for 30 seconds before updating again
  delay(3000);
}
