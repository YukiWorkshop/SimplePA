# SimplePA

Very easy-to-use C++ PulseAudio client library.

## Features
- Easy
- Very easy
- Extremely easy!!

## Requirements
-  libpulse & libpulse-simple

And reasonably new versions of:
-  C++17 compatible compiler
-  CMake

## Install
Use of the [CPM](https://github.com/TheLartians/CPM.cmake) package manager is recommended.

```cmake
include(cmake/CPM.cmake)

CPMAddPackage(
        NAME SimplePA
        GITHUB_REPOSITORY YukiWorkshop/SimplePA
        VERSION 0.0.1
)

target_include_directories(your_project PUBLIC ${SimplePA_SOURCE_DIR})
target_link_libraries(your_project SimplePA)
```

## Usage
```cpp
#include <SimplePA.hpp>

using namespace YukiWorkshop;
```

All operations should be surrounded by `try` and `catch`.
The exception type is `std::runtime_error` unless otherwise specified.

Playback:
```cpp
uint8_t buf[1024]; // Some random data in your memory

SimplePA::Player p; // Defaults to s16le, 44100Hz, Stereo
p.open();
p.play(buf, 1024); // That's it!
```

Record:
```cpp
SimplePA::Recorder r; // Defaults to s16le, 44100Hz, Stereo
r.open();

std::vector<uint8_t> buf(10 * 2 * 2 * 44100); // Buffer for audio data of 10 secs
r.record(buf.data(), buf.size()); // That's it!
```

Quick custom settings. You can always omit parameters that come last.
```cpp
SimplePA::Player p2(
                    "MyApp", // Application name
                    "My Music", // Stream name
                    {PA_SAMPLE_S32LE, 192000, 2}, // Format, sample rate, channels
                    "Sound Blaster X7", // Device name
                    "10.0.0.1" // Server name
                   );
```

Or set them one by one. You can chain these functions.
```cpp
SimplePA::Player p3;
p3.set_name("MyApp").set_stream_name("My Music");
p3.set_device("Sound Blaster X7").set_server("10.0.0.1");
// ...
```

Get latency information in nanoseconds:
```cpp
p.latency();
```

The callback interface:
```cpp
SimplePA::Player p;
p.open();
p.run([](SimplePA::Player &player, std::vector<uint8_t> &buf) {
    // Put your audio data into buffer
    // You can resize the buffer to any size you like
    // You can call player.drain() here to ensure audio had been played on device

    // Return true for valid data & call the callback once more.
    // Return false to end processing. 
});

SimplePA::Recorder r;
r.open();
r.run(
    [](SimplePA::Recorder &recorder, const std::vector<uint8_t> &buf) {
        // Recorded audio data are in buffer
    
        // Return true to call the callback once more.
        // Return false to end processing. 
    },
    1024 // Buffer size
);
```

You can run the above on another thread:
```cpp
SimplePA::Player p;
p.open();
std::thread t([&](){
    p.run([](SimplePA::Player &player, std::vector<uint8_t> &buf) {
        // ...
    });
});
```

## FAQ
#### Why don't you use the async interface offered by libpulse?
Who wants a C-style event loop implemented by `poll()` provided by an audio library to take control of your entire program?

Simply do audio operations on another thread and you are good to go. It's no problem in terms of performance.

## Documentation
TBD

## License
LGPLv3