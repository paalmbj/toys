#include <zlib.h>
#include <iostream>
#include <unordered_map>
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

int main(int argc, char *argv[])
{
	gzFile fp;
	kseq_t *seq;
	int l;
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] << " <in.seq>\n" << std::endl;
		return 1;
	}

	std::unordered_map<std::string, int> counter;

	fp = gzopen(argv[1], "r");
	seq = kseq_init(fp);
	long i=0;
	while ((l = kseq_read(seq)) >= 0 && i < 20000000L) {
		//std::cout << "name: " << seq->name.s << "\n" << std::endl;
		//if (seq->comment.l) printf("comment: %s\n", seq->comment.s);
		//printf("seq: %s\n", seq->seq.s);
		counter[seq->seq.s]++;
		//if (seq->qual.l) printf("qual: %s\n", seq->qual.s);
		if ((i & 0xFFFF) == 0) std::cout << "Loop #" << i << std::endl;
		++i;
	}
	std::cout << "Number of distinct sequences: " << counter.size() << "\n";
	kseq_destroy(seq);
	gzclose(fp);
	return 0;
}



