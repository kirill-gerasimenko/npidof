// npidof.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "processes.h"

#include <iostream>

using namespace Processes;

void printUsage() {
	std::wcout << L"USAGE: npidof.exe <process name>\n";
}

int main(int argc, char* argv[], char* envp[]) {
	if (argc != 2) {
		printUsage();

		return -1;
	}

	wchar_t* processName = Common::convertToWchar(argv[1]);

	Snapshot snapshot = createSnapshot();
	if (snapshot->resultType == Result::Type::Error) {
		std::wcout << L"Failed to create processes snapshot: " <<  snapshot->error.message;

		free(processName);
		destroy(&snapshot);
		return -1;
	}
	
	ProcessesInfos processes = createFromSnapshot(snapshot);
	if (processes->resultType == Result::Type::Error) {
		std::wcout << L"Failed to get processes: " <<  snapshot->error.message;

		free(processName);
		destroy(&processes);
		destroy(&snapshot);

		return -1;
	}

	for (int i = 0; i < processes->success.value->used; i++) {
		ProcessInfo& pi = processes->success.value->items[i];
		if (_wcsicmp(pi.name, processName) == 0) {
			std::wcout << pi.pid << "\n";
		}
	}

	free(processName);
	destroy(&processes);
	destroy(&snapshot);

	return 0;
}

