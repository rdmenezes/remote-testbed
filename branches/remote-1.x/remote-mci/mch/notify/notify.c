#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int plugpipe;

int main(int argc, char* argv[])
{
	if (argc<1) return -1;
	// wait a bit before attempting to write
	usleep(5000000);
	plugpipe = open(argv[1],O_WRONLY | O_NONBLOCK);
	if ( plugpipe < 0 )
	{
		return -2;
	}

	if ( write(plugpipe,"p",1) < 1)
	{
		close(plugpipe);
		return -3;
	}
	close(plugpipe);
	return 0;
}
