# pti-projects

[![Build](/../../actions/workflows/main.yml/badge.svg)](/../../actions/workflows/main.yml)

Projects using **[pti](https://github.com/mathewmariani/pti)**

## Repository Layout

*   `build/` - Intermediate files and other build output. 
*   `extra/` - Files used during the build process.
*   `libs/` - Required external libraries.
*   `projects/` - Root directory for all projects.
*   `resources/` – Scripts and resources used for building the website.


## How to Build on Windows, Linux and macOS

```
> git clone https://github.com/mathewmariani/pti-projects
> cd pti-projects
> cmake --preset Release
> cmake --build --preset Release
```

## License

This project is free software; you can redistribute it and/or modify it under
the terms of the MIT license.
See [LICENSE](LICENSE) for details.