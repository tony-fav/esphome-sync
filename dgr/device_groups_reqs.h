#define USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
  #define DEVICE_GROUPS_ADDRESS 239,255,250,250  // Device groups multicast address
  #define DEVICE_GROUPS_PORT 4447                // Device groups multicast port
  #define DEVICE_GROUPS_DEBUG
const uint8_t MAX_RELAYS = 8;
#define TEST_DEVICE_GROUP_NAME "BedroomHallwayLights"
uint32_t device_group_share_in  = 0xFFFFFFFF;     
uint32_t device_group_share_out = 0x00000000;

bool ehdgr_power_1 = false;
bool ehdgr_power_2 = false;
bool ehdgr_power_3 = false;
bool ehdgr_power_4 = false;
bool ehdgr_power_5 = false;
bool ehdgr_power_6 = false;
bool ehdgr_power_7 = false;
bool ehdgr_power_8 = false;
uint8_t ehdgr_brightness = 0;
uint8_t ehdgr_channel_1 = 0;
uint8_t ehdgr_channel_2 = 0;
uint8_t ehdgr_channel_3 = 0;
uint8_t ehdgr_channel_4 = 0;
uint8_t ehdgr_channel_5 = 0;

// from tasmota.h
typedef unsigned long power_t;              // Power (Relay) type
const uint16_t TOPSZ = 151;                 // Max number of characters in topic string
const uint32_t POWER_MASK = 0xffffffffUL;   // Power (Relay) full mask

enum ExecuteCommandPowerOptions { POWER_OFF, POWER_ON, POWER_TOGGLE, POWER_BLINK, POWER_BLINK_STOP,
                                  POWER_OFF_NO_STATE = 8, POWER_ON_NO_STATE, POWER_TOGGLE_NO_STATE,
                                  POWER_SHOW_STATE = 16 };

enum DevGroupMessageType { DGR_MSGTYP_FULL_STATUS, DGR_MSGTYP_PARTIAL_UPDATE, DGR_MSGTYP_UPDATE, DGR_MSGTYP_UPDATE_MORE_TO_COME, DGR_MSGTYP_UPDATE_DIRECT, DGR_MSGTYPE_UPDATE_COMMAND, DGR_MSGTYPFLAG_WITH_LOCAL = 128 };

enum DevGroupMessageFlag { DGR_FLAG_RESET = 1, DGR_FLAG_STATUS_REQUEST = 2, DGR_FLAG_FULL_STATUS = 4, DGR_FLAG_ACK = 8, DGR_FLAG_MORE_TO_COME = 16, DGR_FLAG_DIRECT = 32, DGR_FLAG_ANNOUNCEMENT = 64, DGR_FLAG_LOCAL = 128 };

enum DevGroupItem { DGR_ITEM_EOL, DGR_ITEM_STATUS, DGR_ITEM_FLAGS,
                    DGR_ITEM_LIGHT_FADE, DGR_ITEM_LIGHT_SPEED, DGR_ITEM_LIGHT_BRI, DGR_ITEM_LIGHT_SCHEME, DGR_ITEM_LIGHT_FIXED_COLOR,
                    DGR_ITEM_BRI_PRESET_LOW, DGR_ITEM_BRI_PRESET_HIGH, DGR_ITEM_BRI_POWER_ON,
                    // Add new 8-bit items before this line
                    DGR_ITEM_LAST_8BIT, DGR_ITEM_MAX_8BIT = 63,
                    //DGR_ITEM_ANALOG1, DGR_ITEM_ANALOG2, DGR_ITEM_ANALOG3, DGR_ITEM_ANALOG4, DGR_ITEM_ANALOG5,
                    // Add new 16-bit items before this line
                    DGR_ITEM_LAST_16BIT, DGR_ITEM_MAX_16BIT = 127,
                    DGR_ITEM_POWER, DGR_ITEM_NO_STATUS_SHARE,
                    // Add new 32-bit items before this line
                    DGR_ITEM_LAST_32BIT, DGR_ITEM_MAX_32BIT = 191,
                    DGR_ITEM_EVENT, DGR_ITEM_COMMAND,
                    // Add new string items before this line
                    DGR_ITEM_LAST_STRING, DGR_ITEM_MAX_STRING = 223,
                    DGR_ITEM_LIGHT_CHANNELS };

enum DevGroupItemFlag { DGR_ITEM_FLAG_NO_SHARE = 1 };

enum DevGroupShareItem { DGR_SHARE_POWER = 1, DGR_SHARE_LIGHT_BRI = 2, DGR_SHARE_LIGHT_FADE = 4, DGR_SHARE_LIGHT_SCHEME = 8,
                         DGR_SHARE_LIGHT_COLOR = 16, DGR_SHARE_DIMMER_SETTINGS = 32, DGR_SHARE_EVENT = 64 };

enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
                     SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_OVERTEMP, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_REMOTE, SRC_SHUTTER,
                     SRC_THERMOSTAT, SRC_CHAT, SRC_TCL, SRC_BERRY, SRC_FILE, SRC_MAX };
const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|"
                                      "Timer|Rule|MaxPower|MaxEnergy|Overtemp|Light|Knx|Display|Wemo|Hue|Retry|Remote|Shutter|"
                                      "Thermostat|Chat|TCL|Berry|File";


// from tasmota_globals.h
#ifdef USE_DEVICE_GROUPS
#define SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, ...) _SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, __VA_ARGS__, 0)
uint8_t device_group_count = 0;
bool first_device_group_is_local = true;
#endif  // USE_DEVICE_GROUPS

// from settings.h

struct XDRVMAILBOX {
  bool          grpflg;
  bool          usridx;
  uint16_t      command_code;
  uint32_t      index;
  uint32_t      data_len;
  int32_t       payload;
  char         *topic;
  char         *data;
  char         *command;
} XdrvMailbox;

// tasmota.ino

struct TasmotaGlobal_t {
  bool skip_light_fade = false;                     // Temporarily skip light fading
  uint8_t devices_present = 8;                  // Max number of devices supported
  power_t power;                            // Current copy of Settings->power
} TasmotaGlobal;

typedef struct {
  uint8_t       device_group_tie[4];       // FB0
} TSettings;

TSettings* Settings = nullptr;




// support_tasmota.ino

void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source)
{
  power_t mask = 1 << (device -1);        // Device to control
  if (state <= POWER_TOGGLE) {
    switch (state) {
    case POWER_OFF: {
      TasmotaGlobal.power &= (POWER_MASK ^ mask);
      break; }
    case POWER_ON:
      TasmotaGlobal.power |= mask;
      break;
    case POWER_TOGGLE:
      TasmotaGlobal.power ^= mask;
    }
  }
}

void ExecuteCommand(const char *cmnd, uint32_t source)
{
  return;
}

enum XsnsFunctions {FUNC_SETTINGS_OVERRIDE, FUNC_PIN_STATE, FUNC_MODULE_INIT, FUNC_PRE_INIT, FUNC_INIT,
                    FUNC_LOOP, FUNC_EVERY_50_MSECOND, FUNC_EVERY_100_MSECOND, FUNC_EVERY_200_MSECOND, FUNC_EVERY_250_MSECOND, FUNC_EVERY_SECOND,
                    FUNC_SAVE_SETTINGS, FUNC_SAVE_AT_MIDNIGHT, FUNC_SAVE_BEFORE_RESTART,
                    FUNC_AFTER_TELEPERIOD, FUNC_JSON_APPEND, FUNC_WEB_SENSOR, FUNC_COMMAND, FUNC_COMMAND_SENSOR, FUNC_COMMAND_DRIVER,
                    FUNC_MQTT_SUBSCRIBE, FUNC_MQTT_INIT, FUNC_MQTT_DATA,
                    FUNC_SET_POWER, FUNC_SET_DEVICE_POWER, FUNC_SHOW_SENSOR, FUNC_ANY_KEY,
                    FUNC_ENERGY_EVERY_SECOND, FUNC_ENERGY_RESET,
                    FUNC_RULES_PROCESS, FUNC_TELEPERIOD_RULES_PROCESS, FUNC_SERIAL, FUNC_FREE_MEM, FUNC_BUTTON_PRESSED, FUNC_BUTTON_MULTI_PRESSED,
                    FUNC_WEB_ADD_BUTTON, FUNC_WEB_ADD_CONSOLE_BUTTON, FUNC_WEB_ADD_MANAGEMENT_BUTTON, FUNC_WEB_ADD_MAIN_BUTTON,
                    FUNC_WEB_ADD_HANDLER, FUNC_SET_CHANNELS, FUNC_SET_SCHEME, FUNC_HOTPLUG_SCAN,
                    FUNC_DEVICE_GROUP_ITEM };


bool XdrvCall(uint8_t Function)
{
  bool result = false;
  return result;
}


int Response_P(const char* format, ...)        // Content send snprintf_P char data
{
  return 0;
}