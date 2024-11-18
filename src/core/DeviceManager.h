#include <memory>
#include "debug/Logger.h"
#include "BaseDevice.h"
#include "ArduinoJson.h"
#include "constants/CoreConstants.h"

class DeviceManager {
public:
    void loop();
private:

    void configure();
    void first(); //TODO usikker på hva denne er
    void registerChangeListener();

    std::shared_ptr<BaseDevice> getDevice();
    std::shared_ptr<BaseDevice> createDevice(CoreConstants::DeviceType::Value deviceType);

    void handleConfigUpdate(const JsonVariantConst &config);
    void changeDevice(CoreConstants::DeviceType::Value deviceType);
    void onConfigUpdate(const JsonVariantConst& config);
    [[nodiscard]] JsonDocument getDeviceConfig();
    [[nodiscard]] bool isConfigured() const;

    CoreConstants::DeviceType::Value activeDeviceType = CoreConstants::DeviceType::Value::UNKNOWN;
    std::unordered_map<CoreConstants::DeviceType::Value, std::shared_ptr<BaseDevice>> deviceTypes;



    std::unique_ptr<BaseDevice> device;

    Debug::Logger logger = Debug::Logger("DeviceManager");
    boolean listenerActive = false;
};

