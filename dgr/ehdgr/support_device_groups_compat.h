#define USE_DEVICE_GROUPS                        // Add support for device groups (+5k5 code)
  #define DEVICE_GROUPS_ADDRESS 239,255,250,250  // Device groups multicast address
  #define DEVICE_GROUPS_PORT 4447                // Device groups multicast port
  #define DEVICE_GROUPS_DEBUG
  // #define USE_DEVICE_GROUPS_SEND 

const uint8_t MAX_RELAYS = 20;

bool ehdgr_power_1 = false;
bool ehdgr_power_2 = false;
bool ehdgr_power_3 = false;
bool ehdgr_power_4 = false;
bool ehdgr_power_5 = false;
bool ehdgr_power_6 = false;
bool ehdgr_power_7 = false;
bool ehdgr_power_8 = false;
bool ehdgr_power_9 = false;
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

// from i18n.h
#define D_CMND_DEVGROUPSTATUS "DevGroupStatus"

// from tasmota_globals.h
#ifdef USE_DEVICE_GROUPS
#define SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, ...) _SendDeviceGroupMessage(DEVICE_INDEX, REQUEST_TYPE, __VA_ARGS__, 0)
uint8_t device_group_count = 0;
bool first_device_group_is_local = true;
#endif  // USE_DEVICE_GROUPS

// from settings.h
typedef union {
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption82 .. SetOption113
    uint32_t device_groups_enabled : 1;    // bit 3 (v8.1.0.9)   - SetOption85  - (DevGroups) Enable Device Groups (1)
    uint32_t multiple_device_groups : 1;   // bit 6 (v8.1.0.9)   - SetOption88  - (DevGroups) Enable relays in separate device groups/PWM Dimmer Buttons control remote devices (1)
  };
} SOBitfield4;

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

//
// modified from tasmota.ino
//
struct TasmotaGlobal_t {
  bool skip_light_fade = false;                     // Temporarily skip light fading
  uint8_t devices_present = 20;                  // Max number of devices supported
  power_t power;                            // Current copy of Settings->power
  uint8_t restart_flag = 0;                     // Tasmota restart flag
} TasmotaGlobal;

typedef struct {
  SOBitfield4   flag4;                     // EF8
  uint8_t       device_group_tie[4];       // FB0
  uint32_t      device_group_share_in;     // FCC  Bitmask of device group items imported
  uint32_t      device_group_share_out;    // FD0  Bitmask of device group items exported
} TSettings;

TSettings* Settings = nullptr;

//
// modified from support_tasmota.ino
//
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
  ehdgr_power_1 = TasmotaGlobal.power & (1 << (1 - 1));
  ehdgr_power_2 = TasmotaGlobal.power & (1 << (2 - 1));
  ehdgr_power_3 = TasmotaGlobal.power & (1 << (3 - 1));
  ehdgr_power_4 = TasmotaGlobal.power & (1 << (4 - 1));
  ehdgr_power_5 = TasmotaGlobal.power & (1 << (5 - 1));
  ehdgr_power_6 = TasmotaGlobal.power & (1 << (6 - 1));
  ehdgr_power_7 = TasmotaGlobal.power & (1 << (7 - 1));
  ehdgr_power_8 = TasmotaGlobal.power & (1 << (8 - 1));
  ehdgr_power_9 = TasmotaGlobal.power & (1 << (9 - 1));
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

bool _SendDeviceGroupMessage(int32_t device, DevGroupMessageType message_type, ...);

void InitTasmotaCompatibility()
{
  Settings = (TSettings*)malloc(sizeof(TSettings));
  Settings->device_group_share_in = 0xFFFFFFFF; 
  Settings->device_group_share_out = 0x00000000;
  Settings->flag4.device_groups_enabled = 1;
  Settings->flag4.multiple_device_groups = 0;
}

enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_ALL};

void AddLogData(uint32_t loglevel, const char* log_data) {
  if (loglevel == LOG_LEVEL_ERROR) {
    ESP_LOGE("DGR", "%s", log_data);
  } else if (loglevel == LOG_LEVEL_INFO)
  {
    ESP_LOGW("DGR", "%s", log_data);
  } else if (loglevel == LOG_LEVEL_DEBUG)
  {
    ESP_LOGI("DGR", "%s", log_data);
  } else if (loglevel == LOG_LEVEL_DEBUG_MORE)
  {
    ESP_LOGD("DGR", "%s", log_data);
  }
}

void AddLog(uint32_t loglevel, const char*  formatP, ...) {
  va_list arg;
  va_start(arg, formatP);
  if (loglevel == LOG_LEVEL_ERROR) {
    esp_log_vprintf_(ESPHOME_LOG_LEVEL_ERROR, "DGR", __LINE__, formatP, arg);
  } else if (loglevel == LOG_LEVEL_INFO)
  {
    esp_log_vprintf_(ESPHOME_LOG_LEVEL_WARN, "DGR", __LINE__, formatP, arg);
  } else if (loglevel == LOG_LEVEL_DEBUG)
  {
    esp_log_vprintf_(ESPHOME_LOG_LEVEL_INFO, "DGR", __LINE__, formatP, arg);
  } else if (loglevel == LOG_LEVEL_DEBUG_MORE)
  {
    esp_log_vprintf_(ESPHOME_LOG_LEVEL_DEBUG, "DGR", __LINE__, formatP, arg);
  }
}