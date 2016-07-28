# Overview of source files

| Filenames | Description |
|----------|-------------|
| network.h network.c | Wrappers around the Berkley socket interface so things can be switched out easily. |
| error.h error.c | For handling error messages. |
| proto.h proto.c | Stuff for handling the protocol such as DHT stuff. |
| handler.h handler.c | Handles incoming connections. |
