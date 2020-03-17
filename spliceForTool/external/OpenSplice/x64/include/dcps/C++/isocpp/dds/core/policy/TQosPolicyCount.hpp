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
#ifndef OSPL_DDS_CORE_POLICY_TQOSPOLICYCOUNT_HPP_
#define OSPL_DDS_CORE_POLICY_TQOSPOLICYCOUNT_HPP_

/**
 * @file
 */

/*
 * OMG PSM class declaration
 */
#include <spec/dds/core/policy/TQosPolicyCount.hpp>

// Implementation

namespace dds
{
namespace core
{
namespace policy
{

template <typename D>
TQosPolicyCount<D>::TQosPolicyCount(QosPolicyId policy_id, int32_t count) : dds::core::Value<D>(policy_id, count) { }

template <typename D>
TQosPolicyCount<D>::TQosPolicyCount(const TQosPolicyCount& other) : dds::core::Value<D>(other.policy_id(), other.count()) { }

template <typename D> QosPolicyId TQosPolicyCount<D>::policy_id() const
{
    return this->delegate().policy_id();
}

template <typename D>
int32_t TQosPolicyCount<D>::count() const
{
    return this->delegate().count();
}

}
}
}

// End of implementation

#endif /* OSPL_DDS_CORE_POLICY_TQOSPOLICYCOUNT_HPP_ */
