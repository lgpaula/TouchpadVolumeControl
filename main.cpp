#include "Audio/AudioHandler.hpp"
#include "Device/TouchpadListener.hpp"

int main() {

    AudioHandler audioHandler;

    TouchpadListener touchpadListener;
    touchpadListener.processEvents();

    touchpadListener.setVolumeChangeCallback([&audioHandler](int volume) {
//        audioHandler.setVolume(10);
        audioHandler.setVolume(volume);
    });

    return 0;
//    BTN_LEFT;
}

/* single top left
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=193        track id
Got input_event type=3 code=53 value=44         x pos
Got input_event type=3 code=54 value=31         y pos
Got input_event type=1 code=330 value=1         single touch
Got input_event type=3 code=57 value=-1         let go
Got input_event type=1 code=330 value=0         let go
*/

/* single top right
Got input_event type=3 code=57 value=194
Got input_event type=3 code=53 value=1133
Got input_event type=3 code=54 value=18
Got input_event type=1 code=330 value=1
Got input_event type=3 code=0 value=1133        auto-repeat?
Got input_event type=3 code=1 value=18          auto-repeat?
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=330 value=0
*/

/* double touch:
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=197
Got input_event type=3 code=53 value=296
Got input_event type=3 code=54 value=339
Got input_event type=1 code=330 value=1
Got input_event type=1 code=325 value=1
Got input_event type=4 code=5 value=0           axes
Got input_event type=3 code=47 value=1          multi-touch slot being modified
Got input_event type=3 code=57 value=198
Got input_event type=3 code=53 value=743
Got input_event type=3 code=54 value=360
Got input_event type=1 code=325 value=0
Got input_event type=1 code=333 value=1         double tap
Got input_event type=4 code=5 value=7200        timestamp
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=14400
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=21600
Got input_event type=0 code=0 value=0
Got input_event type=4 code=5 value=28800
Got input_event type=0 code=0 value=0
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=53 value=290
Got input_event type=3 code=54 value=351
Got input_event type=3 code=47 value=1
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=325 value=1
Got input_event type=1 code=333 value=0         double tap
Got input_event type=3 code=0 value=290
Got input_event type=3 code=1 value=351
Got input_event type=4 code=5 value=36000
Got input_event type=0 code=0 value=0
Got input_event type=3 code=47 value=0          multi-touch slot being modified
Got input_event type=3 code=57 value=-1
Got input_event type=1 code=330 value=0
Got input_event type=1 code=325 value=0
Got input_event type=4 code=5 value=43200
Got input_event type=0 code=0 value=0
*/