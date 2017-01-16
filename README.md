# World 311
A peer to peer virtual world

Warning: World 311 is still in early development and there's no reason to believe it's actually secure yet.
Use only with caution when real security is desired and check out VULNERABILITIES.md for know security flaws.

## Goal
The goal of this project is to create a peer to peer virtual world completely supported by users with no central servers.
We're currenly planning to use Kademlia for peer finding and integrate some form optional permission system to prevent spam.

## Name
The name World 311 arises from an extremely lengthy, complex, and interesting backstory we haven't bothered to write down yet (or come up with).

## Things to be done
(categorised by stack layer)

Connection
- Sockets - Done for TCP/IP
- Secure connections - Almost done
- Multiplexing

Protocol
- DHT (Kademlia)
- Object request
- Practical Byzantine Fault Tolerance
- Scripts (physics, interactivity)

World
- Object formats
- Stream formats

Interface
- Interface scripting (Lua frontend)
- GUI
