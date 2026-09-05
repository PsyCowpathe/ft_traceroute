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