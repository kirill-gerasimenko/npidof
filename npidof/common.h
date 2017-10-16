#pragma once

namespace Common {
	template <typename T>
	T* createEmpty() {
		T* t = (T*)malloc(sizeof(T));
		memset(t, 0, sizeof(T));
		return t;
	}

	template <typename T>
	T* createArray(size_t count, bool empty = false) {
		size_t realSize = count * sizeof(T);
		T* t = (T*)malloc(realSize);
		if (empty) {
			memset(t, 0, realSize);
		}
		return t;
	}

	wchar_t* convertToWchar(const char* input) {
		size_t len = strlen(input) + 1;
		wchar_t* output = Common::createArray<wchar_t>(len);
		size_t res;
		mbstowcs_s(&res, output, len, input, len);
		return output;
	}
}