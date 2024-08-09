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

#include "network/get-templates-hostname.h"     // get_templates()
#include "network/insert.h"                     // insert()
#include "network/optionalhints.h"              // OptionalHints
#include "network/optionalhostname.h"           // OptionalHostname
#include "network/optionalservice.h"            // OptionalService
#include "network/templatevector.h"             // TemplateVector
#include "network/templatevectorresult.h"       // TemplateVectorResult

#include <iterator>     // std::back_inserter()

auto Network::get_templates(const OptionalHostname& hostname,
                            const OptionalService& service,
                            const OptionalHints& hints,
                            bool verbose) -> TemplateVectorResult
{
    TemplateVector templates;
    auto it {std::back_inserter(templates)};
    auto result {insert(hostname, service, hints, it, verbose)};

    if (result) {
        return result;
    }

    return templates;
}
