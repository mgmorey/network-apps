// Copyright (C) 2022  "Michael G. Morey" <mgmorey@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "network/get-templates-endpoint.h"     // get_templates()
#include "network/endpoint.h"                   // Endpoint
#include "network/insert-addrinfo.h"            // insert_addrinfo()
#include "network/optionalhints.h"              // OptionalHints
#include "network/templatevector.h"             // TemplateVector
#include "network/templatevectorresult.h"       // TemplateVectorResult

#include <iterator>     // std::back_inserter()

auto Network::get_templates(const Endpoint& endpoint,
                            const OptionalHints& hints,
                            bool verbose) -> TemplateVectorResult
{
    TemplateVector templates;
    auto result {
        insert_addrinfo(endpoint.first,
                        endpoint.second,
                        hints,
                        std::back_inserter(templates),
                        verbose)
    };

    if (result) {
        return result;
    }

    return templates;
}
