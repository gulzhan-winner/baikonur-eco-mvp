#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "DHT.h"

// ҚҰПИЯ ДЕРЕКТЕР ОСЫНДА ЕМЕС, secrets.h ішінде
#include "secrets.h"

// ===== DHT11 =====
#define DHTPIN 15
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== LED (қаласаң қолдан) =====
#define LED_PIN 2

// ===== Telegram клиент =====
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// ===== Уақыт =====
unsigned long lastBotCheck = 0;
unsigned long lastAutoSend = 0;

const unsigned long BOT_DELAY = 1000;
const unsigned long AUTO_SEND_INTERVAL = 60000; // 1 минут

// ===== График үшін массив =====
#define HISTORY 10
float tempHistory[HISTORY] = {0};
float humHistory[HISTORY]  = {0};
int indexHistory = 0;

// ===== График генерация =====
String makeGraph(const float arr[], const String& label) {
  String g = label + "\n";
  for (int i = 0; i < HISTORY; i++) {
    g += String(i + 1) + ": ";
    float v = arr[i];
    if (v <= 0) { // әлі мән жоқ болса
      g += "—\n";
      continue;
    }
    int bars = (int)v;
    for (int j = 0; j < bars / 2; j++) g += "█";
    g += " " + String(v, 1) + "\n";
  }
  return g;
}

// ===== Бір мәрте оқу функциясы =====
bool readDHT(float &t, float &h) {
  t = dht.readTemperature();
  h = dht.readHumidity();
  return !(isnan(t) || isnan(h));
}

// ===== Telegram хабарламалары =====
void handleNewMessages(int n) {
  for (int i = 0; i < n; i++) {
    String text = bot.messages[i].text;
    String chat_id = bot.messages[i].chat_id;

    if (text == "/start") {
      String intro =
        "Сәлем! 🤖\n"
        "Бұл — ESP32 + DHT11 Telegram бот (MVP).\n\n"
        "Командалар:\n"
        "/status — ағымдағы көрсеткіштер\n"
        "/on — LED қосу\n"
        "/off — LED өшіру\n\n"
        "Автоматты есеп: әр 1 минут сайын жіберіледі.";
      bot.sendMessage(chat_id, intro, "");
    }

    if (text == "/status") {
      float t, h;
      if (!readDHT(t, h)) {
        bot.sendMessage(chat_id, "❌ DHT11 дерек бермеді", "");
        continue;
      }

      String msg = "📊 Ағымдағы мәндер:\n";
      msg += "🌡 Температура: " + String(t, 1) + " °C\n";
      msg += "💧 Ылғалдылық: " + String(h, 1) + " %\n";
      msg += "\n⏱ Автоматты режим: әр 1 минут";
      bot.sendMessage(chat_id, msg, "");
    }

    if (text == "/on") {
      digitalWrite(LED_PIN, HIGH);
      bot.sendMessage(chat_id, "💡 LED ҚОСЫЛДЫ", "");
    }

    if (text == "/off") {
      digitalWrite(LED_PIN, LOW);
      bot.sendMessage(chat_id, "❌ LED ӨШТІ", "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  client.setInsecure();
  dht.begin();

  Serial.print("WiFi қосылуда...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi қосылды ✅");

  bot.sendMessage(CHAT_ID, "✅ ESP32 + DHT11 іске қосылды\n⏱ Әр 1 минутта автоматты жіберу", "");
}

void loop() {
  // Telegram тексеру
  if (millis() - lastBotCheck > BOT_DELAY) {
    int n = bot.getUpdates(bot.last_message_received + 1);
    while (n) {
      handleNewMessages(n);
      n = bot.getUpdates(bot.last_message_received + 1);
    }
    lastBotCheck = millis();
  }

  // ===== Автоматты 1 минут =====
  if (millis() - lastAutoSend > AUTO_SEND_INTERVAL) {
    float t, h;
    if (readDHT(t, h)) {
      tempHistory[indexHistory] = t;
      humHistory[indexHistory]  = h;
      indexHistory = (indexHistory + 1) % HISTORY;

      String msg = "⏱ Автоматты есеп (1 минут)\n";
      msg += "🌡 Темп: " + String(t, 1) + " °C\n";
      msg += "💧 Ылғалдылық: " + String(h, 1) + " %\n\n";
      msg += makeGraph(tempHistory, "📈 Температура графигі");
      msg += "\n";
      msg += makeGraph(humHistory, "📉 Ылғалдылық графигі");

      bot.sendMessage(CHAT_ID, msg, "");
    } else {
      bot.sendMessage(CHAT_ID, "⚠️ Авто есеп: DHT11 дерек бермеді", "");
    }
    lastAutoSend = millis();
  }
}
