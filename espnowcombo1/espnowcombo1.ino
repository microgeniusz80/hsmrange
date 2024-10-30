#include <ESP8266WiFi.h>
#include <espnow.h>

// REPLACE WITH RECEIVER MAC Address
//uint8_t broadcastAddress1[] = {0x10, 0x52, 0x1C, 0xEA, 0xB3, 0xE4};
uint8_t broadcastAddress2[] = {0x50, 0x02, 0x91, 0xEA, 0xD2, 0x77};
//uint8_t broadcastAddress3[] = {0x10, 0x52, 0x1C, 0xE9, 0x89, 0x58};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  String d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long lastTime = 0;  
unsigned long timerDelay = 5000;  // send readings timer

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.println(myData.d);
  Serial.println();
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  
  // Register peer
  //esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  //esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
}
 
void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Set values to send
    myData.d = "Hello";

    // Send message via ESP-NOW
    //esp_now_send(broadcastAddress1, (uint8_t *) &myData, sizeof(myData));
    esp_now_send(broadcastAddress2, (uint8_t *) &myData, sizeof(myData));
    //esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));

    lastTime = millis();
  }
}
