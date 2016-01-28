#include "FastLED.h"
#include "Patterns.h"

#define BRIGHTNESS 255
#define NUM_DUAL_STRIP_LEDS 300
#define NUM_SINGLE_STRIP_LEDS 67
#define NUM_CLOCKED_STRIP_LEDS 96
#define PATTERN_HOLD_SECONDS 90

CRGBPalette16 currentPalette = RainbowColors_p;
TBlendType currentBlending = LINEARBLEND;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB dualStripLeds[NUM_DUAL_STRIP_LEDS];
CRGB singleStripLeds[NUM_SINGLE_STRIP_LEDS];
CRGB clockedStripLeds[NUM_CLOCKED_STRIP_LEDS];

#define NUM_STRIPS 3
Patterns strips[] = {
	Patterns(dualStripLeds, NUM_DUAL_STRIP_LEDS, &patternComplete),
	Patterns(singleStripLeds, NUM_SINGLE_STRIP_LEDS, &patternComplete),
	Patterns(clockedStripLeds, NUM_CLOCKED_STRIP_LEDS, &patternComplete)
};

void setup() {
	delay(1000); // power-up safety delay

	FastLED.addLeds<WS2812B, 4, GRB>(dualStripLeds, NUM_DUAL_STRIP_LEDS);
	FastLED.addLeds<WS2812B, 6, GRB>(dualStripLeds, NUM_DUAL_STRIP_LEDS);
	FastLED.addLeds<WS2812B, 23, GRB>(singleStripLeds, NUM_SINGLE_STRIP_LEDS);
	FastLED.addLeds<LPD8806, 16, 15, GRB>(clockedStripLeds, NUM_CLOCKED_STRIP_LEDS);

	FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
	changePatternPeriodically();

	for (int i = 0; i < NUM_STRIPS; i++)
		strips[i].Update();

	FastLED.show();
}

void patternComplete(Patterns& sender) {
	if (sender.ActivePattern == RAINBOW_CYCLE) {
		sender.Reverse();
	}
	else if (sender.ActivePattern == THEATER_CHASE) {
		sender.Color1 = makeComplementaryColor(sender.Color1);
		sender.Color2 = makeComplementaryColor(sender.Color2);
		sender.Reverse();
	}
	else if (sender.ActivePattern == COLOR_WIPE) {
		sender.Color1 = makeComplementaryColor(sender.Color1);
		sender.Reverse();
	}
	else if (sender.ActivePattern == SCANNER) {
		sender.Reverse();
	}
	else if (sender.ActivePattern == CYLON) {
		sender.Reverse();
	}
	else if (sender.ActivePattern == FADE) {
		if (sender.Direction == FORWARD)
			sender.Color1 = makeComplementaryColor(sender.Color2);
		else
			sender.Color2 = makeComplementaryColor(sender.Color1);
		sender.Reverse();
	}
}

CRGB makeComplementaryColor(CRGB color) {
	CHSV original = RGB2HSV(color);
	uint16_t bound1, bound2, originalHue, newHue;
	originalHue = original.h;
	bound1 = originalHue + 100;
	bound2 = bound1 + 70;
	newHue = random16(bound1, bound2) % 255;
	return CHSV(newHue, random8(230, 255), random8(230, 255));
}

CHSV RGB2HSV(CRGB rgb) {
	uint8_t min, max, delta;

	if (rgb.r < rgb.g)
		min = rgb.r;
	else
		min = rgb.g;
	if (rgb.b < min)
		min = rgb.b;

	if (rgb.r > rgb.g)
		max = rgb.r;
	else
		max = rgb.g;
	if (rgb.b > max)
		max = rgb.b;

	CHSV hsv = CHSV(0, 0, 255); // v, 0..255

	delta = max - min; // 0..255, < v

	if (max != 0)
		hsv.s = (int)(delta)* 255 / max; // s, 0..255
	else {
		// r = g = b = 0 // s = 0, v is undefined
		hsv.s = 0;
		hsv.h = 0;
		return hsv;
	}

	if (rgb.r == max)
		hsv.h = (rgb.g - rgb.b) * 60 / delta;		// between yellow & magenta
	else if (rgb.g == max)
		hsv.h = 120 + (rgb.b - rgb.r) * 60 / delta; // between cyan & yellow
	else
		hsv.h = 240 + (rgb.r - rgb.g) * 60 / delta; // between magenta & cyan

	if (hsv.h < 0)
		hsv.h += 360;

	return hsv;
}

void adjustIntervals() {
	if (NUM_STRIPS > 1) {
		// The dual strips are wrapped around a pole so they need to move quicker than the others
		strips[1].Interval = strips[0].Interval * 2;
		strips[2].Interval = strips[0].Interval * 2;
	}
}

CRGBPalette16& getCloudColorsPalette() {
	currentPalette = CloudColors_p;
	return currentPalette;
}

CRGBPalette16& getLavaColorsPalette() {
	currentPalette = LavaColors_p;
	return currentPalette;
}

CRGBPalette16& getOceanColorsPalette() {
	currentPalette = OceanColors_p;
	return currentPalette;
}

CRGBPalette16& getRainbowColorsPalette() {
	currentPalette = RainbowColors_p;
	return currentPalette;
}

CRGBPalette16& getForestColorsPalette() {
	currentPalette = ForestColors_p;
	return currentPalette;
}

CRGBPalette16& getPartyColorsPalette() {
	currentPalette = PartyColors_p;
	return currentPalette;
}

CRGBPalette16& getRainbowStripeColorsPalette()
{
	currentPalette = RainbowStripeColors_p;
	return currentPalette;
}

CRGBPalette16& getMyRedWhiteAndBluePalette()
{
	currentPalette = myRedWhiteBluePalette_p;
	return currentPalette;
}

CRGBPalette16& getHeatColorsPalette()
{
	currentPalette = HeatColors_p;
	return currentPalette;
}

CRGBPalette16& getRedAndBlueStripedPalette() {
	CRGB red = CRGB::Red;
	CRGB blue = CRGB::Blue;

	currentPalette = CRGBPalette16(
		red, red, red, red,
		blue, blue, blue, blue,
		red, red, red, red,
		blue, blue, blue, blue);

	return currentPalette;
}

CRGBPalette16& getPurpleAndGreenPalette() {
	CRGB purple = CHSV(HUE_PINK, 255, 255);
	CRGB green = CHSV(HUE_GREEN, 255, 255);
	CRGB black = CRGB::Black;

	currentPalette = CRGBPalette16(
		green, green, green, black,
		purple, purple, purple, black,
		green, green, green, black,
		purple, purple, purple, black);

	return currentPalette;
}

CRGBPalette16& getRedGreenBlueWhitePalette()
{
	CRGB red = CHSV(HUE_RED, 255, 255);
	CRGB green = CHSV(HUE_GREEN, 255, 255);
	CRGB blue = CHSV(HUE_BLUE, 255, 255);
	CRGB white = CRGB::White;

	currentPalette = CRGBPalette16(
		red, red, red, red,
		green, green, green, green,
		blue, blue, blue, blue,
		white, white, white, white);

	return currentPalette;
}

// This function generates a random palette that's a gradient
// between four different colors. The first is a dim hue, the second is 
// a bright hue, the third is a bright pastel, and the last is 
// another bright hue. This gives some visual bright/dark variation
// which is more interesting than just a gradient of different hues.
CRGBPalette16& getRandomPalette()
{
	currentPalette = CRGBPalette16(
		CHSV(random8(), 255, 32),
		CHSV(random8(), 255, 255),
		CHSV(random8(), 128, 255),
		CHSV(random8(), 255, 255));

	return currentPalette;
}

void changePatternPeriodically() {
	uint8_t patternIndex = ((millis() / 1000) / PATTERN_HOLD_SECONDS) % 30;
	static uint8_t previousPatternIndex = 99;

	if (patternIndex != previousPatternIndex) {
		previousPatternIndex = patternIndex;
		Serial.print("Pattern index: ");
		Serial.println(patternIndex);

		uint8_t interval1 = random8(10, 15);
		uint8_t interval2 = random8(30, 40);
		uint8_t interval3 = random8(2, 4);
		uint8_t speedChangeInterval = random8(2, 4);
		uint8_t maxSpeed = random8(4, 5);
		CRGB color1 = CHSV(random8(), 255, 255);
		CRGB color2 = makeComplementaryColor(color1);
		uint8_t hue = random8(0, 255);

		if (patternIndex == 0) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRedAndBlueStripedPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 1) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].RainbowCycle(interval1);
		}
		else if (patternIndex == 2) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getCloudColorsPalette(), currentBlending, BRIGHTNESS, interval2, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 3) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRainbowColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 4) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].RainbowCycle(interval2);
		}
		else if (patternIndex == 5) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].ColorWipe(color1, interval1);
		}
		else if (patternIndex == 6) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getOceanColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 7) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRandomPalette(), currentBlending, BRIGHTNESS, interval2, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 8) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getForestColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 9) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].Cylon(hue, interval3);
		}
		else if (patternIndex == 10) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getPartyColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 11) {
			for (int i = 0; i < NUM_STRIPS; i++) {
				strips[i].Fade(color1, color2, 256, interval1);
			}
		}
		else if (patternIndex == 12) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].RainbowCycle(interval2);
		}
		else if (patternIndex == 13) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getPurpleAndGreenPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 14) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRandomPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 15) {
			for (int i = 0; i < NUM_STRIPS; i++) {
				strips[i].TheaterChase(color1, color2, 255);
			}
		}
		else if (patternIndex == 16) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRainbowStripeColorsPalette(), currentBlending, BRIGHTNESS, interval2, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 17) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].Scanner(color1, interval3);
		}
		else if (patternIndex == 18) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getHeatColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 19) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].ColorWipe(color1, interval1);
		}
		else if (patternIndex == 20) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].Cylon(hue, interval3);
		}
		else if (patternIndex == 21) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRandomPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 22) {
			for (int i = 0; i < NUM_STRIPS; i++) {
				strips[i].Fade(color1, color2, 256, interval1);
			}
		}
		else if (patternIndex == 23) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRainbowColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 24) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getRedGreenBlueWhitePalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 25) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].Cylon(hue, interval3);
		}
		else if (patternIndex == 26) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].RainbowCycle(interval1);
		}
		else if (patternIndex == 27) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getPartyColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 28) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getLavaColorsPalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}
		else if (patternIndex == 29) {
			for (int i = 0; i < NUM_STRIPS; i++)
				strips[i].MovingPalette(getMyRedWhiteAndBluePalette(), currentBlending, BRIGHTNESS, interval1, speedChangeInterval, maxSpeed);
		}

		adjustIntervals();
	}
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM = {
	CRGB::Red,
	CRGB::Gray, // 'white' is too bright compared to red and blue
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Black,

	CRGB::Red,
	CRGB::Red,
	CRGB::Gray,
	CRGB::Gray,
	CRGB::Blue,
	CRGB::Blue,
	CRGB::Black,
	CRGB::Black
};