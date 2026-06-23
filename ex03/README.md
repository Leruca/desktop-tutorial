# Ex 03 - SOS LED Signal With `millis()`

This assignment uses Arduino's `millis()` function to play an SOS light signal on the ESP32 built-in LED.

## Pattern

SOS in Morse code is:

- S: three short flashes
- O: three long flashes
- S: three short flashes
- Then a longer off pause before repeating

## Timing

- Short flash: 200 ms
- Long flash: 600 ms
- Gap between flashes in one letter: 200 ms
- Gap between letters: 600 ms
- Gap between repeated SOS messages: 2000 ms

The sketch stores the whole SOS sequence in a step table and advances through it with elapsed-time checks from `millis()`, without using `delay()`.
