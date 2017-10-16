#pragma once

#include "stdafx.h"
#include "result.h"
#include "array.h"

namespace Processes {
	// processes snapshot
	typedef Result::T<HANDLE>* Snapshot;

	Snapshot createSnapshot() {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		return hSnapshot == INVALID_HANDLE_VALUE 
			? Result::error<HANDLE>(GetLastError()) 
			: Result::ok(hSnapshot);
	}

	void destroy(Snapshot* snapshot) {
		if ((*snapshot)->resultType == Result::Type::Success)
			CloseHandle((*snapshot)->success.value);

		Result::destroy(snapshot);
	}

	// process info
	struct ProcessInfo { 
		DWORD pid; 
		wchar_t* name; 
	};

	typedef Result::T<Array::T<ProcessInfo>*>* ProcessesInfos;

	typedef ProcessInfo (*mapFunc)(PROCESSENTRY32&);
	typedef ProcessesInfos (*errorFunc)(DWORD);
	typedef ProcessesInfos (*okFunc)(Array::T<ProcessInfo>*);

	errorFunc _error = &Result::error<Array::T<ProcessInfo>*>;
	okFunc _ok = &Result::ok<Array::T<ProcessInfo>*>;

	ProcessesInfos _createFromSnapshot(HANDLE processSnapshot, mapFunc mapper) {
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(processSnapshot, &pe32))
			return _error(GetLastError());

		Array::T<ProcessInfo>* entries = Array::create<ProcessInfo>(100);

		do {
			ProcessInfo entry = mapper(pe32);
			Array::push<ProcessInfo>(entries, entry);
		} while (Process32Next(processSnapshot, &pe32));

		return _ok(entries);
	}

	ProcessInfo _map(PROCESSENTRY32& entry) {
		size_t len = wcslen(entry.szExeFile) + 1;
		wchar_t* pname = Common::createArray<wchar_t>(len);
		wcscpy_s(pname, len, entry.szExeFile);

		return { entry.th32ProcessID, pname }; 
	}

	ProcessesInfos createFromSnapshot(Snapshot snapshot) {
		return snapshot->resultType == Result::Type::Success
			? _createFromSnapshot(snapshot->success.value, _map)
			: _error(snapshot->error.code);
	}

	void destroy(ProcessesInfos* entries) {
		if ((*entries)->resultType == Result::Type::Success) {
			for (int i = 0; i < (*entries)->success.value->used; i++) {
				free((*entries)->success.value->items[i].name);
			}

			Array::destroy(&(*entries)->success.value);
		}

		Result::destroy(entries);
	}
}
