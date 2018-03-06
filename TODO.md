# TODO

RandGen()/RandSleep()
- Replace random generators with `uint8_t RandGen();` and `uint8_t RandSleep();`
- Remove Xns and replace with RandSleep().

Signal Queue/LinkedList
- Replace current thread/signal system for single-thread operation
- Add interrupt queue
- Expand linked list function support (list item count, removal by index, et al.)

Chat/Keystroke Handler
- Simplify the SendKeys() and SendKeysQueue() functions
- Add Ctrl+A/C/V to send chat.
- Add / command support and chat selector

Bot Controller
- Set 100% to hold key instead of reset.

User Interface
- Add Start/Stop/Eject/Fire
- Add status bar

Functionality
- Add auto-update
- Fix idle bot

*keri*
