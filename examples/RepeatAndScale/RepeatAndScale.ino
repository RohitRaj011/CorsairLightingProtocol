/*
   Copyright 2019 Leon Kiefer

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include <CorsairLightingFirmware.h>
#include <FastLEDController.h>
#include <CorsairLightingProtocol.h>
#include <CorsairLightingProtocolHID.h>
#include <FastLEDControllerUtils.h>
#include <FastLED.h>

#define DATA_PIN_CHANNEL_1 2
#define DATA_PIN_CHANNEL_2 3

CRGB ledsChannel1[100];
CRGB ledsChannel2[144];

CorsairLightingFirmware firmware = corsairLightingNodePROFirmware();
FastLEDController ledController(true);
CorsairLightingProtocol cLP(&ledController, &firmware);
CorsairLightingProtocolHID cHID(&cLP);

void setup() {
	FastLED.addLeds<NEOPIXEL, DATA_PIN_CHANNEL_1>(ledsChannel1, 100);
	FastLED.addLeds<NEOPIXEL, DATA_PIN_CHANNEL_2>(ledsChannel2, 144);
	ledController.addLEDs(0, ledsChannel1, 50);
	ledController.addLEDs(1, ledsChannel2, 60);
	ledController.onUpdateHook(0, []() {
		CLP::repeat(&ledController, 0, 2);
	});
	ledController.onUpdateHook(1, []() {
		CLP::scale(&ledController, 1, 144);
	});
}

void loop() {
	cHID.update();

	if (ledController.updateLEDs()) {
		FastLED.show();
	}
}
