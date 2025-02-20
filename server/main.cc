#include "server.h"
#include "shared.h"

int main()
{
	runServer();

	Shared &instance = Shared::Instance();
	instance.~Shared();
	return 0;
}