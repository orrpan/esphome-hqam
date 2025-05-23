#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/template/sensor/template_sensor.h"
#include "esphome/components/template/text_sensor/template_text_sensor.h"

#include <list>
#include <string>

namespace esphome
{
  namespace mower
  {

    class Automower : public PollingComponent, public uart::UARTDevice
    {
    public:
      Automower(uart::UARTComponent *parent, uint32_t update_interval);

      void set_battery_level_sensor(template_::TemplateSensor *s);
      void set_battery_used_sensor(template_::TemplateSensor *s);
      void set_charging_time_sensor(template_::TemplateSensor *s);
      void set_battery_voltage_sensor(template_::TemplateSensor *s);
      void set_firmware_version_sensor(template_::TemplateSensor *s);
      void set_mode_text_sensor(template_::TemplateTextSensor *s);
      void set_status_text_sensor(template_::TemplateTextSensor *s);
      void set_last_code_received_text_sensor(template_::TemplateTextSensor *s);

      template_::TemplateSensor *get_battery_level_sensor() const;
      template_::TemplateSensor *get_battery_used_sensor() const;
      template_::TemplateSensor *get_charging_time_sensor() const;
      template_::TemplateSensor *get_battery_voltage_sensor() const;
      template_::TemplateSensor *get_firmware_version_sensor() const;
      template_::TemplateTextSensor *get_mode_text_sensor() const;
      template_::TemplateTextSensor *get_status_text_sensor() const;
      template_::TemplateTextSensor *get_last_code_received_text_sensor() const;

      void setup() override;
      void update() override;
      void loop() override;

      void set_mode(const std::string &value);
      void set_stop(bool stop);
      void set_left_motor(int value);
      void set_right_motor(int value);
      void key_back();
      void key_yes();
      void key_num(uint8_t num);

    protected:
      bool _writable = true;
      bool stopStatus = false;

      template_::TemplateSensor *battery_level_sensor_ = nullptr;
      template_::TemplateSensor *battery_used_sensor_ = nullptr;
      template_::TemplateSensor *charging_time_sensor_ = nullptr;
      template_::TemplateSensor *battery_voltage_sensor_ = nullptr;
      template_::TemplateSensor *firmware_version_sensor_ = nullptr;
      template_::TemplateTextSensor *mode_text_sensor_ = nullptr;
      template_::TemplateTextSensor *status_text_sensor_ = nullptr;
      template_::TemplateTextSensor *last_code_received_text_sensor_ = nullptr;

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

      void sendCommands(int index);
      void checkUartRead();
      void setStopStatusFromCode(uint16_t val);
      void publishMode(uint16_t val);
      void publishStatus(uint16_t val);
      std::string formatHex(uint16_t v);
    };

  } // namespace mower
} // namespace esphome
