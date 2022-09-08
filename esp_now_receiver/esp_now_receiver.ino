#include <esp_now.h>
#include <WiFi.h>
#include <SSD1306Wire.h>

// Define a data structure
typedef struct struct_message {
	int throttle_value;
	int steering_value;
	int light;
};


// Create a structure object
struct_message myData;

// define oled pin 
#define SDA 4 
#define SCL 5 

// for led
char th_buffer[100];
char st_buffer[100];

// initializing OLED screen
SSD1306Wire display(0x3c, SDA, SCL);

// callback function executed when data is received.
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
	memcpy(&myData, incomingData, sizeof(myData));
	/*
	Serial.print("Data received:\t");
	Serial.print(len);
	Serial.print("\tThrottle Value:\t");
	Serial.print(myData.throttle_value);
	Serial.print("\tSteering Value:\t");
	Serial.println(myData.steering_value / 4);
	Serial.println();
	*/


}

void setup() {
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);

	if (esp_now_init() != ESP_OK) {
	  Serial.println("Error initializing ESP-NOW");
	  return;
	}

	// register receive call back function
	esp_now_register_recv_cb(OnDataRecv);
    
	// initializing OLED
	display.init();
	display.flipScreenVertically();
	display.setFont(ArialMT_Plain_10);
}


void loop() {

	// convert data type
	itoa(myData.throttle_value, th_buffer, 10);
	itoa(myData.steering_value, st_buffer, 10);

	// draw string on oled 
	display.drawString(0, 0, "Steering: ");
	display.drawString(48, 0, st_buffer);

	display.drawString(0, 16, "Throttle: ");
	display.drawString(48, 0, th_buffer);

	// show 
	display.display();
	delay(200);
	display.clear();
}
