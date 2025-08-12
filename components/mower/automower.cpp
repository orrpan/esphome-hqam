#include "automower.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace mower
    {

        Automower::Automower(uart::UARTComponent *parent, uint32_t update_interval)
            : PollingComponent(update_interval), uart::UARTDevice(parent)
        {

            battery_temperature_sensor_ = new template_::TemplateSensor();
            battery_level_sensor_ = new template_::TemplateSensor();
            battery_used_sensor_ = new template_::TemplateSensor();
            battery_voltage_sensor_ = new template_::TemplateSensor();
            blade_motor_speed_sensor_ = new template_::TemplateSensor();
            charging_time_sensor_ = new template_::TemplateSensor();
            firmware_version_sensor_ = new template_::TemplateSensor();

            last_code_received_text_sensor_ = new template_::TemplateTextSensor();
            mode_text_sensor_ = new template_::TemplateTextSensor();
            status_text_sensor_ = new template_::TemplateTextSensor();
        }

        void Automower::set_battery_temperature_sensor(template_::TemplateSensor *s) { battery_temperature_sensor_ = s; }
        void Automower::set_battery_level_sensor(template_::TemplateSensor *s) { battery_level_sensor_ = s; }
        void Automower::set_battery_used_sensor(template_::TemplateSensor *s) { battery_used_sensor_ = s; }
        void Automower::set_battery_voltage_sensor(template_::TemplateSensor *s) { battery_voltage_sensor_ = s; }
        void Automower::set_blade_motor_speed_sensor(template_::TemplateSensor *s) { blade_motor_speed_sensor_ = s; }
        void Automower::set_charging_time_sensor(template_::TemplateSensor *s) { charging_time_sensor_ = s; }
        void Automower::set_firmware_version_sensor(template_::TemplateSensor *s) { firmware_version_sensor_ = s; }

        void Automower::set_last_code_received_text_sensor(template_::TemplateTextSensor *s) { last_code_received_text_sensor_ = s; }
        void Automower::set_mode_text_sensor(template_::TemplateTextSensor *s) { mode_text_sensor_ = s; }
        void Automower::set_status_text_sensor(template_::TemplateTextSensor *s) { status_text_sensor_ = s; }

        template_::TemplateSensor *Automower::get_battery_temperature_sensor() const { return battery_temperature_sensor_; }
        template_::TemplateSensor *Automower::get_blade_motor_speed_sensor() const
        {
            return blade_motor_speed_sensor_;
        }
        template_::TemplateSensor *Automower::get_battery_level_sensor() const { return battery_level_sensor_; }
        template_::TemplateSensor *Automower::get_battery_used_sensor() const { return battery_used_sensor_; }
        template_::TemplateSensor *Automower::get_battery_voltage_sensor() const { return battery_voltage_sensor_; }
        template_::TemplateSensor *Automower::get_charging_time_sensor() const { return charging_time_sensor_; }
        template_::TemplateSensor *Automower::get_firmware_version_sensor() const { return firmware_version_sensor_; }

        template_::TemplateTextSensor *Automower::get_last_code_received_text_sensor() const { return last_code_received_text_sensor_; }
        template_::TemplateTextSensor *Automower::get_mode_text_sensor() const { return mode_text_sensor_; }
        template_::TemplateTextSensor *Automower::get_status_text_sensor() const { return status_text_sensor_; }

        void Automower::setup() {}

        void Automower::update() { sendCommands(0); }

        void Automower::loop() { checkUartRead(); }

        void Automower::set_mode(const std::string &value)
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

        void Automower::set_stop(bool stop)
        {
            write_array(stop ? STOP_ON_DATA : STOP_OFF_DATA, 5);
        }

        void Automower::set_left_motor(int value)
        {
            uint8_t data[5] = {0x0F, 0x92, 0x23, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
            write_array(data, 5);
        }

        void Automower::set_right_motor(int value)
        {
            uint8_t data[5] = {0x0F, 0x92, 0x03, static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>(value & 0xFF)};
            write_array(data, 5);
        }

        void Automower::key_back() { write_array(KEY_BACK, 5); }
        void Automower::key_yes() { write_array(KEY_YES, 5); }
        void Automower::key_num(uint8_t num)
        {
            uint8_t data[5] = {0x0F, 0x80, 0x5F, 0x00, num};
            write_array(data, 5);
        }

        void Automower::sendCommands(int index)
        {
            if (index < (int)pollingCommandList.size())
            {
                set_retry(
                    5, 3, [this, index](uint8_t attempt) -> RetryResult
                    {
                        if (!_writable) return RetryResult::RETRY;
                        auto it = pollingCommandList.begin();
                        ESP_LOGD("Automower", "UART TX: %02X %02X %02X %02X %02X", (*it)[0], (*it)[1], (*it)[2], (*it)[3], (*it)[4]);
                        std::advance(it, index);
                        write_array(*it, 5);
                        _writable = false;
                        sendCommands(index + 1);
                        return RetryResult::DONE; },
                    2.0f);
            }
        }

        void Automower::checkUartRead()
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
                case 0x01EC:
                    if (charging_time_sensor_)
                        charging_time_sensor_->publish_state(val);
                    break;
                case 0x01EF:
                    if (battery_level_sensor_)
                        battery_level_sensor_->publish_state(val);
                    break;
                case 0x0233:
                    if (battery_temperature_sensor_)
                        battery_temperature_sensor_->publish_state(val);
                    break;
                case 0x2EE0:
                    if (battery_used_sensor_)
                        battery_used_sensor_->publish_state(val);
                    break;
                case 0x2EEA:
                    if (blade_motor_speed_sensor_)
                        blade_motor_speed_sensor_->publish_state(val);
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

        void Automower::setStopStatusFromCode(uint16_t val)
        {
            stopStatus = (val == 0x0002);
        }

        void Automower::publishMode(uint16_t val)
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
            case 0x0002:
                mode = "Charging then AUTO";
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

        void Automower::publishStatus(uint16_t val)
        {
            if (!status_text_sensor_)
                return;
            if (val == 0x0410) // Collission or dodge
                return;
            std::string s;
            switch (val)
            {
            case 0x0010:
                s = "Outside working area";
                break;
            case 0x0012:
                s = "LBV Low battery voltage";
                break;
            case 0x03EA:
                s = "MIP Mowing in progress";
                break;
            case 0x0006:
                s = "Left wheel motor blocked";
                break;
            case 0x0008:
                s = "Right wheel motor blocked";
                break;
            case 0x000C:
                s = "No loop signal";
                break;
            case 0x001A:
                s = "Station blocked";
                break;
            case 0x0022:
                s = "Mower lifted";
                break;
            case 0x0034:
                s = "Station no contact";
                break;
            case 0x0036:
                s = "Pin expired";
                break;
            case 0x03E8:
                s = "Leaving station";
                break;
            case 0x03EE:
                s = "Start mowing";
                break;
            case 0x03F0:
                s = "Mowing started";
                break;
            case 0x03F4:
                s = "Start mowing2";
                break;
            case 0x03F6:
                s = "Charging";
                break;
            case 0x03F8:
                s = "Waiting timer2";
                break;
            case 0x1016:
                s = "Waiting timer";
                break;
            case 0x0400:
                s = "Parking in station";
                break;
            case 0x040C:
                s = "Square mode";
                break;
            case 0x040E:
                s = "Stuck";
                break;
            case 0x0412:
                s = "Searching";
                break;
            case 0x0414:
                s = "Stop";
                break;
            case 0x0418:
                s = "Docking";
                break;
            case 0x041A:
                s = "Leaving station";
                break;
            case 0x041C:
                s = "Error";
                break;
            case 0x0420:
                s = "Waiting for use";
                break;
            case 0x0422:
                s = "Follow boundary";
                break;
            case 0x0424:
                s = "Found N-Signal";
                break;
            case 0x0426:
                s = "Stuck";
                break;
            case 0x0428:
                s = "Searching";
                break;
            case 0x042E:
                s = "Follow guide line";
                break;
            case 0x0430:
                s = "Follow loop wire";
                break;
            default:
                s = "STATUS_" + formatHex(val);
                break;
            }
            status_text_sensor_->publish_state(s);
        }

        std::string Automower::formatHex(uint16_t v)
        {
            char s[16];
            sprintf(s, "%04x", v);
            return std::string(s);
        }

    } // namespace automower
} // namespace esphome