# Use an official Debian-based image
FROM debian:latest

# Install required packages: GCC, Valgrind, and Make
RUN apt-get update && apt-get install -y \
    gcc \
    valgrind \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside the container
WORKDIR /app

# Ensure the tmp directory exists for Valgrind logs
RUN mkdir -p /app/tmp

# Copy the entrypoint script and give it execution permissions
COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Set the entrypoint
ENTRYPOINT ["/bin/bash", "/entrypoint.sh"]
