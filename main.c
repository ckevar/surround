#include "capture.h"

int main(int argc, char const *argv[])
{
	loadCapSettings("hw:2,1", 44100, 2);
	return 0;
}