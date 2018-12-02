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

#include "NCursor.h"
#include "PlatfromIntegration.h"

namespace Nakama {

	std::string NCursor::Serialize() {
		return base64(value.c_str(), value.length());
	}

	void NCursor::Restore(std::string serialized) {
		value = unbase64_to_str(serialized.c_str(), serialized.length());
	}
}
