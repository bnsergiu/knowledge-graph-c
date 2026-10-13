CC = cc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700
CPPFLAGS = -Iinclude
BUILD_DIR := build
COMMON_SRCS := src/entity.c src/graph.c src/bst.c src/queue.c src/heap.c src/query.c
HEADERS := $(wildcard include/*.h)
PUBLIC_BINS := $(addprefix $(BUILD_DIR)/public_pas,1 2 3 4)

.PHONY: all clean test public-test regression-test

all: $(BUILD_DIR)/knowledge_graph

$(BUILD_DIR)/knowledge_graph $(BUILD_DIR)/student_kg: src/main.c $(COMMON_SRCS) $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.c,$^) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/public_pas%: tests/public/harness/pas%_harness.c $(COMMON_SRCS) src/main.c $(HEADERS) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter-out src/main.c,$(filter %.c,$^)) $(LDFLAGS) $(LDLIBS) -o $@

test: public-test regression-test

public-test: $(PUBLIC_BINS)
	sh tests/public/run_public_tests.sh

regression-test: $(BUILD_DIR)/knowledge_graph
	sh tests/regression/run_growth_test.sh

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
