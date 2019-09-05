FROM alpine:latest

# Install packages with APK.
RUN apk update && apk add \ 
    gcc \
    libc-dev \
    make

# Create a new system group and add huffman user.
RUN addgroup -S huffman_group && adduser -S huffman -G huffman_group

# Make huffman active user.
USER huffman

# Set working directory to be huffman user's home directory.
WORKDIR /home/huffman

# Copy source files.
COPY *.c *.h Makefile ./

# Compile the huffman program(s).
RUN make

# Remove source files.
RUN rm *.c *.h Makefile

# Launch a shell.
CMD ["/bin/sh"]
