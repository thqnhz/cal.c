# Cal.c

Calculator in C

The app is still a WIP. ***No real features are implemented!!***

## Table of contents

1. [Demo](#demo)
2. [Build from source](#build-from-source)
3. [Third-party libraries](#third-party-libraries)
4. [License](#license)

## Demo

Screen shot (Linux build)

![Screenshot](assets/sceenshots/app.png)

## Build from source

> [!NOTE]
> Requires `gcc` and `make` installed.

1. Clone the project

```bash
$ git clone https://github.com/thqnhz/cal.c.git
$ cd cal.c
```

2. Bootstrap nob.h (Only need to do this once)

```bash
$ gcc -o nob nob.c
```

3. Run the app

```bash
$ ./build/Calc
```

## Third-party libraries

Build system: [Nob](https://github.com/tsoding/nob.h)
Rendering: [Raylib](https://github.com/raysan5/raylib)
Font: [Rubik](https://fonts.google.com/specimen/Rubik)

All licenses belong to their respective owners

## License

This project is licensed under the [MIT](LICENSE) license.

