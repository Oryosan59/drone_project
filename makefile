# Makefile for the drone project

# --- Configuration ---
TARGET = main
SRCDIR = src
INCDIR = include
LIBDIR = /home/pi/navigator-lib/target/debug
LIBS = -lbluerobotics_navigator -lpthread
CC = g++
CFLAGS = -I$(INCDIR) -I$(LIBDIR) -L$(LIBDIR) $(LIBS) -Wall -std=c++11 -g # -g for debugging

# --- Source Files ---
MAIN_SRC = main.cpp
GAMEPAD_SRC = $(SRCDIR)/gamepad.cpp
PWM_CONTROL_SRC = $(SRCDIR)/pwm_control.cpp
SENSOR_MODULE_SRC = $(SRCDIR)/sensor_module.cpp
UDP_SERVER_SRC = udp_server/udp_server.cpp
UDP_CLIENT_SRC = udp_client/udp_client.cpp

# --- Object Files ---
MAIN_OBJ = $(MAIN_SRC:.cpp=.o)
GAMEPAD_OBJ = $(GAMEPAD_SRC:.cpp=.o)
PWM_CONTROL_OBJ = $(PWM_CONTROL_SRC:.cpp=.o)
SENSOR_MODULE_OBJ = $(SENSOR_MODULE_SRC:.cpp=.o)
UDP_SERVER_OBJ = $(UDP_SERVER_SRC:.cpp=.o)
UDP_CLIENT_OBJ = $(UDP_CLIENT_SRC:.cpp=.o)

# --- All Object Files ---
OBJ = $(MAIN_OBJ) $(GAMEPAD_OBJ) $(PWM_CONTROL_OBJ) $(SENSOR_MODULE_OBJ) $(UDP_SERVER_OBJ) $(UDP_CLIENT_OBJ)

# --- Rules ---

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# --- Clean ---

clean:
	rm -f $(OBJ) $(TARGET)

# --- Phony Targets ---
.PHONY: all clean
