// See the file "COPYING" in the main distribution directory for copyright.

#ifndef INPUT_READERS_RAW_H
#define INPUT_READERS_RAW_H

#include <vector>

#include "../ReaderBackend.h"

namespace input { namespace reader {

/**
 * A reader that returns a file (or the output of a command) as a single
 * blob.
 */
class Raw : public ReaderBackend {
public:
	Raw(ReaderFrontend* frontend);
	~Raw();

	static ReaderBackend* Instantiate(ReaderFrontend* frontend) { return new Raw(frontend); }

protected:
	virtual bool DoInit(const ReaderInfo& info, int arg_num_fields, const threading::Field* const* fields);
	virtual void DoClose();
	virtual bool DoUpdate();
	virtual bool DoHeartbeat(double network_time, double current_time);

private:
	bool OpenInput();
	bool CloseInput();
	int64_t GetLine(FILE* file);
	bool Execute();
	void WriteToStdin();

	string fname; // Source with a potential "|" removed.
	FILE* file;
	bool execute;
	bool firstrun;
	time_t mtime;

	// options set from the script-level.
	string separator;
	unsigned int sep_length; // length of the separator

	static const int block_size;
	uint64_t bufpos;
	char* buf;
	char* outbuf;

	int stdin_fileno;
	int stdout_fileno;
	int stderr_fileno;

	string stdin_string;
	uint64_t stdin_towrite;

	int pipes[6];
	pid_t childpid;

	enum IoChannels {
		stdout_in = 0,
		stdout_out = 1,
		stdin_in = 2,
		stdin_out = 3,
		stderr_in = 4,
		stderr_out = 5
	};

};

}
}

#endif /* INPUT_READERS_RAW_H */
