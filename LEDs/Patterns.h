#ifndef _PATTERNS_h
#define _PATTERNS_h

#include "FastLED.h"

// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, CYLON, FADE, MOVING_PALETTE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

class Patterns {
private:
	uint16_t _numLeds;
	CRGB *_leds;

public:
	pattern  ActivePattern;   // which pattern is running
	direction Direction;      // direction to run the pattern

	unsigned long Interval;   // milliseconds between updates
	unsigned long lastUpdate; // last update of position

	unsigned long SpeedChangeInterval;
	unsigned long lastSpeedChange;
	uint8_t MaxSpeed;
	uint8_t theta = 0;
	bool canReverse;

	uint8_t hue;
	CRGB Color1, Color2;	  // What colors are in use
	CRGBPalette16 Palette;    // What palette is in use
	TBlendType Blending;      // What palette blending mode is in use
	uint8_t Brightness;       // What palette brightness is in use
	uint8_t Speed;		      // What movement speed to use for palette
	uint8_t StartIndex;		  // The starting index for selecting a color from the palette

	uint16_t TotalSteps;      // total number of steps in the pattern
	uint16_t Index;           // current step within the pattern

	void(*OnComplete)(Patterns& sender);      // Callback on completion of pattern

	Patterns(CRGB leds[], uint16_t numLeds, void(*callback)(Patterns& sender) = NULL);

	// Update the pattern
	void Update();

	// Increment the Index and reset at the end
	void Increment();

	// Reverse pattern direction
	void Reverse();

	// Initialize for a RainbowCycle
	void RainbowCycle(uint16_t interval, direction dir = FORWARD);

	// Update the Rainbow Cycle Pattern
	void RainbowCycleUpdate();

	// Initialize for a Theater Chase
	void TheaterChase(CRGB color1, CRGB color2, uint16_t interval, direction dir = FORWARD);

	// Update the Theater Chase Pattern
	void TheaterChaseUpdate();

	// Initialize for a ColorWipe
	void ColorWipe(CRGB color, uint16_t interval, direction dir = FORWARD);

	// Update the Color Wipe Pattern
	void ColorWipeUpdate();

	// Initialize for a Scanner
	void Scanner(CRGB color1, uint16_t interval);

	// Update the Scanner Pattern
	void ScannerUpdate();

	// Initialize for a Cylon
	void Cylon(uint16_t interval);

	// Update the Cylon Pattern
	void CylonUpdate();

	// Initialize for a Fade
	void Fade(CRGB color1, CRGB color2, uint16_t steps, uint16_t interval, direction dir = FORWARD);

	// Update the Fade Pattern
	void FadeUpdate();

	// Initialize for a MovingPalette
	void MovingPalette(CRGBPalette16& palette, TBlendType blending, uint8_t brightness, uint16_t interval, uint8_t speedChangeInterval = 10, uint8_t maxSpeed = 20, direction dir = FORWARD);

	// Update the MovingPalette Pattern
	void MovingPaletteUpdate();

	// Set all pixels to a color (synchronously)
	void ColorSet(CRGB color);

	// Input a value 0 to 255 to get a color value.
	// The colours are a transition r - g - b - back to r.
	CRGB Wheel(byte wheelPos);
};

#endif