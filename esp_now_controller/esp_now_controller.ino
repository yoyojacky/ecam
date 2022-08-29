#include <esp_now.h>
#include <Wire.h>
#include "WiFi.h"
#include "SSD1306Wire.h"



// Variables for test data
#define SDA 4
#define SCL 5
// 油门 
int throttle_pin = 5;
// 转向
int steering_pin = 7;
// 车灯
bool turn_on_light = false;
// for OLED
char wheels_buffer[100];
char throttle_buffer[100];

// 接收机MAC地址
// E1 控制器， E2 接收机
// E1: 83:F7:03:41:D0:94,  E2: 84:F7:03:A9:4C:98
uint8_t broadcastAddress[] = {0x84, 0xF7, 0x03, 0xA9, 0x4C, 0x98};

typedef struct struct_message {
	int throttle_value;
	int steering_value;
	bool turn_on_light;

} struct_message;

// 实例化
struct_message myData;

// 初始化变量
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	Serial.print("\r\nLast Packet Send Status:\t");
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" :
			"Delivery Fail");
}

// Initialize OLED display
SSD1306Wire display(0x3c, SDA, SCL);

void setup() {
	Serial.begin(115200);
	pinMode(steering_pin, INPUT);

	WiFi.mode(WIFI_STA);
	
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
	}

	esp_now_register_send_cb(OnDataSent);

	memcpy(peerInfo.peer_addr, broadcastAddress, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	if (esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer");
		return;
	}
	// initializing OLED 
	display.init();

	display.flipScreenVertically();
	display.setFont(ArialMT_Plain_10);

}

void loop() {
	int wheels_value= analogRead(steering_pin);
	int throttle_value = analogRead(throttle_pin);

	Serial.print("wheels_value:\t ");
	Serial.print(wheels_value);
	Serial.print("\tThrottle_value: ");
	Serial.println(throttle_value);
	myData.steering_value = wheels_value;
	myData.throttle_value = throttle_value;
	myData.turn_on_light= false;

	itoa(wheels_value, wheels_buffer, 10);
	display.drawString(0, 0, "Steering: ");
	display.drawString(40, 0, wheels_buffer);

	itoa(throttle_value, throttle_buffer, 10);
	display.drawString(0, 16, "throttle: ");
	display.drawString(40, 16, throttle_buffer);
	display.display();



	esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

	if (result == ESP_OK) {
		Serial.println("Sending confirmed");
	}
	else {
		Serial.println("Sending error");
	}
}
