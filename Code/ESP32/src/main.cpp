#include <Arduino.h>
#include <FastLED.h>
#include "LiquidCrystal_I2C.h"
#include "Communication.h"
#include "Message.h"
#include "IDS.h"
#include "lidar.h"
#include "Logger.h"

#define LED_PIN     18
#define NUM_LEDS    41
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

LiquidCrystal_I2C lcd(0x27, 16, 2);

Communication comTeensy = Communication(&Serial);
Message teen_msg;

#define dt 100   // every single dt, the points are clustered to check wether there is an ennemy or not
#define MAX_POINTS int(4500 * dt / 1000)
Lidar lidar(&Serial2);
lidarFrame lidar_frame;
float angle_step;
uint16_t distance;
float angle;
unsigned int point_id;
typedef struct __attribute__((packed)){
    uint16_t distance;
    float angle;
} point;
point points_front_zone[MAX_POINTS];
point points_back_zone[MAX_POINTS];
point* cur_point_front;
point* cur_point_back;
const point* end_points_front = &points_front_zone[MAX_POINTS];
const point* end_points_back = &points_back_zone[MAX_POINTS];
unsigned long msstart;
unsigned int points_len;

void normalizeAngle(float* angle){
    *angle -= (2 * PI) * ((int)(*angle / (2 * PI)));
    if (*angle > PI)
        *angle -= 2 * PI;
    else if (*angle <= -PI)
        *angle += 2 * PI;
}

void setup() {
    Serial.begin(115200);
    Serial2.begin(230400);

    Logger::setup(&Serial, &Serial, &Serial, false, false, false);

    lcd.init();
    lcd.backlight();
    delay(100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   ENSMASTEEL   ");

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(255, 255, 255);
    }
    FastLED.show();

    msstart = millis();
    cur_point_front = points_front_zone;
    cur_point_back = points_back_zone;
}

void loop() {

    /* Teensy updates */
    comTeensy.update();

    /* Handle the Teensy messages */
    if (comTeensy.waitingRX()) {
        teen_msg = comTeensy.peekOldestMessage();
        switch (teen_msg.did) {
            case MessActuator:
                switch (teen_msg.aid){
                case SetTeamColorJaune:
                    lcd.setCursor(0, 0);
                    lcd.print("   ENSMASTEEL   ");
                    lcd.setCursor(0, 1);
                    lcd.print("   TEAM JAUNE   ");
                    break;
                case SetTeamColorBleu:
                    lcd.setCursor(0, 0);
                    lcd.print("   ENSMASTEEL   ");
                    lcd.setCursor(0, 1);
                    lcd.print("   TEAM BLEU    ");
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
        }
        comTeensy.popOldestMessage();
    }

    /* Lidar updates */
    lidar.update();

    /* Handle the lidar messages */
    if (lidar.waitingRX()) {
        lidar_frame = lidar.peekOldestFrame();

        angle_step = (float) ((lidar_frame.end_angle + 360000 - lidar_frame.start_angle) % 36000) / (FRAME_NPOINT - 1.0f);
        for (point_id = 0; point_id < FRAME_NPOINT; point_id++) {
            distance = lidar_frame.data[point_id].distance;
            if (distance > 150 && distance < 400 && lidar_frame.data[point_id].confidence > 225) {
                angle = ((float) lidar_frame.start_angle + angle_step * (float) point_id) * DEG_TO_RAD * 0.01f;
                normalizeAngle(&angle);
                if (angle < 0.0f && angle > -PI / 2.0f) {
                    // front zone
                    if (cur_point_front <= end_points_front) {
                        cur_point_front->distance = distance;
                        cur_point_front->angle = angle;
                        cur_point_front++;
                    } else {
                        Logger::debug("There is no enough place in the buffer points!");
                    }
                } else {
                    if (angle > PI / 2.0f) {
                        // back zone
                        if (cur_point_back <= end_points_back) {
                            cur_point_back->distance = distance;
                            cur_point_back->angle = angle;
                            cur_point_back++;
                        } else {
                            Logger::debug("There is no enough place in the buffer points!");
                        }
                    } else {
                        // sides zones
                    }
                }
            }
        }

        if ((millis() - msstart) > dt) {
            // let's analyse the points
            if (cur_point_front > points_front_zone) {
                distance = 0;
                angle = 0.0f;
                points_len = (unsigned int) (cur_point_front - points_front_zone);
                for (point_id = 0; point_id < points_len; point_id++) {
                    distance += points_front_zone[point_id].distance;
                    angle += points_front_zone[point_id].angle;
                    Logger::teleplot("> FRONT RAW xy :" + String(points_front_zone[point_id].distance / 1000.0f * std::cos(points_front_zone[point_id].angle),3) + ":" + String(points_front_zone[point_id].distance / 1000.0f * std::sin(points_front_zone[point_id].angle),3) + "|xy");
                }
                distance /= (uint16_t) points_len;  // mm
                angle /= (float) points_len * 0.001f; // millieme de rad
                Logger::teleplot("> FRONT CLUSTER xy :" + String(distance / 1000.0f * std::cos(angle / 1000.0f),3) + ":" + String(distance / 1000.0f * std::sin(angle / 1000.0f),3) + "|xy");
                comTeensy.send(newMessageLidar(ESP_32, Teensy, distance, (int16_t) angle));
                comTeensy.update();
                cur_point_front = points_front_zone;
            }
            if (cur_point_back > points_back_zone) {
                distance = 0;
                angle = 0.0f;
                points_len = (unsigned int) (cur_point_back - points_back_zone);
                for (point_id = 0; point_id < points_len; point_id++) {
                    distance += points_back_zone[point_id].distance;
                    angle += points_back_zone[point_id].angle;
                    Logger::teleplot("> BACK RAW xy :" + String(points_back_zone[point_id].distance / 1000.0f * std::cos(points_back_zone[point_id].angle),3) + ":" + String(points_back_zone[point_id].distance / 1000.0f * std::sin(points_back_zone[point_id].angle),3) + "|xy");
                }
                distance /= (uint16_t) points_len;  // mm
                angle /= (float) points_len * 0.001f; // millieme de rad
                Logger::teleplot("> BACK CLUSTER xy :" + String(distance / 1000.0f * std::cos(angle / 1000.0f),3) + ":" + String(distance / 1000.0f * std::sin(angle / 1000.0f),3) + "|xy");
                comTeensy.send(newMessageLidar(ESP_32, Teensy, distance, (int16_t) angle));
                comTeensy.update();
                cur_point_back = points_back_zone;
            }
            Logger::teleplot("> xy :0.5:0.5|xy");
            Logger::teleplot("> xy :-0.5:-0.5|xy");

            msstart = millis();
        }

        lidar.popOldestFrame();
    }
}
