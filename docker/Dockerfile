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

# Copy the entire project except for ignored files
COPY . .

# Build the project: Clean and compile in the ./test directory
RUN cd ./test && make clean && make

# Run the compiled binary with Valgrind
CMD ["valgrind", "--leak-check=full", "--show-leak-kinds=all", "./bin/arisr_test"]
