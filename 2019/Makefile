CC = clang
CFLAGS = -Wall -std=c11

BUILD_DIR = build/
TEST_DIR = 2019/test/
SRC_DIR = 2019/src/

.PHONY: all
all: $(BUILD_DIR)test_day4 $(BUILD_DIR)day4

$(BUILD_DIR)test_day4: $(TEST_DIR)unity.c $(TEST_DIR)test_day4.c $(SRC_DIR)day4.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

$(BUILD_DIR)day4: $(SRC_DIR)day4.c $(SRC_DIR)day4.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR); \
	mkdir -p $(BUILD_DIR)

