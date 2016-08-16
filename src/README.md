# Overview of source files

| Filenames | Description |
|----------|-------------|
| draw.h draw.c | For drawing things to the screen. |
| error.h error.c | For handling error messages. |
| handler.h handler.c | Handles incoming connections. |
| network.h network.c | Wrappers around the Berkley socket interface so things can be switched out easily. |
| proto.h proto.c | Stuff for handling the protocol such as DHT stuff. |
| secure.h secure.c | For making and receiving secure connections. |
