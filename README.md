Simple utility that approximately calculates the time between two calendar dates in units of time.\
The library works with dates ranging from 0000 to 9999, and as of now only supports the DDMMYY format.

Only depends on the C standard library, requiring a compiler supporting >C89.

## Usage
```console
timemath [DD/MM/YY] [DD/MM/YY]
```
## Examples
```console
$ timemath 03/10/2024 04/10/2024

second:  86400.0
minutes: 1440.0
hours:   24.0
days:    1.0
weeks:   0.1
months:  0.0
years:   0.0
```
```console
$ timemath 01/09/1939 02/09/1945

second:  189326400.0
minutes: 3155440.0
hours:   52590.7
days:    2191.3
weeks:   313.0
months:  72.0
years:   6.0
```