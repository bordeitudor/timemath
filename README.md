Simple utility that approximately calculates the time between two calendar dates in units of time.\
The library works with dates starting from 0, and as of now supports all commonly used date formats.

The only dependency is the C standard library, requiring a compiler supporting C11.

## Usage
```console
timemath <date> <date> [options]
```
## Examples
```console
$ timemath 10/03/2024 12/02/2024 -format MMDDYYYY

seconds:  2246400
minutes: 37440.0
hours:   624.0
days:    26.0
weeks:   3.7
months:  0.9
years:   0.1
```
```console
$ timemath 01/09/1939 02/09/1945 -format DDMMYYYY

seconds:  189432000
minutes: 3157200.0
hours:   52620.0
days:    2192.5
weeks:   313.2
months:  72.1
years:   6.0
```
