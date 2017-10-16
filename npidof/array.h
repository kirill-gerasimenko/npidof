#pragma once

#include "common.h"

namespace Array {
	template <typename TValue>
	struct T {
		size_t size;
		size_t used;
		TValue* items;
	};

	template <typename TValue>
	void init(Array::T<TValue>* a, size_t initialSize) {
		a->items = Common::createArray<TValue>(initialSize);
		a->size = initialSize;
		a->used = 0;
	}

	template <typename TValue>
	T<TValue>* create(size_t initialSize) {
		T<TValue>* a = Common::createEmpty<T<TValue>>();
		Array::init<TValue>(a, initialSize);

		return a;
	}

	template <typename TValue>
	void destroy(Array::T<TValue>** a) {
		free((*a)->items);
		free(*a);
		*a = NULL;
	}

	template <typename TValue>
	void push(Array::T<TValue>* a, TValue& value) {
		if (a->used == a->size) {
			a->size = a->size * 2;
			a->items = (TValue*)realloc(a->items, a->size * sizeof(TValue));
		}
		a->items[a->used++] = value;
	}
}