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

      void update_battery_capacity_mAh();

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

      // static constexpr uint8_t getMealTime[5] = {0x0F, 0x00, 0x38, 0x00, 0x00};
      // static constexpr uint8_t getSquareModePercent[5] = {0x0F, 0x01, 0x34, 0x00, 0x00};
      // static constexpr uint8_t getSquareModeStatus[5] = {0x0F, 0x01, 0x37, 0x00, 0x00};
      // static constexpr uint8_t getSquareModeStatus[5] = {0x0F, 0x01, 0x38, 0x00, 0x00};
      static constexpr uint8_t getBatteryCapacity_mAh[5] = {0x0F, 0x01, 0xEB, 0x00, 0x00};
      // static constexpr uint8_t getBatteryChargingTimeMinute[5] = {0x0F, 0x01, 0xEC, 0x00, 0x00};
      // static constexpr uint8_t getBatteryCapacity_mAh[5] = {0x0F, 0x01, 0xEF, 0x00, 0x00};
      // static constexpr uint8_t getBatteryCapacitySearchStart_mAh[5] = {0x0F, 0x01, 0xEF, 0x00, 0x00};
      // static constexpr uint8_t getStatus[5] = {0x0F, 0x01, 0xF1, 0x00, 0x00};
      // static constexpr uint8_t getBatteryTemperatureCurrent[5] = {0x0F, 0x02, 0x33, 0x00, 0x00};
      // static constexpr uint8_t getBatteryLastChargeMinute[5] = {0x0F, 0x02, 0x35, 0x00, 0x00};
      // static constexpr uint8_t getBatteryTemperatureCharged[5] = {0x0F, 0x02, 0x35, 0x00, 0x00};
      // static constexpr uint8_t getBatteryNextTemperatureMeasurementSecond[5] = {0x0F, 0x02, 0x36, 0x00, 0x00};
      // static constexpr uint8_t getSpeedRight[5] = {0x0F, 0x24, 0xBF, 0x00, 0x00};
      // static constexpr uint8_t getSpeedLeft[5] = {0x0F, 0x24, 0xC0, 0x00, 0x00};
      // static constexpr uint8_t getBatteryCapacityUsed_mAh[5] = {0x0F, 0x2E, 0xE0, 0x00, 0x00};
      // static constexpr uint8_t getBladeMotorSpeed[5] = {0x0F, 0x2E, 0xEA, 0x00, 0x00};
      // static constexpr uint8_t getBatteryVoltage_mV[5] = {0x0F, 0x2E, 0xF4, 0x00, 0x00};
      // static constexpr uint8_t getFirmwareVersion[5] = {0x0F, 0x33, 0x90, 0x00, 0x00};
      // static constexpr uint8_t getSecond[5] = {0x0F, 0x36, 0xB1, 0x00, 0x00};
      // static constexpr uint8_t getMinute[5] = {0x0F, 0x36, 0xB3, 0x00, 0x00};
      // static constexpr uint8_t getHour[5] = {0x0F, 0x36, 0xB5, 0x00, 0x00};
      // static constexpr uint8_t getDay[5] = {0x0F, 0x36, 0xB7, 0x00, 0x00};
      // static constexpr uint8_t getMonth[5] = {0x0F, 0x36, 0xB9, 0x00, 0x00};
      // static constexpr uint8_t getYear[5] = {0x0F, 0x36, 0xBD, 0x00, 0x00};
      // static constexpr uint8_t getLanguageFileVersion[5] = {0x0F, 0x3A, 0xC0, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StartHour[5] = {0x0F, 0x4A, 0x38, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StartMinute[5] = {0x0F, 0x4A, 0x39, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StopHour[5] = {0x0F, 0x4A, 0x3A, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer1StopMinute[5] = {0x0F, 0x4A, 0x3B, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StartHour[5] = {0x0F, 0x4A, 0x3C, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StartMinute[5] = {0x0F, 0x4A, 0x3D, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StopHour[5] = {0x0F, 0x4A, 0x3E, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer1StopMinute[5] = {0x0F, 0x4A, 0x3F, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StartHour[5] = {0x0F, 0x4A, 0x40, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StartMinute[5] = {0x0F, 0x4A, 0x41, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StopHour[5] = {0x0F, 0x4A, 0x42, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimer2StopMinute[5] = {0x0F, 0x4A, 0x43, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StartHour[5] = {0x0F, 0x4A, 0x44, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StartMinute[5] = {0x0F, 0x4A, 0x45, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StopHour[5] = {0x0F, 0x4A, 0x46, 0x00, 0x00};
      // static constexpr uint8_t getWeekEndTimer2StopMinute[5] = {0x0F, 0x4A, 0x47, 0x00, 0x00};
      // static constexpr uint8_t getTimerStatus[5] = {0x0F, 0x4A, 0x4E, 0x00, 0x00};
      // static constexpr uint8_t getTimerDays[5] = {0x0F, 0x4A, 0x50, 0x00, 0x00};
      // static constexpr uint8_t getWeekTimerDays[5] = {0x0F, 0x4A, 0x50, 0x00, 0x00};

      // static constexpr uint8_t setKey0[5] = {0x0F, 0x80, 0x5F, 0x00, 0x00};
      // static constexpr uint8_t setKey1[5] = {0x0F, 0x80, 0x5F, 0x00, 0x01};
      // static constexpr uint8_t setKey2[5] = {0x0F, 0x80, 0x5F, 0x00, 0x02};
      // static constexpr uint8_t setKey3[5] = {0x0F, 0x80, 0x5F, 0x00, 0x03};
      // static constexpr uint8_t setKey4[5] = {0x0F, 0x80, 0x5F, 0x00, 0x04};
      // static constexpr uint8_t setKey5[5] = {0x0F, 0x80, 0x5F, 0x00, 0x05};
      // static constexpr uint8_t setKey6[5] = {0x0F, 0x80, 0x5F, 0x00, 0x06};
      // static constexpr uint8_t setKey7[5] = {0x0F, 0x80, 0x5F, 0x00, 0x07};
      // static constexpr uint8_t setKey8[5] = {0x0F, 0x80, 0x5F, 0x00, 0x08};
      // static constexpr uint8_t setKey9[5] = {0x0F, 0x80, 0x5F, 0x00, 0x09};
      // static constexpr uint8_t setPRG_A[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0A};
      // static constexpr uint8_t setPRG_B[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0B};
      // static constexpr uint8_t setPRG_C[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0C};
      // static constexpr uint8_t setKeyHOME[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0D};
      // static constexpr uint8_t setKeyMANAUTO[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0E};
      // static constexpr uint8_t setKeyC[5] = {0x0F, 0x80, 0x5F, 0x00, 0x0F};
      // static constexpr uint8_t setKeyUP[5] = {0x0F, 0x80, 0x5F, 0x00, 0x10};
      // static constexpr uint8_t setKeyDOWN[5] = {0x0F, 0x80, 0x5F, 0x00, 0x11};
      // static constexpr uint8_t setKeyYES[5] = {0x0F, 0x80, 0x5F, 0x00, 0x12};
      // static constexpr uint8_t setModeMAN[5] = {0x0F, 0x81, 0x2C, 0x00, 0x00};
      // static constexpr uint8_t setModeAUTO[5] = {0x0F, 0x81, 0x2C, 0x00, 0x01};
      // static constexpr uint8_t setModeHOME[5] = {0x0F, 0x81, 0x2C, 0x00, 0x03};
      // static constexpr uint8_t setModeDEMO[5] = {0x0F, 0x81, 0x2C, 0x00, 0x04};
      // static constexpr uint8_t setTimerActive[5] = {0x0F, 0xCA, 0x4E, 0x00, 0x00};
      // static constexpr uint8_t setTimerInactive[5] = { 0x0F,
      //                                                  0xCA,
      //                                                  0x4E,
      //                                                  0x00,
      //                                                  0x01 }

      const std::list<const uint8_t *>
          pollingCommandList = {
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
