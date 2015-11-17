#include "Patterns.h"

Patterns::Patterns(CRGB leds[], uint16_t numLeds, void(*callback)(Patterns&)) : _leds(leds), _numLeds(numLeds) {
	OnComplete = callback;
}

void Patterns::Update() {
	if ((millis() - lastUpdate) > Interval) {
		lastUpdate = millis();
		switch (ActivePattern)
		{
		case RAINBOW_CYCLE:
			RainbowCycleUpdate();
			break;
		case THEATER_CHASE:
			TheaterChaseUpdate();
			break;
		case COLOR_WIPE:
			ColorWipeUpdate();
			break;
		case SCANNER:
			ScannerUpdate();
			break;
		case CYLON:
			CylonUpdate();
			break;
		case FADE:
			FadeUpdate();
			break;
		case MOVING_PALETTE:
			MovingPaletteUpdate();
			break;
		default:
			break;
		}
	}
}

void Patterns::Increment() {
	if (Direction == FORWARD) {
		Index++;
		if (Index >= TotalSteps) {
			Index = 0;
			if (OnComplete != NULL)
				OnComplete(*this); // call the completion callback
		}
	}
	else // Direction == REVERSE
	{
		--Index;
		if (Index <= 0) {
			Index = TotalSteps - 1;
			if (OnComplete != NULL)
				OnComplete(*this); // call the completion callback
		}
	}
}

void Patterns::Reverse() {
	if (Direction == FORWARD) {
		Direction = REVERSE;
		Index = TotalSteps - 1;
	}
	else {
		Direction = FORWARD;
		Index = 0;
	}
}

void Patterns::RainbowCycle(uint16_t interval, direction dir) {
	ActivePattern = RAINBOW_CYCLE;
	Interval = interval;
	TotalSteps = 256;
	Index = 0;
	Direction = dir;
}

void Patterns::RainbowCycleUpdate() {
	for (int i = 0; i < _numLeds; i++)
		_leds[i] = Wheel(((i * 256 / _numLeds) + Index) & 255);
	Increment();
}

void Patterns::TheaterChase(CRGB color1, CRGB color2, uint16_t interval, direction dir) {
	ActivePattern = THEATER_CHASE;
	Interval = interval;
	TotalSteps = _numLeds;
	Color1 = color1;
	Color2 = color2;
	Index = 0;
	Direction = dir;
}

void Patterns::TheaterChaseUpdate() {
	for (int i = 0; i < _numLeds; i++) {
		if ((i + Index) % 3 == 0)
			_leds[i] = Color1;
		else
			_leds[i] = Color2;
	}
	Increment();
}

void Patterns::ColorWipe(CRGB color, uint16_t interval, direction dir) {
	ActivePattern = COLOR_WIPE;
	Interval = interval;
	TotalSteps = _numLeds;
	Color1 = color;
	Index = 0;
	Direction = dir;
}

void Patterns::ColorWipeUpdate() {
	_leds[Index] = Color1;
	Increment();
}

void Patterns::Scanner(CRGB color1, uint16_t interval) {
	ActivePattern = SCANNER;
	Interval = interval;
	TotalSteps = (_numLeds - 1) * 2;
	Color1 = color1;
	Index = 0;
}

void Patterns::ScannerUpdate() {
	for (int i = 0; i < _numLeds; i++) {
		if (i == Index)
			_leds[i] = Color1; // Scan Pixel to the right
		else if (i == TotalSteps - Index)
			_leds[i] = Color1; // Scan Pixel to the left
		else {
			_leds[i].nscale8(250); // Fading tail
		}
	}
	Increment();
}

void Patterns::Cylon(uint16_t interval) {
	ActivePattern = CYLON;
	Interval = interval;
	TotalSteps = (_numLeds - 1) * 2;
	Index = 0;
}

void Patterns::CylonUpdate() {
	for (int i = 0; i < _numLeds; i++) {
		if (i == Index)
			_leds[i] = CHSV(hue++, 255, 255); // Scan Pixel to the right
		else if (i == TotalSteps - Index)
			_leds[i] = CHSV(hue++, 255, 255); // Scan Pixel to the left
		else {
			_leds[i].nscale8(250); // Fading tail
		}
	}
	Increment();
}

void Patterns::Fade(CRGB color1, CRGB color2, uint16_t steps, uint16_t interval, direction dir) {
	ActivePattern = FADE;
	Interval = interval;
	TotalSteps = steps;
	Color1 = color1;
	Color2 = color2;
	Index = 0;
	Direction = dir;
}

void Patterns::FadeUpdate() {
	// Calculate linear interpolation between Color1 and Color2
	// Optimise order of operations to minimize truncation error
	uint8_t red = ((Color1.r * (TotalSteps - Index)) + (Color2.r * Index)) / TotalSteps;
	uint8_t green = ((Color1.g * (TotalSteps - Index)) + (Color2.g * Index)) / TotalSteps;
	uint8_t blue = ((Color1.b * (TotalSteps - Index)) + (Color2.b * Index)) / TotalSteps;

	ColorSet(CRGB(red, green, blue));
	Increment();
}

void Patterns::MovingPalette(CRGBPalette16& palette, TBlendType blending, uint8_t brightness, uint16_t interval, uint8_t speedChangeInterval, uint8_t maxSpeed, direction dir) {
	ActivePattern = MOVING_PALETTE;
	Interval = interval;
	SpeedChangeInterval = speedChangeInterval;
	MaxSpeed = maxSpeed;
	theta = 0;
	canReverse = true;
	TotalSteps = 256;
	Palette = palette;
	Blending = blending;
	Brightness = brightness;
	Speed = 1;
	StartIndex = 0;
	Index = 0;
	Direction = dir;
}

void Patterns::MovingPaletteUpdate() {
	if (millis() - lastSpeedChange > SpeedChangeInterval) {
		uint8_t sinTheta = triwave8(theta);
		uint8_t newSpeed = scale8(sinTheta, MaxSpeed);
		if (newSpeed == 0 && canReverse) {
			Reverse();
			Speed = 1;
			canReverse = false;
		}
		else if (newSpeed >= 1) {
			Speed = newSpeed;
			canReverse = true;
		}
		theta++;
		lastSpeedChange = millis();
	}

	if (Direction == FORWARD)
		StartIndex += Speed;
	else
		StartIndex -= Speed;
	uint8_t colorIndex = StartIndex;
	for (int i = 0; i < _numLeds; i++) {
		_leds[i] = ColorFromPalette(Palette, colorIndex, Brightness, Blending);
		colorIndex += 3;
	}
	Increment();
}

void Patterns::ColorSet(CRGB color) {
	for (int i = 0; i < _numLeds; i++)
		_leds[i] = color;
}

CRGB Patterns::Wheel(byte wheelPos) {
	wheelPos = 255 - wheelPos;
	if (wheelPos < 85)
		return CRGB(255 - wheelPos * 3, 0, wheelPos * 3);
	else if (wheelPos < 170) {
		wheelPos -= 85;
		return CRGB(0, wheelPos * 3, 255 - wheelPos * 3);
	}
	else {
		wheelPos -= 170;
		return CRGB(wheelPos * 3, 255 - wheelPos * 3, 0);
	}
}