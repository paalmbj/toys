#include <zlib.h>
#include <iostream>
#include <unordered_map>
#include <list>
#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

#define NUM_FIELDS 4

struct coord {
	int x;
	int y;
};

int main(int argc, char *argv[])
{
	gzFile fp;
	kseq_t *seq;
	int l;
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] << " <in.seq>\n" << std::endl;
		return 1;
	}

	std::unordered_map<std::string, std::list<coord> > sequences;
	fp = gzopen(argv[1], "r");
	seq = kseq_init(fp);
	long i=0;
	coord c;
	while ((l = kseq_read(seq)) >= 0 && i < 5000000L) {
		// Get coordinates from comment. After colon 2 and 3 (0 based).
		int colons[NUM_FIELDS];
		int p = 0, col = 0;
		for (; p < seq->comment.l && col < NUM_FIELDS; ++p) {
			if (seq->comment.s[p] == ':') {
				colons[col++] = p;
			}
		}
		if (col == NUM_FIELDS) {
			c.x = atoi(seq->comment.s+colons[2]+1);
			c.y = atoi(seq->comment.s+colons[3]+1);
			sequences[seq->seq.s].push_back(c);
		}

		if ((i & 0xFFFF) == 0) std::cout << "Loop #" << i << std::endl;
		++i;
	}
	std::cout << "Number of distinct sequences: " << sequences.size() << "\n";
	kseq_destroy(seq);
	gzclose(fp);
	return 0;
}

