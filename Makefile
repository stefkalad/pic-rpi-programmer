#
# Copyright (C) 2005-2020 Darron Broad
# All rights reserved.
#
# This file is part of Pickle Microchip PIC ICSP.
#
# Pickle Microchip PIC ICSP is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as published
# by the Free Software Foundation.
#
# Pickle Microchip PIC ICSP is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with Pickle Microchip PIC ICSP. If not, see http://www.gnu.org/licenses/
#

# build targets and i/o configuration, see config directory
include config.mk

CC=gcc
CFLAGS=-Os -std=gnu99 -pedantic-errors -I. -Wall
CFLAGS+=-D__USE_LINUX_IOCTL_DEFS
LDFLAGS=
OUT_DIR=out
SRC_DIR=src
OUT_BIN_DIR=out/bin

BINDIR=/usr/local/bin

ifeq "$(GDB)" "1"
CFLAGS+=-ggdb
endif

ifeq "$(VERBOSE)" "1"
CFLAGS+=-DVERBOSE
endif

ifeq "$(LOADER)" "1"
CFLAGS+=-DLOADER
endif



TARGET1=$(OUT_BIN_DIR)/pickle
SOURCE1=pickle.c dotconf.c inhx32.c io.c pic.c util.c

TARGET2=$(OUT_BIN_DIR)/pio
SOURCE2=pio.c icspio.c dotconf.c io.c util.c

TARGET3=$(OUT_BIN_DIR)/ptest
SOURCE3=ptest.c icspio.c dotconf.c io.c util.c

TARGET4=$(OUT_BIN_DIR)/pctrl
SOURCE4=pctrl.c dotconf.c io.c util.c

# CLIB5=$(CLIB)
# TARGET5=pload
# SOURCE5=pload.c dotconf.c inhx32.c io.c util.c stk500v2.c stk500v2_load.c ip.c rw.c serial_posix.c

ifeq "$(DRYRUN)" "1"
CFLAGS+=-DDRYRUN
endif

ifeq "$(P12)" "1"
CFLAGS+=-DP12
SOURCE1+=pic12.c
endif

ifeq "$(P14)" "1"
CFLAGS+=-DP14
SOURCE1+=pic14.c
endif

ifeq "$(N14)" "1"
CFLAGS+=-DN14
SOURCE1+=pic14n.c
endif

ifeq "$(P16)" "1"
CFLAGS+=-DP16
SOURCE1+=pic16.c
endif

ifeq "$(N16)" "1"
CFLAGS+=-DN16
SOURCE1+=pic16n.c
endif

ifeq "$(P24)" "1"
CFLAGS+=-DP24
SOURCE1+=pic24.c
endif

ifeq "$(P32)" "1"
CFLAGS+=-DP32
SOURCE1+=pic32.c
endif

ifeq "$(PCTRL)" "1"
CFLAGS+=-DPCTRL
endif

ifeq "$(PTEST)" "1"
CFLAGS+=-DPTEST
endif

ifeq "$(PIO)" "1"
CFLAGS+=-DPIO
endif

# ifeq "$(PLOAD)" "1"
# CFLAGS+=-DPLOAD
# endif

ifeq "$(PICKLE_RPI)" "1"
 SOURCE1+=raspi.c
 SOURCE2+=raspi.c
 SOURCE3+=raspi.c
 SOURCE4+=raspi.c
#  SOURCE5+=raspi.c
endif


ifeq "$(QSORT)" "1"
CFLAGS+=-DQSORT
endif

OBJECT1=$(patsubst %, $(OUT_DIR)/%.o, $(basename $(SOURCE1)))
HEADER1=$(patsubst %, $(SRC_DIR)/%.h, $(basename $(SOURCE1)))


OBJECT2=$(SOURCE2:.c=.o)
HEADER2=$(SOURCE2:.c=.h)

OBJECT3=$(SOURCE3:.c=.o)
HEADER3=$(SOURCE3:.c=.h)

OBJECT4=$(SOURCE4:.c=.o)
HEADER4=$(SOURCE4:.c=.h)

# OBJECT5=$(SOURCE5:.c=.o)
# HEADER5=$(SOURCE5:.c=.h)



#
# COMMAND LINE TOOL(S)
#
TARGETS:=$(TARGET1)
ifeq "$(PIO)" "1"
TARGETS:=$(TARGETS) $(TARGET2)
endif
ifeq "$(PTEST)" "1"
TARGETS:=$(TARGETS) $(TARGET3)
endif
ifeq "$(PCTRL)" "1"
TARGETS:=$(TARGETS) $(TARGET4)
endif
# ifeq "$(PLOAD)" "1"
# TARGETS:=$(TARGETS) $(TARGET5)
# endif

build:$(TARGET1)



$(TARGET1): $(OBJECT1) | $(OUT_BIN_DIR)
	@echo -n "[LINK] "
	$(CC) $(LDFLAGS) $^ -o $@


# $(OBJECT1):$(HEADER1) Makefile config.mk VERSION


$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	@echo -n "[CC] "
	$(CC) $(CFLAGS) -c $< -o $@


$(OUT_BIN_DIR):
	mkdir -p $(OUT_BIN_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)



# $(TARGET1):$(OBJECT1)
# 	@echo -n "[LINK] "
# 	$(CC) $(CFLAGS) $(OBJECT1) -o $(TARGET1) $(CLIB1) -Wl,-Map,$(TARGET1).map


# $(OBJECT1):$(HEADER1) Makefile config.mk VERSION

# $(TARGET2):$(OBJECT2)
# 	@echo -n "[LINK] "
# 	$(CC) $(CFLAGS) $(OBJECT2) -o $(TARGET2) $(CLIB2) -Wl,-Map,$(TARGET2).map


# $(OBJECT2):$(HEADER2) Makefile config.mk VERSION pickle.h

# $(TARGET3):$(OBJECT3)
# 	@echo -n "[LINK] "
# 	$(CC) $(CFLAGS) $(OBJECT3) -o $(TARGET3) $(CLIB3) -Wl,-Map,$(TARGET3).map


# $(OBJECT3):$(HEADER3) Makefile config.mk VERSION pickle.h

# $(TARGET4):$(OBJECT4)
# 	@echo -n "[LINK] "
# 	$(CC) $(CFLAGS) $(OBJECT4) -o $(TARGET4) $(CLIB4) -Wl,-Map,$(TARGET4).map


# $(OBJECT4):$(HEADER4) Makefile config.mk VERSION pickle.h

# $(TARGET5):$(OBJECT5)
# 	@echo -n "[LINK] "
# 	$(CC) $(CFLAGS) $(OBJECT5) -o $(TARGET5) $(CLIB5) -Wl,-Map,$(TARGET5).map


# $(OBJECT5):$(HEADER5) Makefile config.mk VERSION pickle.h

install:build
	mkdir -p $(BINDIR)
	cp $(TARGET1) $(BINDIR)/$(TARGET1)
ifeq "$(P12)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p12
endif
ifeq "$(P14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p14
endif
ifeq "$(N14)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/n14
endif
ifeq "$(P16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p16
endif
ifeq "$(N16)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/n16
endif
ifeq "$(P24)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p24
endif
ifeq "$(P32)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/p32
endif
ifeq "$(PIO)" "1"
	cp $(TARGET2) $(BINDIR)/$(TARGET2)
endif
ifeq "$(PTEST)" "1"
	cp $(BINDIR)/$(TARGET1) $(BINDIR)/ptest
endif
ifeq "$(PCTRL)" "1"
	cp $(BINDIR)/$(TARGET1) $(BINDIR)/pctrl
endif
ifeq "$(TTY)" "1"
	ln -sf $(BINDIR)/$(TARGET1) $(BINDIR)/pload
endif

clean:
	cd pickle-conf && $(MAKE) clean
	rm -f out/*
