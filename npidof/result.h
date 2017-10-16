#pragma once

#include "stdafx.h"
#include "common.h"

namespace Result { 
	enum class Type { Error, Success };

	template <typename TValue>
	struct SuccessResult {
		TValue value;
	};

	struct ErrorResult {
		wchar_t* message;
		DWORD code;
	};

	template <typename TValue>
	struct T {
		Type resultType;
		union {
			SuccessResult<TValue> success;
			ErrorResult error;
		};
	};

	template <typename TValue>
	T<TValue>* ok(TValue value) {
		T<TValue>* t = Common::createEmpty<T<TValue>>();

		t->resultType = Type::Success;
		t->success = SuccessResult<TValue> { value };

		return t;
	}

	template <typename TValue>
	T<TValue>* error(DWORD code) {
		T<TValue>* t = Common::createEmpty<T<TValue>>();
		wchar_t* msg = Common::createArray<wchar_t>(1025);

		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, NULL, msg, 1025, NULL);

		t->resultType = Type::Error;
		t->error = ErrorResult{ msg, code };

		return t;
	}

	template <typename TValue>
	void destroy(T<TValue>** t) {
		if ((*t)->resultType == Type::Error && (*t)->error.message != NULL)
			free((*t)->error.message);

		free(*t);
		*t = NULL;
	}
}

