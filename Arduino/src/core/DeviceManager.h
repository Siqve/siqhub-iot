#include <memory>
#include "debug/Logger.h"
#include "core/types/BaseDevice.h"
#include "ArduinoJson.h"

class DeviceManager {
public:
    void loop();
private:
    void init();
    void registerChangeListener();

    std::unique_ptr<BaseDevice> createDevice(const std::string& deviceType);
    void configure(const JsonVariantConst& config);
    JsonDocument getDeviceConfig();

    std::unique_ptr<BaseDevice> device;
    Debug::Logger logger = Debug::Logger("DeviceManager");
    boolean initialized = false;
    boolean listenerActive = false;
};

