CC = clang
CFLAGS = -Wall -std=c11

build_dir = build/
test_dir = 2019/test/
src_dir = 2019/src/

.PHONY: all
all: $(build_dir)test_day4 $(build_dir)day4 \
	 $(build_dir)test_day5 $(build_dir)day5 \
	 $(build_dir)test_day6 $(build_dir)day6

$(build_dir)test_day%: $(test_dir)unity.c $(test_dir)test_day%.c $(src_dir)day%.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

$(build_dir)day%: $(src_dir)day%.c $(src_dir)day%.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

.PHONY: clean
clean:
	rm -r $(build_dir); \
	mkdir -p $(build_dir)

