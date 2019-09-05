FROM alpine:latest

# Create a new system group and add huffman user.
RUN addgroup -S huffman_group && adduser -S huffman -G huffman_group

# Set working directory to be huffman user's home directory.
WORKDIR /home/huffman

# Copy source files.
COPY *.c *.h Makefile ./

# Install packages, compile huffman program, then remove sources and packages.
RUN apk update \
    && apk --no-cache add --virtual .build-dependencies gcc libc-dev make \
    && make \
    && rm *.c *.h Makefile \
    && apk del .build-dependencies

# Make huffman active user.
USER huffman

# Launch a shell.
CMD ["/bin/sh"]
