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

#### 1.1

LML (layman language) command: who are you?

LML response: manufacturer is <this>, manufacturer part number is <this>, manufacturer serial number is <this>, manufacturer other data (specific) is <this>

- CLI prompt before command:

```
? dev mod>
```

- CLI command:

```
?
```

- CLI response:

```
MFR_NAME,MFR_PN,MFR_SN,MFR_DATA
```

- CLI prompt after command:

```
? dev mod>
```

#### 1.2

LML command: what (local) devices do you control?

LML response: list devices

- CLI prompt before command:

```
? dev mod>
```

- CLI command:

```
dev
```

- CLI response:

```
dev1
dev2
```

- CLI prompt after response:

```
up dev1 dev2>
```

#### 1.3

LML command: go to upper level

LML response: ok, I go to upper level

- CLI prompt before command:

```
up dev1 dev2>
```

- CLI command:

```
up
```

- CLI response:

- CLI prompt after response:

```
? dev mod>
```

#### 1.4

LML command: what modules do you control?
LML response: list modules

- CLI prompt before command:

```
? dev mod>
```

- CLI command:

```
mod
```

- CLI response:

```
mod1
mod2
```

- CLI prompt after response:

```
up mod1 mod2>
```
