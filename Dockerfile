FROM gcc

RUN mkdir /work /work/bin

WORKDIR /work
COPY *.c Makefile /work/

RUN env HOME=/work make

RUN cp -r /work/bin /app
