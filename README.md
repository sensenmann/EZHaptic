![Logo](https://github.com/sensenmann/EZHaptic/blob/main/doc/EZHaptic_small.png?raw=true)

[![GitHub Stars](https://img.shields.io/github/stars/sensenmann/EZHaptic?style=social)](https://github.com/sensenmann/EZHaptic/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/sensenmann/EZHaptic?style=social)](https://github.com/sensenmann/EZHaptic/network/members)
![License](https://img.shields.io/github/license/sensenmann/EZHaptic)
![Platform](https://img.shields.io/badge/platform-ESP32-orange)
![Language](https://img.shields.io/github/languages/top/sensenmann/EZHaptic)
![Last Commit](https://img.shields.io/github/last-commit/sensenmann/EZHaptic)

# EZHaptic
An easy-to-use vibration control library for ESP32 using native hardware timers and PWM.

## Table of Contents
- [Features](#features)
- [Quickstart](#quickstart)
- [Usage](#usage)
- [Pattern Language](#pattern-language)
- [Predefined Patterns](#predefined-patterns)


## Features
- Control vibration motors using ESP32's native hardware timers
- Pattern-based vibration control with 125ms intervals
- PWM support for different vibration intensities
- No external libraries required
- Clean object-oriented C++ design
- Easy integration into existing projects

## Quickstart

```cpp
#include "EZHaptic.h"

EZHaptic haptic(5); // GPIO 5 for motor output

void setup() {
    haptic.begin();
    haptic.play("x__o_.. x__o_.. !");
}

void loop() {
    // Nothing to do – runs independently using a hardware timer
}
```

## Usage

### Constructor

```cpp
EZHaptic(uint8_t motorPin, timer_group_t group = TIMER_GROUP_0, timer_idx_t idx = TIMER_0);
```

- `motorPin`: GPIO pin for the motor
- `group`: Timer group (default `TIMER_GROUP_0`)
- `idx`: Timer index (default `TIMER_0`)

### Public Methods

| Method                                                 | Description                                          |
|--------------------------------------------------------|------------------------------------------------------|
| `begin`                                                | Initializes PWM and timer                            |
| `play`                                                 | Starts pattern execution                             |
| `playRepeated`                                         | Starts pattern execution with given repetition count |
| `stop`                                                 | Stops current pattern                                |

## Pattern Language

Each pattern is a `String`, where each character is processed every 125ms:

| Character | Action                                   |
|----------|------------------------------------------|
| `x` | Full power vibration                     |
| `o` | Medium power vibration                   |
| `.` | Light vibration                          |
| `_` | Motor off                                |
| `!` | Immediately stops the pattern (optional) |

You can define and play your own vibration sequences using combinations of these characters.

## Predefined Patterns

```cpp
const static String BzzBzz = "xxx.xx..";
const static String LongRing = "xxxxxxxx        xxxxxxxx        ";
```

Use them directly:

```cpp
haptic.play(BzzBzz);
```

