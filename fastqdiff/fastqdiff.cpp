#include <iostream>
#include <string>
#include <zlib.h>
#include <cmath>
#include <cstdint>

using namespace std;

#define BUFFER_SIZE 512 * 1024
#define CHUNK BUFFER_SIZE // Defined by zlib

const size_t SEQUENCE_TO_QUALITY_LEN = string("\n+\n").size();

class NucleotideValueFunctor
{
	uint64_t operator()(char input) {
		if (input == 'N') return 0;
		else if (input == 'A') return 1;
		else if (input == 'C') return 2;
		else if (input == 'G') return 3;
		else if (input == 'T') return 4;
		else return 0;
	}

	char symbol(uint64_t val) {
		if (input == 0) return 'N';
		else if (input == 1) return 'A';
		else if (input == 2) return 'C';
		else if (input == 3) return 'G';
		else if (input == 4) return 'T';
		else return 0;
	}
};

class QualityScoreValueFunctor
{
	uint64_t operator()(char input) {
		return 33 - input;
	}
	char val(uint64_t input) {
		return 33 + input;
	}
};

template<int BASE, typename VALUE_FNC, int LENGTH>
class Sequence
{
private:
	static const int N_PER_VALUE = 8*sizeof(uint64_t) / log2(BASE);
	uint64_t data[(LENGTH + N_PER_VALUE - 1) / N_PER_VALUE];

public:
	Sequence(const char* seq) 
	{
		int idata, i;
		for (i=0, idata = 0; i<LENGTH; ++idata)
		{
			uint64_t val = 0;
			int j;
			for (j=0; i<LENGTH && j<N_PER_VALUE; ++i, ++j) {
				val *= BASE;
				val += VALUE_FNC(seq[i]);
			}	
			data[idata] = val;
		}
	}
};

typedef Sequence<5, BaseValueFunctor, 350> BaseSequence ;



struct SequenceReadParameters
{
	bool error;
	unsigned int sequenceStart;
	unsigned int sequenceLength;
	unsigned int qualityStart;
	unsigned int recordLength;
};


SequenceReadParameters GetParameters(const char* buffer, size_t len) {
	string rec = string(buffer, len);
	SequenceReadParameters result;
	size_t header_end=rec.find("\n");
	if (header_end == string::npos) {
		result.error = true;
		return result;
	}
	result.sequenceStart = header_end+1;
	size_t seq_end=rec.find("\n", header_end+1);
	if (seq_end == string::npos) {
		result.error = true;
		return result;
	}
	result.sequenceLength = seq_end - (header_end+1);
	result.qualityStart = result.sequenceStart + result.sequenceLength + SEQUENCE_TO_QUALITY_LEN;
	result.recordLength = result.qualityStart + result.sequenceLength + 1;
	result.error = result.recordLength > len;
	return result;
}



int main(int argc, char** argv)
{
	// ** Initialization **
	if (argc != 3)
	{
		cerr << "use: fastqdiff file1 file2" << endl;
		return 1;
	}

	char buffer[BUFFER_SIZE];
	gzFile fq[2];
	SequenceReadParameters par[2];
	int i, len;
	for (i=0; i<2; ++i) {
		if ((fq[i] = gzopen(argv[i+1], "rb")) == NULL) 
		{
			cerr << "fastqdiff: failed to open " << argv[i+1] << endl;
			return 1;
		}
		//gzbuffer(fq[i], BUFFER_SIZE);
		if ((len = gzread(fq[i], buffer, BUFFER_SIZE)) <= 0) 
		{
			cerr << "fastqdiff: failed to read from " << argv[i+1] << endl;
			return 1;
		}
		par[i] = GetParameters(buffer, len);
		if (par[i].error) {
			cerr << "fastqdiff: file " << argv[i+1] << " has invalid format" << endl;
			return 1;
		}
		gzseek(fq[i], 0, SEEK_SET);
		//cerr << "File " << argv[i+1] << " has sequence length " << par[i].sequenceLength << endl;
	}

	// ** Main part **
	//

	// Process files
	for (i=0; i<2; ++i) 
	{
		// Use a negative increment on second file to get "differential expression"
		const int increment = (i == 0) ? 1 : -1;
		
			

	}
	

	return 0;
}

