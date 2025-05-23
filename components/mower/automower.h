// automower.h (inside mower namespace)
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
// #include "esphome/components/sensor/sensor.h"
// #include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/template/sensor/template_sensor.h"
#include "esphome/components/template/text_sensor/template_text_sensor.h"

#include <list>

namespace esphome
{
  namespace mower
  {

    class Automower : public PollingComponent, public uart::UARTDevice
    {
    public:
      Automower(uart::UARTComponent *parent, uint32_t update_interval)
          : PollingComponent(update_interval), uart::UARTDevice(parent) {}

      void set_battery_level_sensor(template_::TemplateSensor *s) { battery_level_sensor_ = s; }
      void set_battery_used_sensor(template_::TemplateSensor *s) { battery_used_sensor_ = s; }
      void set_charging_time_sensor(template_::TemplateSensor *s) { charging_time_sensor_ = s; }
      void set_battery_voltage_sensor(template_::TemplateSensor *s) { battery_voltage_sensor_ = s; }
      void set_firmware_version_sensor(template_::TemplateSensor *s) { firmware_version_sensor_ = s; }
      void set_mode_text_sensor(template_::TemplateTextSensor *s) { mode_text_sensor_ = s; }
      void set_status_text_sensor(template_::TemplateTextSensor *s) { status_text_sensor_ = s; }
      void set_last_code_received_text_sensor(template_::TemplateTextSensor *s) { last_code_received_text_sensor_ = s; }

      template_::TemplateSensor *get_battery_level_sensor() const { return battery_level_sensor_; }
      template_::TemplateSensor *get_battery_used_sensor() const { return battery_used_sensor_; }
      template_::TemplateSensor *get_charging_time_sensor() const { return charging_time_sensor_; }
      template_::TemplateSensor *get_battery_voltage_sensor() const { return battery_voltage_sensor_; }
      template_::TemplateSensor *get_firmware_version_sensor() const { return firmware_version_sensor_; }
      template_::TemplateTextSensor *get_mode_text_sensor() const { return mode_text_sensor_; }
      template_::TemplateTextSensor *get_status_text_sensor() const { return status_text_sensor_; }
      template_::TemplateTextSensor *get_last_code_received_text_sensor() const { return last_code_received_text_sensor_; }

      void setup() override {}
      void update() override { sendCommands(0); }
      void loop() override { checkUartRead(); }

      void set_mode(const std::string &value)
      {
        if (value == "MAN")
        {
          write_array(MAN_DATA, sizeof(MAN_DATA));
        }
        else if (value == "AUTO")
        {
          write_array(AUTO_DATA, sizeof(AUTO_DATA));
        }
        else if (value == "HOME")
        {
          write_array(HOME_DATA, sizeof(HOME_DATA));
        }
        else if (value == "DEMO")
        {
          write_array(DEMO_DATA, sizeof(DEMO_DATA));
        }
        else
        {
          ESP_LOGE("Automower", "Unknown mode: %s", value.c_str());
        }
      }

      void set_stop(bool stop)
      {
        write_array(stop ? STOP_ON_DATA : STOP_OFF_DATA, 5);
      }

      void set_left_motor(int value)
      {
        uint8_t data[5] = {0x0F, 0x92, 0x23, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
        write_array(data, 5);
      }

      void set_right_motor(int value)
      {
        uint8_t data[5] = {0x0F, 0x92, 0x03, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
        write_array(data, 5);
      }

      void key_back() { write_array(KEY_BACK, 5); }
      void key_yes() { write_array(KEY_YES, 5); }
      void key_num(uint8_t num)
      {
        uint8_t data[5] = {0x0F, 0x80, 0x5F, 0x00, num};
        write_array(data, 5);
      }

    protected:
      bool _writable = true;
      bool stopStatus = false;

      template_::TemplateSensor *battery_level_sensor_ = new template_::TemplateSensor();
      template_::TemplateSensor *battery_used_sensor_ = new template_::TemplateSensor();
      template_::TemplateSensor *charging_time_sensor_ = new template_::TemplateSensor();
      template_::TemplateSensor *battery_voltage_sensor_ = new template_::TemplateSensor();
      template_::TemplateSensor *firmware_version_sensor_ = new template_::TemplateSensor();
      template_::TemplateTextSensor *mode_text_sensor_ = new template_::TemplateTextSensor();
      template_::TemplateTextSensor *status_text_sensor_ = new template_::TemplateTextSensor();
      template_::TemplateTextSensor *last_code_received_text_sensor_ = new template_::TemplateTextSensor();

      static constexpr uint8_t MAN_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x00};
      static constexpr uint8_t AUTO_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x01};
      static constexpr uint8_t HOME_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x03};
      static constexpr uint8_t DEMO_DATA[5] = {0x0F, 0x81, 0x2C, 0x00, 0x04};
      static constexpr uint8_t STOP_ON_DATA[5] = {0x0F, 0x81, 0x2F, 0x00, 0x02};
      static constexpr uint8_t STOP_OFF_DATA[5] = {0x0F, 0x81, 0x2F, 0x00, 0x00};
      static constexpr uint8_t KEY_BACK[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0F};
      static constexpr uint8_t KEY_YES[5] = {0x0F, 0x80, 0x5F, 0x00, 0x12};
      static constexpr uint8_t getModeCmd[5] = {0x0F, 0x01, 0x2C, 0x00, 0x00};
      static constexpr uint8_t getStatusCode[5] = {0x0F, 0x01, 0xF1, 0x00, 0x00};
      static constexpr uint8_t getChargingTime[5] = {0x0F, 0x01, 0xEC, 0x00, 0x00};
      static constexpr uint8_t getBatteryCurrent[5] = {0x0F, 0x01, 0xEB, 0x00, 0x00};
      static constexpr uint8_t getBatteryLevel[5] = {0x0F, 0x01, 0xEF, 0x00, 0x00};
      static constexpr uint8_t getBatteryUsed[5] = {0x0F, 0x2E, 0xE0, 0x00, 0x00};
      static constexpr uint8_t getBatteryVoltage[5] = {0x0F, 0x2E, 0xF4, 0x00, 0x00};
      static constexpr uint8_t getFirmwareVersion[5] = {0x0F, 0x33, 0x90, 0x00, 0x00};
      static constexpr uint8_t READ_STOP_CMD[5] = {0x0F, 0x01, 0x2F, 0x00, 0x00};

      const std::list<const uint8_t *> pollingCommandList = {
          getModeCmd, getStatusCode, getBatteryLevel,
          getChargingTime, getBatteryUsed, getBatteryVoltage,
          getFirmwareVersion, READ_STOP_CMD};

      void sendCommands(int index)
      {
        if (index < (int)pollingCommandList.size())
        {
          set_retry(
              5, 3, [this, index](uint8_t attempt) -> RetryResult
              {
        if (!_writable) return RetryResult::RETRY;
        auto it = pollingCommandList.begin();
        std::advance(it, index);
        write_array(*it, 5);
        _writable = false;
        sendCommands(index + 1);
        return RetryResult::DONE; },
              2.0f);
        }
      }

      void checkUartRead()
      {
        while (available() > 0 && peek() != 0x0F)
          read();
        while (available() >= 5 && peek() == 0x0F)
        {
          uint8_t readData[5];
          read_array(readData, 5);
          _writable = true;

          ESP_LOGD("Automower", "UART RX: %02X %02X %02X %02X %02X", readData[0], readData[1], readData[2], readData[3], readData[4]);

          uint16_t addr = ((readData[1] & 0x7F) << 8) | readData[2];
          uint16_t val = (readData[4] << 8) | readData[3];

          ESP_LOGD("Automower", "Decoded: addr=0x%04X val=0x%04X", addr, val);

          switch (addr)
          {
          case 0x012C:
            publishMode(val);
            break;
          case 0x01F1:
            publishStatus(val);
            break;
          case 0x01EF:
            if (battery_level_sensor_)
              battery_level_sensor_->publish_state(val);
            break;
          case 0x01EC:
            if (charging_time_sensor_)
              charging_time_sensor_->publish_state(val);
            break;
          case 0x2EE0:
            if (battery_used_sensor_)
              battery_used_sensor_->publish_state(val);
            break;
          case 0x2EF4:
            if (battery_voltage_sensor_)
              battery_voltage_sensor_->publish_state(val / 1000.0f);
            break;
          case 0x3390:
            if (firmware_version_sensor_)
              firmware_version_sensor_->publish_state(val);
            break;
          case 0x012F:
            setStopStatusFromCode(val);
            break;
          default:
            ESP_LOGW("Automower", "Unhandled address: 0x%04X with value 0x%04X", addr, val);
            break;
          }
        }
      }

      void setStopStatusFromCode(uint16_t val)
      {
        stopStatus = (val == 0x0002);
      }

      void publishMode(uint16_t val)
      {
        if (!mode_text_sensor_)
          return;
        std::string mode;
        switch (val)
        {
        case 0x0000:
          mode = "MAN";
          break;
        case 0x0001:
          mode = "AUTO";
          break;
        case 0x0003:
          mode = "HOME";
          break;
        case 0x0004:
          mode = "DEMO";
          break;
        default:
          mode = "MODE_" + formatHex(val);
          break;
        }
        mode_text_sensor_->publish_state(mode);
      }

      void publishStatus(uint16_t val)
      {
        if (!status_text_sensor_)
          return;
        std::string s;
        switch (val)
        {
        case 0x0012:
          s = "LBV Low battery voltage";
          break;
        case 0x03EA:
          s = "MIP Mowing in progress";
          break;
        default:
          s = "STATUS_" + formatHex(val);
          break;
        }
        status_text_sensor_->publish_state(s);
      }

      std::string formatHex(uint16_t v)
      {
        char s[16];
        sprintf(s, "%04x", v);
        return std::string(s);
      }
    };

  } // namespace mower
} // namespace esphome
