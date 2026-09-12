FROM debian:latest
RUN apt-get update -y && apt-get install gcc -y && apt-get install make -y && apt-get install tcpdump -y
RUN apt-get install -y inetutils-ping
RUN apt-get install -y traceroute
RUN adduser bob
COPY app /app
