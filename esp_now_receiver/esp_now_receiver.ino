#include <esp_now.h>
#include <WiFi.h>

// Define a data structure
typedef struct struct_message {
	int throttle_value;
	int steering_value;
	bool turn_on_light;
} struct_message;


// Create a structure object
struct_message myData;

// callback function executed when data is received.
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
	memcpy(&myData, incomingData, sizeof(myData));
	Serial.print("Data received:\t");
	Serial.print(len);
	Serial.print("\tThrottle Value:\t");
	Serial.print(myData.throttle_value);
	Serial.print("\tSteering Value:\t");
	Serial.println(myData.steering_value);
	Serial.println();
}

void setup() {
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);

	if (esp_now_init() != ESP_OK) {
	  Serial.println("Error initializing ESP-NOW");
	  return;
	}

	esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
}
