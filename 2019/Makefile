CC = clang
CFLAGS = -Wall -std=c11

build_dir = build/
test_dir = 2019/test/
src_dir = 2019/src/

.PHONY: all
all: $(build_dir)test_day4 $(build_dir)day4 \
	 $(build_dir)test_day5 $(build_dir)day5

$(build_dir)test_day4: $(test_dir)unity.c $(test_dir)test_day4.c $(src_dir)day4.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

$(build_dir)day4: $(src_dir)day4.c $(src_dir)day4.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

$(build_dir)test_day5: $(test_dir)unity.c $(test_dir)test_day5.c $(src_dir)day5.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

$(build_dir)day5: $(src_dir)day5.c $(src_dir)day5.h
	$(CC) $(CFLAGS) -o $@ $(filter-out %.h,$^)

.PHONY: clean
clean:
	rm -r $(build_dir); \
	mkdir -p $(build_dir)

