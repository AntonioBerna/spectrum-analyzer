# spectrum-analyzer

![GitHub License](https://img.shields.io/github/license/antonioberna/spectrum-analyzer)
![GitHub repo size](https://img.shields.io/github/repo-size/antonioberna/spectrum-analyzer)
![GitHub Created At](https://img.shields.io/github/created-at/antonioberna/spectrum-analyzer)

## Demo

https://github.com/user-attachments/assets/eb3b91cd-cf6a-4602-b6fd-9f211323806a

## Requirements

The project requires the following components:

- ESP32 DevKit V1
- OLED Display 128x64
- KY-037 Microphone with LM393 Comparator

The following is the schematic of the project:

<p align="center">
    <img src=".github/imgs/schema-di-montaggio.png" width="800" />
</p>

> [!NOTE]
> You can find the Fritzing project file in the `.github/fritzing/` directory.

> [!WARNING]
> You need to have the [PlatformIO](https://platformio.org/install/ide?install=vscode) extension installed on your Visual Studio Code.

## Mini docs

If you want to use the project, you need to clone the repository with the following command:

```
git clone https://github.com/AntonioBerna/spectrum-analyzer.git
```

Then you need to open the project with Visual Studio Code and upload the code to the ESP32 DevKit V1.
