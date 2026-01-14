# 4LAMP (aka *flamp1*)
# ⚠️ Newer version available. Rewritten using esp-idf and FreeRTOS [here](https://github.com/Igor4er/4lamp2).
**A smart ([no, really](#my-complaints)) lighting solution built for the ESP8266 ESP-01 module.**

## 🔧 What It Does

4LAMP constantly pings your phone (or any chosen device) to detect when you enter your home. Once detected, it activates ***vibrant*** lights to greet your dramatic return.

Built to work with an N-channel MOSFET like the **IRLZ44N** for easy control of high-power LEDs or lamps.

## 💤 Auto Shutoff (aka “let me sleep in peace” mode)

If the internet connection drops, the lights turn off automatically.  
Why? Because my house is weird. I have to cut the internet to sleep without a miniature sun blinding me at 2AM. So I made this feature. For me. By me.

## 💡 Compatibility

Designed for **ESP8266 ESP-01**. Tested with **IRLZ44N** MOSFET.

Requires your phone to respond to pings. Be aware that IPhones don't do it while screen is off. Unfortunately i don't have any Android phone to check whether it would respond while sleeps.

## <a name="my-complaints"></a>🤖 What You Call "Smart Devices" Are Actually DUMB

Let’s be real - if a “smart” light needs to be turned on and off manually through an app, it's not smart. It's just wireless. You’ve basically traded your wall switch for a phone screen. It's even more complicated than wall swith if you think about it.

**4LAMP is actually smart.**  
It doesn’t wait for you to tap buttons - it *knows* when you're home and acts on its own. And when you disconnect the internet (or the world ends), it shuts off like a considerate little bot should.

This is the difference between *remote-controlled* and *automated*.
