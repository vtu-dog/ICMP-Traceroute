# ICMP-Traceroute
A traceroute utility utilizing the ICMP protocol

## Usage

In order to compile and run the program, type:
```
$ make
$ sudo ./traceroute A.B.C.D
```
where `A.B.C.D` is the desired IPv4 address.

Note that the program will only work on \*nix operating systems, because it makes use of raw sockets and not winsock. This is also the reason as to why you need administrator privileges - due to security precautions you need elevation to receive raw packets on an interface. It could be avoided by using the UDP protocol, but hey, my university's computer networks course required ICMP :P

If you want to get rid of the `.o` files afterwards, `make clean` is your friend.
In case you wanted to delete the binary as well, `make distclean` will leave only the source files.
