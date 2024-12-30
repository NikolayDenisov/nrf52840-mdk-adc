# nRF52840-MDK ADC Example

## Howto use

1. Clone this repo

```bash
git clone https://github.com/NikolayDenisov/nrf52840-mdk-adc.git
```

2. Open Makefile in armgcc directory

```bash
cd armgcc
vim Makefile
```

3. Change MDK_ROOT and SDK_ROOT directories. Paste yours values.

```
MDK_ROOT := /Users/nick/projects/iot/nrf52840-mdk/
SDK_ROOT := /Users/nick/projects/iot/nRF5_SDK_17.1.0_ddde560/
```

4. Build project

```bash
make
```

5. Connect nRF52840-MDK to PC
6. Flash microcontoller
```bash
make flash
```# nrf52840-mdk-adc
