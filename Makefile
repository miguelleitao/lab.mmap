
TARGETS=mmap_cat mmap_cp

all: ${TARGETS}

%: %.c
	cc -Wall -o $@ $^

clean:
	rm -rf ${TARGETS}
