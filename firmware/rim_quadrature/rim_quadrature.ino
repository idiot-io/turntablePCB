// ESP32-S3 rim quadrature bring-up. Inputs are 3.3 V comparator outputs,
// not the charge/discharge nodes used by the absolute sensor array.
#include <Arduino.h>
#include <driver/pulse_cnt.h>
#include <esp_err.h>

constexpr int pinA = 11;
constexpr int pinB = 12;
constexpr int countsPerTurn = 512;
constexpr int counterLimit = 30000;
constexpr int reportIntervalMs = 10;
pcnt_unit_handle_t counter;

void addChannel(int edgePin, int levelPin, bool channelA)
{
  pcnt_chan_config_t config = {};
  config.edge_gpio_num = edgePin;
  config.level_gpio_num = levelPin;
  pcnt_channel_handle_t channel;
  ESP_ERROR_CHECK(pcnt_new_channel(counter, &config, &channel));
  // With A and B both low, a rising A (A leads B) is positive.
  ESP_ERROR_CHECK(pcnt_channel_set_edge_action(
      channel, channelA ? PCNT_CHANNEL_EDGE_ACTION_INCREASE : PCNT_CHANNEL_EDGE_ACTION_DECREASE,
      channelA ? PCNT_CHANNEL_EDGE_ACTION_DECREASE : PCNT_CHANNEL_EDGE_ACTION_INCREASE));
  ESP_ERROR_CHECK(pcnt_channel_set_level_action(
      channel, PCNT_CHANNEL_LEVEL_ACTION_INVERSE, PCNT_CHANNEL_LEVEL_ACTION_KEEP));
}

void setup()
{
  Serial.begin(115200);
  pcnt_unit_config_t config = {};
  config.high_limit = counterLimit;
  config.low_limit = -counterLimit;
  config.flags.accum_count = 1;
  ESP_ERROR_CHECK(pcnt_new_unit(&config, &counter));
  addChannel(pinA, pinB, true);
  addChannel(pinB, pinA, false);
  ESP_ERROR_CHECK(pcnt_unit_add_watch_point(counter, config.high_limit));
  ESP_ERROR_CHECK(pcnt_unit_add_watch_point(counter, config.low_limit));
  ESP_ERROR_CHECK(pcnt_unit_enable(counter));
  ESP_ERROR_CHECK(pcnt_unit_clear_count(counter));
  ESP_ERROR_CHECK(pcnt_unit_start(counter));
}

void loop()
{
  static uint32_t lastReport = millis();
  static int lastCount = 0;
  uint32_t now = millis();
  uint32_t elapsed = now - lastReport;
  if (elapsed < reportIntervalMs)
    return;

  int count;
  ESP_ERROR_CHECK(pcnt_unit_get_count(counter, &count));
  int delta = count - lastCount;
  int wrapped = (count % countsPerTurn + countsPerTurn) % countsPerTurn;
  double angle = 360.0 * wrapped / countsPerTurn;
  double speed = 60000.0 * delta / (countsPerTurn * elapsed);
  Serial.printf("%d,%.3f,%.2f\n", count, angle, speed);
  lastCount = count;
  lastReport = now;
}
