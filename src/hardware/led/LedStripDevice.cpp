#include "LedStripDevice.h"

#include <set>
#include "constants/TableConstants.h"
#include "services/supabase/SupabaseQueryService.h"
#include "services/supabase/SupabaseRealtimeService.h"
#include "services/supabase/utils/SupabaseFilterUtils.h"

#include "utils/TimeUtils.h"
#include "utils/TextUtils.h"
#include "utils/ColorUtils.h"
#include "hardware/led/utils/LedUtils.h"
#include "constants/LedSettingsConstants.h"
#include "constants/CoreConstants.h"
#include "debug/DebugCommandHandler.h"

const std::string REALTIME_TOPIC = "LedStripDevice";

using namespace LedSettingsConstants;
using namespace TableConstants;
using namespace ColorUtils;
using namespace LedUtils;

LedStripDevice::LedStripDevice() : BaseDevice(CoreConstants::DeviceType::toString(CoreConstants::DeviceType::Value::LED_STRIP)) {
    ledStrip.Begin();
}

void LedStripDevice::initialize(const JsonDocument &settings) {
    logger.info("Initializing LedStripDevice");
    handleSettingsUpdate(settings);
    enableDebugCommands();
}


void LedStripDevice::loop() {
    if (!listenerConnected && !colorProfileId.empty()) {
        createColorProfileListener();
    }
    if (isSingleColor) {
        singleColorLoop();
    } else {
        gradientLoop();
    }
}

void LedStripDevice::singleColorLoop() {
    logger.info("Single color loop");
    setSolidColor(ledStrip, colors[0]);
    ledStrip.Show();
}

static float period = 100.0f;
static float progress = 0.0f;
static uint16_t halfPixelCount = LED_PIXEL_COUNT / 2; // Divide by 2 because the led strip is folded

void LedStripDevice::gradientLoop() {
    int colorsCount = (int) colors.size();
    float pixelIncrement = (period / (float) (halfPixelCount)) * frequency;
    float color_period = period / (float) colorsCount;

    // Progress color index if progress surpasses color_period or below 0 (for reverse)
    if (progress >= color_period) {
        progress -= color_period;
        currentBaseColorIndex = (currentBaseColorIndex + 1) % colorsCount;
    } else if (progress < 0) {
        progress += color_period;
        currentBaseColorIndex = (currentBaseColorIndex - 1) % colorsCount;
        if (currentBaseColorIndex < 0) {
            currentBaseColorIndex = colorsCount - 1;
        }
    }
    RgbColor color1 = colorToRgbColor(colors[currentBaseColorIndex]);
    RgbColor color2 = colorToRgbColor(colors[(currentBaseColorIndex + 1) % colorsCount]);

    // Each "run" creates a new progress, to separate the main fade-progression from the "gradient"
    float gradientProgress = progress;
    int tempBaseColorIndex = currentBaseColorIndex;
    for (int i = 0; i < halfPixelCount; i++) {
        if (gradientProgress >= color_period) {
            gradientProgress = fmod(gradientProgress, color_period);
            tempBaseColorIndex = (tempBaseColorIndex + 1) % colorsCount;
            color1 = colorToRgbColor(colors[tempBaseColorIndex]);
            color2 = colorToRgbColor(colors[(tempBaseColorIndex + 1) % colorsCount]);
        }
        float offset = abs(gradientProgress) / color_period;
        RgbColor color = RgbColor::LinearBlend(color1, color2, offset);
        ledStrip.SetPixelColor(i, color);
        ledStrip.SetPixelColor(LED_PIXEL_COUNT - 1 - i, color);
        gradientProgress += pixelIncrement;
    }
    progress += reverse ? -0.5f : 0.5f;
    ledStrip.Show();
}

void LedStripDevice::handleSettingsUpdate(const JsonDocument &settings) {
    logger.info("Updating settings for LedStripDevice");
    bool dirty = false;
    std::string newColorProfileId = settings[COLOR_PROFILE_ID_KEY];
    if (colorProfileId != newColorProfileId) {
        colorProfileId = newColorProfileId;
        dirty = true;
    }
    fps = settings[FPS_KEY].as<int>();
    frequency = settings[FREQUENCY_KEY].as<float>();
    reverse = settings[REVERSE_KEY].as<bool>();
    if (dirty) {
        reload();
    }
}

JsonDocument LedStripDevice::getInitialColorProfile() {
    std::optional<JsonDocument> colorRowData = SupabaseQueryService::getInstance().select(ColorProfile::TABLE_NAME, ColorProfile::COLUMN_ID, colorProfileId);
    if (!colorRowData) {
        logger.error("Failed to get color data for color with id: " + colorProfileId);
        return JsonDocument();
    }
    JsonDocument colorData(colorRowData->as<JsonArray>()[0]);
    return colorData;
}


static unsigned long lastListenerRegisterAttemptMillis;

void LedStripDevice::createColorProfileListener() {
    if (!TimeUtils::isMillisElapsed(millis(), lastListenerRegisterAttemptMillis, 5000)) {
        return;
    }

    std::string filter = SupabaseFilterUtils::equals(ColorProfile::COLUMN_ID, colorProfileId);
    bool listenerCreatedSuccessfully =
            SupabaseRealtimeService::getInstance()
                    .addUpdateListener(REALTIME_TOPIC, ColorProfile::TABLE_NAME, filter,
                                       [this](const JsonVariantConst &data) {
                                           handleColorProfileUpdate(data);
                                       });
    listenerConnected = listenerCreatedSuccessfully;

    if (!listenerCreatedSuccessfully) {
        logger.error("Failed to create listener for LedStripDevice");
    }
}

void LedStripDevice::handleColorProfileUpdate(const JsonVariantConst &colorRow) {
    logger.info("Updating color profile for LedStripDevice");
    const std::string hexesString = colorRow[ColorProfile::COLUMN_HEXES];
    std::vector hexes = TextUtils::split(hexesString, ',');

    colors.clear();
    for (const std::string &hex: hexes) {
        colors.push_back(Gamma32(hexStringToColor(hex)));
    }

    if (colors.size() == 1) {
        isSingleColor = true;
    } else {
        isSingleColor = false;
    }
    loop();
}

int LedStripDevice::getFps() {
    // If single color, we don't really need to update the color that often
    if (isSingleColor) {
        return 1;
    }
    return fps;
}

void LedStripDevice::removeListener() {
    SupabaseRealtimeService::getInstance().removeListener(REALTIME_TOPIC);
    listenerConnected = false;
}

void LedStripDevice::reload() {
    currentBaseColorIndex = 0;
    removeListener();
    handleColorProfileUpdate(getInitialColorProfile());
}

void LedStripDevice::enableDebugCommands() {
    Debug::DebugCommandHandler::getInstance().registerListener("led-color", [this](std::string &args) {
        std::istringstream iss(args);
        std::string s;
        getline(iss, s, ' ');
        if (s.empty()) {
            return;
        }
        int number = strtol(s.c_str(), nullptr, 10);

        ledStrip.SetPixelColor(number, RgbColor(0, 0, 255));
        ledStrip.Show();
    });
}


