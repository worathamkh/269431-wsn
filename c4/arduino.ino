#include <XBee.h>

int SENSOR = A11;
int LED = 50;

int light = true;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();

void writeXBee(String data) {
    uint8_t data_int[data.length()];
    for (int i = 0; i < data.length(); i++) {
        data_int[i] = (uint8_t)data[i];
    }
    XBeeAddress64 addr64 = XBeeAddress64();
    addr64.setMsb(0x0013A200);
    addr64.setLsb(0x40C91987);
    ZBTxRequest zbTx = ZBTxRequest(addr64, data_int, sizeof(data_int));
    xbee.send(zbTx);
}

String readXBee() {
    xbee.readPacket();
    if (xbee.getResponse().isAvailable()) {
        String sample;
        if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
            xbee.getResponse().getZBRxResponse(rx);
            for (int i = 0; i < rx.getDataLength(); i++) {
                sample += (char)rx.getData(i);
            }
        }
        return sample;
    }
    return "";
    // else if (xbee.getResponse().isError()) {
    //     return (String)xbee.getResponse().getErrorCode();
    // } else {
    //     return "";
    // }
}

int readLightSensor() {
    return 1024 - analogRead(SENSOR);
}

void setup() {
    // put your setup code here, to run once:
    pinMode(SENSOR, INPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
    //xbee_serial.begin(9600);
    xbee.setSerial(Serial);
}

void loop() {
    // put your main code here, to run repeatedly:
    int brightness = readLightSensor();
    writeXBee(String(brightness));
    // if (light) {
    //     digitalWrite(LED, HIGH);
    //     light = false;
    // } else {
    //     digitalWrite(LED, LOW);
    //     light = true;
    // }
    /* String message = readXBee(); */
    /* String message = ""; */
    char x;
    if (Serial.available() >= 19) {
        if (Serial.read() == 0x7E) {
            for (int i = 0; i < 17; i++) {
                byte discard = Serial.read();
            }
            x = (char) Serial.read();
        }
    }
    if (x == 'Y') {
        digitalWrite(LED, HIGH);
    } else if (x == 'N') {
        digitalWrite(LED, LOW);
    }
    delay(500);
}
