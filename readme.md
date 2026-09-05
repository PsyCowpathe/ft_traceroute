# ft_traceroute

A clone of traceroute written in C.

## Options

    --help: Show the help menu.

## Bonus options

    -m: Maximum TTL (default is 30)
    -f: Start from the Nth hop (default is 1)
    -q: Number of probes sent per TTL
    -p: Starting port
    -rdns: Resolve IP addresses to domain names

## Unit tests

A unitTests.sh script is available to automatically compare the output with the official traceroute:
    sudo ./unitTests.sh

### Important: This program requires sudo privileges to create sockets.






















Tests :

traceroute --> afiche le help menu
traceroute "" // OK
traceroute -m --> erreur manque le host // ok
traceroute --g // ok
traceroute --m // ok
traceroute ---m 8.8.8.8 -> ---m est considéré comme le host et affiche une erreur // ok
traceroute 0000a0 //ok
traceroute 60 8.8.8.8 //ok
traceroute -8.8.8.8 //ok

traceroute -4 -m -1 8.8.8.8 //ok
traceroute -4 -m 256 8.8.8.8 //ok
traceroute -4 -m -1 8.8.8.8 //ok
traceroute -4 -m 256 8.8.8.8 //ok

traceroute -f 256 8.8.8.8 //ok
traceroute -f 255 8.8.8.8 //ok
traceroute -f 300 8.8.8.8 //ok
traceroute -f -1 8.8.8.8 //ok

traceroute -q -1 8.8.8.8 //ok
traceroute -q 255 8.8.8.8 //ok

traceroute 8.8.8.8 -1 //ok
traceroute 8.8.8.8 60.1 // ok
traceroute -f 25 8.8.8.8 //ok
traceroute 8.8.8.8 65001 //ok

traceroute -m 50 -f 35 8.8.8.8

work :
traceroute 8.8.8.8
traceroute 8.8.8.8 60
traceroute -f 5 8.8.8.8
traceroute 8.8.8.8 65000