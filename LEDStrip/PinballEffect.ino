uint8_t currentPixel = 0;
uint8_t currentPixelBrightness = 0;

uint8_t activeStartPixels = 0;
uint8_t activeEndPixels = 0;
bool forward = true;
int color = ColorUtils::rgbToColor(255, 0, 0);

void pinballCycle(Adafruit_NeoPixel &strip ) {
  incrementActivation();

  int weightedActivation = activationToWeighted(currentPixelBrightness);
  int inverseWeightedActivation = activationToWeighted(255 - currentPixelBrightness);
  int previousPixel = getPreviousBallPixel();
  strip.setPixelColor(currentPixel, weightedActivation, 0, 0);
  if (previousPixel >= getStartingBallPixel())
     strip.setPixelColor(previousPixel, inverseWeightedActivation, 0, 0);
  for (int i = 0; i < activeStartPixels; i++) {
     strip.setPixelColor(i, 255, 0, 0);
  }
  for (int i = 0; i < activeEndPixels; i++) {
     strip.setPixelColor(PIXEL_COUNT - 1 - i, 255, 0, 0);
  }
}

int incrementActivation() {
  if (currentPixelBrightness == 255) {
    if (forward && currentPixel == getFinalBallPixel()) {
      ballReachedEnd();
    } else if (!forward && currentPixel == getStartingBallPixel()) {
      ballReachedStart();
    }
    currentPixel = currentPixel + (forward ? 1 : -1);
    currentPixelBrightness = 0;
    if (isFinished())
      reset();
} else
    currentPixelBrightness++;
}

void reset() {
  currentPixel = 0;
  currentPixelBrightness = 0;
  activeStartPixels = 0;
  activeEndPixels = 0;
  forward = true;
  for (int i = 0; i < PIXEL_COUNT; i++) {
     strip.setPixelColor(i, 0, 0, 0);
  }
}

void ballReachedEnd() {
  forward = false;
  activeEndPixels++;
}

void ballReachedStart() {
  forward = true;
  activeStartPixels++;
}

bool isFinished() {
  return getStartingBallPixel() > getFinalBallPixel();
}

int getFinalBallPixel() {
  return PIXEL_COUNT - 1 - activeEndPixels;
}

int getStartingBallPixel() {
  return activeStartPixels; //if cycPixel 1 time, the first Pixel will be 1
}

int getPreviousBallPixel() {
  return forward ? currentPixel - 1 : currentPixel + 1;
}

uint8_t activationToWeighted(uint8_t activation) {
  float activationFactor = (-0.373 * activation) / (activation - 350); //Found by trial and error with GeoGebra
  return activationFactor * 255;
}
