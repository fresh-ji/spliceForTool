/*
*                         Vortex OpenSplice
*
 *   This software and documentation are Copyright 2006 to  ADLINK
 *   Technology Limited, its affiliated companies and licensors. All rights
 *   reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
*
*/


/**
 * @file
 */

#ifndef ORG_OPENSPLICE_PUB_COHERENT_SET_DELEGATE_HPP_
#define ORG_OPENSPLICE_PUB_COHERENT_SET_DELEGATE_HPP_

#include <dds/pub/Publisher.hpp>


namespace org
{
namespace opensplice
{
namespace pub
{


class OMG_DDS_API CoherentSetDelegate
{
public:
    CoherentSetDelegate(const dds::pub::Publisher& pub);
    ~CoherentSetDelegate();

    void end();

    bool operator ==(const CoherentSetDelegate& other) const;

    dds::pub::Publisher get_publisher() const {
        return this->pub;
    }

private:
    dds::pub::Publisher pub;
    bool ended;
};

}
}
}

#endif /* ORG_OPENSPLICE_PUB_COHERENT_SET_DELEGATE_HPP_ */
