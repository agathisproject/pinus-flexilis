# pinus-flexilis

Agathis Management Controller CLI specification.

### assumptions

 - the CLI is normally accessed by connecting a USB cable to the USB service port
   - later on, we may add provisions for this CLI to be accessed from a data link port to support remote servicing
 - the user of this interface has a service profile (can change hardware configuration, no access to data links)
   - the first service user (default service user) is the test engineer guy on the production line of the module
 - the default service user does not need any credentials to get the MC (TMC or MMC) to answer
 - the default service user credentials can be changed in the field by an admin (first customer) accessing the Agathis system/stack from a data link

### service commands

- after USB connection, the MC responds with the prompt line for the default service user or *password?* for normal service user
- after the normal service user is validated, the prompt line is displayed
- the prompt line displays the available commands for the current level
- the commands have a hierarchical structure - use *up* command to go up one level

#### 1.1

LML (layman language) command: who are you?

LML response: manufacturer is <this>, manufacturer part number is <this>, manufacturer serial number is <this>, manufacturer other data (specific) is <this>

- CLI command:

```
? tfun dev mod> ?
```

- CLI response:

```
up >

dbg: <any debug/developer help info line starts with "dbg:">

MC_TYPE = <MMC or TMC>
I2C_ADDR = <I2C address>
MFR_NAME = <manufacturer name>
MFR_PN = <manufacturer part number>
MFR_SN = <manufacturer serial number>
MFR_DATA = <manufacturer optional data>
```

#### 1.2 TRUNK FUNCTIONS

LML command: what are your trunk functions?

LML response: list trunk functions (to allow further queries for details)

- CLI command:

```
? tfun dev mod> tfun
```

- CLI response:

```
up PWR MC JTAG CLK 1PPS USB BLVDS P1X P16X>
```

#### 1.2.1 POWER SETTINGS

LML command: what power do you do?

LML response: V5 rail and V3 rail specs and status
 
- CLI command:

```
up PWR MC JTAG CLK 1PPS USB BLVDS P1X P16X> PWR
```

- CLI response:

```
up V5_SOURCE_ON V5_SOURCE_OFF V5_LOAD_ON V5_LOAD_OFF>
 
SOURCE I5NOM = <V5 rail nominal source current for[A]>
SOURCE I5CUTOFF = <V5 rail power switch protection current [A]>
LOAD I5NOM = <V5 rail nominal load current for V5 rail [A]>
LOAD I5CUTOFF = <V5 rail load switch protection current [A]>
SOURCE I3NOM = <V3 rail nominal source current [mA]>
SOURCE I3CUTOFF = <V5 rail power switch protection current [mA]>
LOAD I3NOM = <V3 rail nominal load current for V5 rail [mA]>

V5S = ON | OFF (this is the power switch status)
V5L = ON | OFF (this is the load switch status)
V5 CURRENT = <current value [A]> SOURCED | LOADED
V3 CURRENT = <current value [A]> SOURCED | LOADED
```

#### 1.2.1.1 POWER SWITCHES CONTROL

LML command: turn V5 rail power/load switch on/off

LML response: done

- CLI command:

```
up V5_SOURCE_ON V5_SOURCE_OFF V5_LOAD_ON V5_LOAD_OFF> V5_SOURCE_ON | V5_SOURCE_ON | V5_SOURCE_OFF | V5_LOAD_ON | V5_LOAD_OFF
```
- CLI response:

```
up V5_SOURCE_ON V5_SOURCE_OFF V5_LOAD_ON V5_LOAD_OFF>
 
SOURCE I5NOM = <V5 rail nominal source current for[A]>
SOURCE I5CUTOFF = <V5 rail power switch protection current [A]>
LOAD I5NOM = <V5 rail nominal load current for V5 rail [A]>
LOAD I5CUTOFF = <V5 rail load switch protection current [A]>
SOURCE I3NOM = <V3 rail nominal source current [mA]>
SOURCE I3CUTOFF = <V5 rail power switch protection current [mA]>
LOAD I3NOM = <V3 rail nominal load current for V5 rail [mA]>

V5S = ON | OFF (this is the power switch status)
V5L = ON | OFF (this is the load switch status)
V5 CURRENT = <current value [A]> SOURCED | LOADED
V3 CURRENT = <current value [A]> SOURCED | LOADED
```

#### 1.2.2 MGMT CONTROLLER
 
#### 1.2.3 JTAG

#### 1.2.4 CLK

#### 1.2.5 1PPS

#### 1.2.6 USB

#### 1.2.7 BLVDS
 
#### 1.2.8 P1X

#### 1.2.9 P16X

#### 1.3 DEV

LML command: what devices do you control?
LML response: list devices

- CLI command:
 
```
? tfun dev mod> dev
```

- CLI response: 

```
up dev1 dev2>
``` 

#### 1.4 MOD

LML command: what modules do you control?
LML response: list modules

- CLI command:
 
```
? tfun dev mod> mod
```

- CLI response: 

```
up mod1 mod2>
``` 
