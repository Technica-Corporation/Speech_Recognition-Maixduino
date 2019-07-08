# Maix Speech Recognition Demo

Quick Speech Recognition demo using the Maixduino Speech Recognition library.

### Prerequisites

This demo was run on a Sipeed Maix One Dock with the Microphone Array and LCD screen. 
I had tried using the built-in microphone, but was unable to get it working with Maixduino; the microphone did work when used with just the k210 standalone SDK though.

I used PlatformIO to build this demo, so these instructions will also be using it.

### Environment Setup

Install PlatformIO(and vscode)
```
https://platformio.org/install/ide?install=vscode
```

Add a new Platform, and select 'Advanced Installation'
```
Use the Sipeed k210 Platform repository for Maixduino v0.3.9
https://github.com/sipeed/platform-kendryte210.git#e6ff97955c4cb15f7fe39450cadb8f145caefbf1
```

Import the Speech Recognition Demo
```
Select 'Sipeed MAIX ONE DOCK' as the board
Check 'Use libraries installed by Arduino IDE'
```

## Running the Demo

After the demo is built and uploaded to the device, it should be displaying "Waiting for Wake Up..." on the LCD.
This is the 'Standby' loop and will proceed when it is awake.
```
One or Two claps: This will move it to the second loop
Moving the Microphone Array a short distance across a table/desk: Nothing
Clearing you Throat: Nothing
```

The second loop will have it displaying "Color Command..." on the LCD.
```
One or Two claps: This will move it back to the standby loop
Moving the Microphone Array a short distance across a table/desk: Nothing
Clearing your Throat: Nothing
Red:     Change LCD display RED
Green:   Change LCD display GREEN
Blue:    Change LCD display BLUE
Cyan:    Change LCD display CYAN
Magenta: Change LCD display MAGENTA
Yellow:  Change LCD display YELLOW
```

While running, you will definately notice that it recognizes wrong sometimes; this is pretty normal.
I recorded the models on a desk in front of me, so it was fairly close; you may have to adjust your speaking distance. 
For certain colors you may have to stress some of the vowels as well to get it to register correctly.
```
For Example, "RED" will likely not be recognized if you say it very quickly with a short 'E'
If you say "RED" with a long 'E' it will have an easier time recognizing it.
```

Other reasons it will recognize incorrectly it due to how the recognizer itself works.
It will pick the model it thinks is 'closest' to whatever it hears, even if that word/phrase was not included as a model at all.

## Adding Additional Models
Models were recorded using the example present in the Maix Speech Recognition library examples.
```
https://github.com/sipeed/Maixduino/tree/master/libraries/Maix_Speech_Recognition/examples/get_voice_model
```

Just use that example to record new models and add them like the four model headers included in this demo.
The example will output a 'number of frames' and an 'array' to a Serial Monitor.
```
Each Model entry should look something like this:
    const uint16_t fram_num_NAME = NUMBER_OF_FRAMES_HERE;
    const int16_t NAME_0[vv_frm_max*mfcc_num] = {
	ARRAY_HERE
	};
```

Each Model can have up to four recordings keyed to it, and then the SpeechRecognizer can have twenty models registered. 
