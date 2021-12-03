# esphome sync

This project is due to a desire to leverage WLED's new virtual strip (via DDP) feature or software like xLights or LEDFx to controls ESP based smart bulbs. Thanks to @KaufHA for the example DDP implementation within his highly customized firmware for his RGB-CCT bulbs.

Using a light effect like a complete Arduino sketch (globals = `statics`, `state==0` for the `start()` and `state==1` for the `loop()`), I have been able to implement both DDP control and E131 control for 5 channel bulbs that I have as well as an 8 channel AC dimmer.

# The Why

The new feature in WLED to control "virtual strips" through the DDP protocol enables the use of ESPHome as a receiving node but with a firmware that is "safe" for 1MB flash chips.

I got the idea from Kauf who implemented the protocol for his bulbs specifically, and I have now implemented the idea for RGB+ bulbs and monochromatic lights (brightness only) receiving both DDP or E131.

Binary lights (relays) are supportable add well using a threshold of brightness to decide when to turn the relay on or off.

When not in DDP or E131 mode, the devices can be used as regular ESPHome light entities.

This method also bridges a gap left by unsupported LED drivers (like the my** that WLED doesn't support and has no reason to)