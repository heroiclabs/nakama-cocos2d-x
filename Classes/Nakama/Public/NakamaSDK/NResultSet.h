/**
 * Copyright 2017 The Nakama Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "NCursor.h"

// Implementation inlined
namespace Nakama {

	template <class T>
	class NAKAMA_API NResultSet {

	public:
		NResultSet(const std::vector<T>& res, const NCursor& cur) :
			results(res),
			cursor(cur) {}

		~NResultSet() {}

		const std::vector<T>& GetResults() const { return results; }
		const NCursor& GetCursor() const { return cursor; }
		int32_t Size() { return results.size(); }

		const T& operator [](int i) const
		{
			return results[i];
		}

	private:
		std::vector<T> results;
		NCursor cursor;

	};
}